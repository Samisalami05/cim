#include "cim.h"
#include "gapbuffer.h"
#include "input.h"
#include "linebuffer.h"
#include "render_buffer.h"
#include "screen.h"
#include "terminal.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

cim* cim_init() {
	cim* c = malloc(sizeof(cim));
	gapbuffer_init(&c->gap_buf);
	linebuffer_init(&c->line_buf);

	terminal_init(&c->terminal);
	screen_init(&c->screen);
	render_buffer_init(&c->render_buf, c->screen.width, c->screen.height); // TODO: Should not depend on screen
	return c;
}

static void deinit(cim* c) {
	gapbuffer_deinit(&c->gap_buf);
	linebuffer_deinit(&c->line_buf);

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

static void render(cim* c, int scroll) {
	if (c->line_buf.count == 0) return;

	terminal_clear();

	size_t curr_off = -1;
	if (!linebuffer_line_offset(&c->line_buf, &curr_off, scroll)) return;

	for (int line = scroll + 1; line < scroll + c->line_buf.count; line++) {
		size_t next_off;
		linebuffer_line_offset(&c->line_buf, &next_off, line);

		size_t length = next_off - curr_off;
		char str[length + 1];
		gapbuffer_read(&c->gap_buf, (uint8_t*)str, curr_off, length);
		str[length] = '\0';
		printf(" %d:%ld:%ld %s", line, length, curr_off, str);
		curr_off = next_off;
	}
	size_t length = c->gap_buf.size - curr_off;
	char str[length + 1];
	gapbuffer_read(&c->gap_buf, (uint8_t*)str, curr_off, length);
	str[length] = '\0';
	printf(" %ld:%ld:%ld %s", scroll + c->line_buf.count, length, curr_off, str);
	printf("\nEND\n");
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

	char utf8_bytes[4];
	int scroll = 0;
	linebuffer_append(&c->line_buf, 0);

	for (;;) {
		//screen_set_n(&c->screen, (char*)c->render_buf.data, c->render_buf.width * c->render_buf.height, 0, 0);
		render(c, scroll);
		key_event key = read_key();  // returns immediately
        if (key == 'q')
            break;
		if (key == ':')
			printf("start command\n");

		uint32_t special = key & KEY_MASK;

		if (special == KEY_ARROW_LEFT) gapbuffer_move(&c->gap_buf, -1);
		if (special == KEY_ARROW_RIGHT) gapbuffer_move(&c->gap_buf, 1);
		//if (special == KEY_ARROW_UP) linebuffer_move(&c->line_buf, -1);
		//if (special == KEY_ARROW_DOWN) linebuffer_move(&c->line_buf, 1);

		if (special == KEY_ENTER) {
			gapbuffer_append(&c->gap_buf, '\n');
			linebuffer_append(&c->line_buf, c->gap_buf.gap_start);
		}

		if (key & KEY_SPECIAL) continue;
		
		size_t n = utf8_encode(special, utf8_bytes);
       	gapbuffer_append_n(&c->gap_buf, (const uint8_t*)utf8_bytes, n);

		print_buffer_content(&c->gap_buf);
	}

	terminal_disable_rawmode(&c->terminal);
	terminal_main_screen();

	
	deinit(c);
	return 1;
}
