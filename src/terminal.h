#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>
#include <termios.h>

typedef struct {
	struct termios def_mode;
	struct termios raw_mode;

	// Maybe supported colors
} terminal;

void terminal_init(terminal* t);

uint8_t terminal_size(uint16_t* width, uint16_t* height);
void terminal_moveto(uint16_t x, uint16_t y);
void terminal_clear();

void terminal_main_screen();
void terminal_alternative_screen();

void terminal_enable_rawmode(terminal* t);
void terminal_disable_rawmode(terminal* t);

void terminal_hide_cursor();
void terminal_show_cursor();

#endif
