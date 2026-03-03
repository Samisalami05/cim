#ifndef CURSOR_H
#define CURSOR_H

#include "gapbuffer.h"
#include "linebuffer.h"
#include <stddef.h>
#include <stdint.h>

typedef struct {
	size_t line_num;
	size_t line_pos;
	gapbuffer* curr_gap_buf;
	linebuffer* curr_line_buf;
	uint8_t hidden;
} cursor;

void cursor_init(cursor* cur, linebuffer* line_buf, gapbuffer* gap_buf);

void cursor_move_up(cursor* cur);
void cursor_move_down(cursor* cur);
void cursor_move_left(cursor* cur);
void cursor_move_right(cursor* cur);

void cursor_moveto(cursor* cur, size_t pos);

#endif
