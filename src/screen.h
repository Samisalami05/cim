#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

typedef struct {
	uint16_t width;
	uint16_t height;

	// TODO: implement color
} screen;

uint8_t screen_init(screen* s);
void screen_deinit(screen* s);

uint8_t screen_set_c(screen* s, char c, uint16_t x, uint16_t y);
uint8_t screen_set_n(screen* s, char* data, uint16_t size, uint16_t x, uint16_t y);

#endif
