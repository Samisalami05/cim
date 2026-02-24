#include "cim.h"
#include "gapbuffer.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "terminal.h"

typedef struct {
	uint16_t width;
	uint16_t height;
	gapbuffer buf;
	size_t line;
} panel;

static uint8_t panel_init(panel* p) {
	if (!terminal_size(&p->width, &p->height)) return 0;
	if (!gapbuffer_init_f(&p->buf, "src/main.c")) return 0;
	p->line = 0;
	return 1;
}

static void panel_deinit(panel* p) {
	gapbuffer_deinit(&p->buf);
}

int main(void) {
	cim* prog = cim_init();
	if (prog == NULL) {
		fprintf(stderr, "Failed to initialize cim\n");
		return 1;
	}

	if (!cim_run(prog)) return 1;

	return 0;
}
