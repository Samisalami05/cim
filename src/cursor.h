#ifndef CURSOR_H
#define CURSOR_H

#include "gapbuffer.h"
#include <stddef.h>
#include <stdint.h>

typedef struct {
	size_t pos;
	gapbuffer* curr_buf;
	uint8_t hidden;
} cursor;

void cursor_move_up(cursor* cur);
void cursor_move_down(cursor* cur);
void cursor_move_left(cursor* cur);
void cursor_move_right(cursor* cur);

void cursor_hide(cursor* cur);
void cursor_show(cursor* cur);

void cursor_moveto(cursor* cur, size_t pos);

#endif
