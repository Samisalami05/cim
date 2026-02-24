#include "screen.h"
#include "terminal.h"
#include <stdint.h>
#include <stdio.h>

uint8_t screen_init(screen* s) {
	if (!terminal_size(&s->width, &s->height)) return 0;
	return 1;
}

void screen_deinit(screen* s) {

}

uint8_t screen_set_c(screen* s, char c, uint16_t x, uint16_t y) {
	if (x >= s->width || y >= s->height) {
		fprintf(stderr, "screen: Cant set char: Coordinate %d, %d out of bound\n", x, y);
		return 0;
	}
	terminal_moveto(x, y);
	putchar(c);

	return 1;
}

uint8_t screen_set_n(screen* s, char* data, uint16_t size, uint16_t x, uint16_t y) {
	if (x + size >= s->width || y >= s->height) {
		fprintf(stderr, "screen: Cant set %d characters: Coordinate %d, %d out of bound\n", size, x, y);
		return 0;
	}

	terminal_moveto(x, y);
	for (uint16_t i = 0; i < size; i++) {
		putchar(data[i]);
	}

	return 1;
}


