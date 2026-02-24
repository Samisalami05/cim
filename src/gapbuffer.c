#include "gapbuffer.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

static uint8_t allocate(gapbuffer* buf, size_t size) {
	void* temp = realloc(buf->data, size);

	if (temp == NULL) {
		perror("buffer: realloc");
		return 0;
	}
	buf->data = temp;

	size_t right = buf->size - buf->gap_end;
	size_t total_right = buf->allocated - buf->gap_end;

	size_t move = total_right - right;
	memcpy(buf->data + buf->gap_end + move, buf->data + buf->gap_end, right);

	buf->gap_end += move;
	return 1;
}

// Resizes buffer if necessary
static uint8_t ensure_size(gapbuffer* buf, size_t size) {
	if (buf->allocated < size) {
		while (buf->allocated < size) {
			buf->allocated = buf->allocated == 0 ? 4 : buf->allocated * 2;
		}
		if (!allocate(buf, buf->allocated)) return 0;
	}
	return 1;
}

void gapbuffer_init(gapbuffer* buf) {
	buf->data = NULL;
	buf->allocated = 0;
	buf->size = 0;

	buf->gap_start = 0;
	buf->gap_end = 0;
}

uint8_t gapbuffer_init_f(gapbuffer* buf, const char* file_path) {
	gapbuffer_init(buf);

	FILE* f = fopen(file_path, "rb");
	if (f == NULL) {
		perror("buffer: fopen");
		return 0;
	}

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	uint8_t data[size];
	fread(data, size, 1, f);
	fclose(f);

	if (!gapbuffer_append_n(buf, data, size)) return 0;

	return 1;
}

void gapbuffer_deinit(gapbuffer* buf) {
	if (buf->data) free(buf->data);
}

uint8_t gapbuffer_append(gapbuffer* buf, uint8_t data) {
	if (!ensure_size(buf, buf->size + 1)) return 0;
	buf->data[buf->gap_start] = data;
	buf->gap_start++;
	buf->size++;
	return 1;
}

uint8_t gapbuffer_append_n(gapbuffer* buf, const uint8_t* data, size_t size) {
	if (!ensure_size(buf, buf->size + size)) return 0;
	memcpy(buf->data + buf->gap_start, data, size);
	buf->size += size;
	buf->gap_start += size;

	return 1;
}

uint8_t gapbuffer_move(gapbuffer* buf, int move) {
	if (move == 0) return 1;
	if (move < -buf->gap_start || move > buf->allocated - buf->gap_end) {
		fprintf(stderr, "gapbuffer: Invalid move: Trying to move outside of buffer\n");
		return 0;
	}

	if (move < 0) {
		memcpy(buf->data + buf->gap_end + move, buf->data + buf->gap_start + move, -move);
	}
	else {
		memcpy(buf->data + buf->gap_start, buf->data + buf->gap_end, move);
	}
	return 1;
}

uint8_t gapbuffer_moveto(gapbuffer* buf, size_t pos) {
	int move = pos - buf->gap_start;
	if (!gapbuffer_move(buf, move)) return 0;
	return 1;
}

uint8_t gapbuffer_read_c(gapbuffer* buf, uint8_t* dest, size_t off) {
	if (off < 0 || off >= buf->size) {
		fprintf(stderr, "Cant read from buffer: Trying to read out of bounds\n");
		return 0;
	}
	if (off >= buf->gap_start) {
		size_t gap = buf->gap_end - buf->gap_start;
		off += gap;
	}

	*dest = buf->data[off];
	return 1;
}

uint8_t gapbuffer_read(gapbuffer* buf, uint8_t* dest, size_t off, size_t size) {
	if (off < 0 || off + size >= buf->size) {
		fprintf(stderr, "Cant read from buffer: Trying to read out of bounds\n");
		return 0;
	}
	// TODO: Fix this
	if (off >= buf->gap_start) {
		size_t gap = buf->gap_end - buf->gap_start;
		off += gap;
	}

	// not reading over gap
	if (off + size < buf->gap_start || off > buf->gap_end) {
		memcpy(dest, buf->data + off, size);
	}
	else {
		printf("THIS is not implemented lol\n");
	}
	return 1;
}

void gapbuffer_clear(gapbuffer* buf) {
	buf->size = 0;
}
