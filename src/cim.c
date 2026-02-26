#include "cim.h"
#include "gapbuffer.h"
#include "input.h"
#include "render_buffer.h"
#include "screen.h"
#include "terminal.h"
#include <stdint.h>
#include <stdlib.h>

cim* cim_init() {
	cim* c = malloc(sizeof(cim));
	gapbuffer_init(&c->buffer);
	terminal_init(&c->terminal);
	screen_init(&c->screen);
	render_buffer_init(&c->render_buf, c->screen.width, c->screen.height); // TODO: Should not depend on screen
	return c;
}

static void deinit(cim* c) {
	gapbuffer_deinit(&c->buffer);
	render_buffer_deinit(&c->render_buf);
	screen_deinit(&c->screen);
	free(c);
}

static void print_buffer_content(gapbuffer* buf) {
	char data[buf->size + 1];
	gapbuffer_read(buf, (uint8_t*)data, 0, buf->size);
	data[buf->size] = '\0';
	printf("file: %s\n", data);
}

uint8_t cim_run(cim* c) {
	terminal_alternative_screen();
	terminal_clear();
	terminal_enable_rawmode(&c->terminal);
	terminal_hide_cursor();

	printf("Press any key (q to quit):\n");

	for (int i = 0; i < c->render_buf.width * c->render_buf.height; i++) {
		c->render_buf.data[i] = '#';
	}

	for (int y = 0; y < c->screen.height; y++) {
		for (int x = 0; x < c->screen.width + 1; x++) {
			//if (x == c->screen.width) { printf("\n"); continue; }
			//printf("#");
		}
	}

	terminal_moveto(1, 1);
	fflush(stdout);

	char utf8_bytes[5];
	for (;;) {
		//screen_set_n(&c->screen, (char*)c->render_buf.data, c->render_buf.width * c->render_buf.height, 0, 0);
		
		key_event key = read_key();  // returns immediately
        if (key == 'q')
            break;
		if (key == ':')
			printf("start command\n");

		uint32_t special = key & KEY_MASK;

		if (special == KEY_ARROW_LEFT) gapbuffer_move(&c->buffer, -1);
		if (special == KEY_ARROW_RIGHT) gapbuffer_move(&c->buffer, 1);

		if (key & KEY_SPECIAL) continue;
		
		size_t n = utf8_encode(special, utf8_bytes);
		utf8_bytes[n] = '\0';
		
        //printf("You pressed: %d, %s\n", key, utf8_bytes);
		//for (size_t i = 0; i < n; i++)
		//	printf("%02x ", (unsigned char)utf8_bytes[i]);
		//printf("\n");
		gapbuffer_append_n(&c->buffer, (const uint8_t*)utf8_bytes, n);

		if (key == 'w') print_buffer_content(&c->buffer);
	}

	terminal_disable_rawmode(&c->terminal);
	terminal_main_screen();

	
	deinit(c);
	return 1;
}
