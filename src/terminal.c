#include "terminal.h"
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

void terminal_init(terminal* t) {
	tcgetattr(STDIN_FILENO, &t->def_mode);
    t->raw_mode = t->def_mode;
	t->raw_mode.c_lflag &= ~(ICANON | ECHO);
}

uint8_t terminal_size(uint16_t* width, uint16_t* height) {
	struct winsize ws;
	if (ioctl( STDIN_FILENO , TIOCGWINSZ, &ws ) != 0 &&
		ioctl( STDOUT_FILENO, TIOCGWINSZ, &ws ) != 0 &&
		ioctl( STDERR_FILENO, TIOCGWINSZ, &ws ) != 0 ) {
		fprintf( stderr,
			"ioctl failed: %s\n", strerror(errno)
		);
		return 0;
	}

	if (width) *width = ws.ws_col;
	if (height) *height = ws.ws_row;
	return 1;
}

void terminal_moveto(uint16_t x, uint16_t y) {
	printf("\x1b[%d;%dH", y + 1, x + 1);
}

void terminal_clear() {
	uint16_t width, height;
	terminal_size(&width, &height);
	terminal_moveto(0, 0);

	// print spaces
	char spaces[width * height + 1];
	memset(spaces, ' ', width * height);
	spaces[width * height] = '\0';
	printf("%s", spaces);

	terminal_moveto(0, 0);
}

void terminal_main_screen() {
	printf("\x1b[?1049l");
}

void terminal_alternative_screen() {
	printf("\x1b[?1049h");
}

void terminal_enable_rawmode(terminal* t) {
    tcsetattr(STDIN_FILENO, TCSANOW, &t->raw_mode);
}

void terminal_disable_rawmode(terminal* t) {
    tcsetattr(STDIN_FILENO, TCSANOW, &t->def_mode);
}

void terminal_hide_cursor() {
	printf("\e[?25l");
}

void terminal_show_cursor() {
	printf("\e[?25h");
}
