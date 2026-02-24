#include "cim.h"
#include "gapbuffer.h"
#include "input.h"
#include "screen.h"
#include "terminal.h"
#include <stdint.h>
#include <stdlib.h>

cim* cim_init() {
	cim* c = malloc(sizeof(cim));
	gapbuffer_init(&c->buffer);
	terminal_init(&c->terminal);
	screen_init(&c->screen);
	return c;
}

static void deinit(cim* c) {
	gapbuffer_deinit(&c->buffer);
	screen_deinit(&c->screen);
	free(c);
}

uint8_t cim_run(cim* c) {
	terminal_alternative_screen();
	terminal_clear();
	terminal_enable_rawmode(&c->terminal);

	printf("Press any key (q to quit):\n");

	char utf8_bytes[5];
	for (;;) {
		key_event key = read_key();  // returns immediately
        if (key == 'q')
            break;
		if (key == ':')
			printf("start command\n");

		uint32_t special = key & KEY_MASK;

		if (special == KEY_ARROW_UP) printf("Arrow up\n");

		if (key & KEY_CTRL) {
			printf("Ctrl is pressed!\n");
		}

		if (key & KEY_ALT) {
			printf("Alt is pressed!\n");
		}

		if (key & KEY_SHIFT) {
			printf("Shift is pressed!\n");
		}

		if (key & KEY_SPECIAL) continue;
		
		size_t n = utf8_encode(special, utf8_bytes);
		utf8_bytes[n] = '\0';
		
        printf("You pressed: %d, %s\n", key, utf8_bytes);
		for (size_t i = 0; i < n; i++)
			printf("%02x ", (unsigned char)utf8_bytes[i]);
		printf("\n");
		gapbuffer_append_n(&c->buffer, (const uint8_t*)utf8_bytes, n);
	}

	terminal_disable_rawmode(&c->terminal);
	terminal_main_screen();

	gapbuffer_append(&c->buffer, '\0');
	printf("file: %s\n", c->buffer.data);

	deinit(c);
	return 1;
}
