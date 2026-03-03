#include "linebuffer.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void linebuffer_init(linebuffer* buf) {
	buf->data = NULL;
	buf->allocated = 0;
	buf->count = 0;

	buf->gap_start = 0;
	buf->gap_end = 0;
}

void linebuffer_deinit(linebuffer* buf) {
	if (buf->data) free(buf->data);
}

// size is number of lines
static uint8_t allocate(linebuffer* buf, size_t size) {
	line_data* temp = realloc(buf->data, size * sizeof(line_data));
	if (temp == NULL) {
		perror("linebuffer: realloc");
		return 0;
	}
	buf->data = temp;
	buf->allocated = size;

	size_t right = buf->count - buf->gap_start;
	if (right != 0) {
		size_t move = buf->allocated - right;
		memcpy(buf->data + move, buf->data + buf->gap_end + 1, right * sizeof(line_data));
		buf->gap_end = move - 1;
	}
	else {
		buf->gap_end = buf->allocated - 1;
	}

	return 1;
}

// size is number of lines
static uint8_t ensure_size(linebuffer* buf, size_t size) {
	if (buf->allocated < size) {
		size_t new_size = buf->allocated == 0 ? 4 : buf->allocated;
		while (new_size < size) new_size *= 2;

		if (!allocate(buf, new_size)) return 0;
	}

	return 1;
}

uint8_t linebuffer_append(linebuffer* buf, size_t line_off, size_t line_len) {
	if (!ensure_size(buf, buf->count + 1)) return 0;
	buf->data[buf->gap_start] = (line_data){ 
		.offset = line_off,
		.length = line_len,
	};

	buf->gap_start++;
	buf->count++;

	return 1;
}

// Increment offsets for all lines after the line
uint8_t linebuffer_scale_line(linebuffer* buf, size_t line_num, size_t inc) {
	if (line_num < 0 || line_num >= buf->count) {
		fprintf(stderr, "linebuffer: Cant read line %ld from buffer: Line does not exist\n", line_num);
		return 0;
	}

	size_t index = line_num;
	if (line_num >= buf->gap_start) {
		size_t gap = buf->gap_end - buf->gap_start + 1; // TODO: i think its +1
		index += gap;
	}

	buf->data[index].length += inc;
	for (size_t i = index + 1; i < buf->allocated; i++) {
		buf->data[i].offset += inc;
	}

	return 1;
}

uint8_t linebuffer_move(linebuffer* buf, int move) {
	if (move == 0) return 1;
	if (buf->gap_start + move < 0 || buf->gap_start + move > buf->count) {
		fprintf(stderr, "linebuffer: Invalid move: Trying to move outside of buffer\n");
		return 0;
	}

	if (move < 0) {
		memcpy(buf->data + buf->gap_end + 1 + move, buf->data + buf->gap_start + move, -move * sizeof(line_data));
	}
	else {
		memcpy(buf->data + buf->gap_start, buf->data + buf->gap_end + 1, move * sizeof(line_data));
	}
	
	buf->gap_start += move;
	buf->gap_end += move;

	return 1;

}

uint8_t linebuffer_line(linebuffer* buf, line_data* out, size_t line_num) {
	if (line_num < 0 || line_num >= buf->count) {
		fprintf(stderr, "linebuffer: Cant read line %ld from buffer: Line does not exist\n", line_num);
		return 0;
	}

	size_t index = line_num;
	if (line_num >= buf->gap_start) {
		size_t gap = buf->gap_end - buf->gap_start + 1; // TODO: i think its +1
		index += gap;
	}

	*out = buf->data[index];
	return 1;
}

void linebuffer_print(linebuffer* buf) {
	printf("[ ");
	for (int i = 0; i < buf->allocated; i++) {
		if ((i >= buf->gap_start && i <= buf->gap_end)) {
			printf("_ ");
			continue;
		}
		printf("(%ld, %ld) ", buf->data[i].offset, buf->data[i].length);
	}
	printf("]\n\n");
}
