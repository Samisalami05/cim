#ifndef LINEBUFFER_H
#define LINEBUFFER_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
	size_t offset;
	size_t length;
} line_data;

typedef struct {
	line_data* data; // offsets
	size_t allocated;
	size_t count;

	size_t gap_start;
	size_t gap_end;
} linebuffer;

void linebuffer_init(linebuffer* buf);
void linebuffer_deinit(linebuffer* buf);

uint8_t linebuffer_append(linebuffer* buf, size_t line_off, size_t line_len);
uint8_t linebuffer_scale_line(linebuffer* buf, size_t line_num, size_t inc);

uint8_t linebuffer_move(linebuffer* buf, int move);

uint8_t linebuffer_line(linebuffer* buf, line_data* out, size_t line_num);

void linebuffer_print(linebuffer* buf);

#endif
