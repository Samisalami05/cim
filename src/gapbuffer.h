#ifndef GAPBUFFER_H
#define GAPBUFFER_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
	uint8_t* data;
	size_t allocated;
	size_t size;

	size_t gap_start;
	size_t gap_end;
} gapbuffer;

void gapbuffer_init(gapbuffer* buf);
uint8_t gapbuffer_init_f(gapbuffer* buf, const char* file_path);
void gapbuffer_deinit(gapbuffer* buf);

uint8_t gapbuffer_append(gapbuffer* buf, uint8_t data);
uint8_t gapbuffer_append_n(gapbuffer* buf, const uint8_t* data, size_t size);

// Moves gap
uint8_t gapbuffer_move(gapbuffer* buf, int move);
uint8_t gapbuffer_moveto(gapbuffer* buf, size_t pos);

uint8_t gapbuffer_read_c(gapbuffer* buf, uint8_t* dest, size_t off);
uint8_t gapbuffer_read(gapbuffer* buf, uint8_t* dest, size_t off, size_t size);

void gapbuffer_clear(gapbuffer* buf);

#endif
