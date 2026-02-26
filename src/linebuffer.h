#ifndef LINEBUFFER_H
#define LINEBUFFER_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
	size_t* data; // offsets
	size_t allocated;
	size_t count;

	size_t gap_start;
	size_t gap_end;
} linebuffer;

void linebuffer_init(linebuffer* buf);
void linebuffer_deinit(linebuffer* buf);

uint8_t linebuffer_append(linebuffer* buf, size_t line_off);
uint8_t linebuffer_move(linebuffer* buf, int move);

uint8_t linebuffer_line_offset(linebuffer* buf, size_t* out, size_t line_num);

#endif
