#include "cim.h"
#include "cursor.h"
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
	cursor_init(&c->cursor, &c->line_buf, &c->gap_buf);

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

	for (int line = scroll; line < scroll + c->line_buf.count; line++) {
		line_data curr_line;
		if (!linebuffer_line(&c->line_buf, &curr_line, line)) return;

		char str[curr_line.length + 1];
		if (!gapbuffer_read(&c->gap_buf, (uint8_t*)str, curr_line.offset, curr_line.length)) return;
		str[curr_line.length] = '\0';

		printf(" %d:%ld:%ld %s", line, curr_line.length, curr_line.offset, str);
	}
	printf("\nEND\n");
	printf("%ld\n", c->gap_buf.gap_start);
	print_buffer_content(&c->gap_buf);
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
	linebuffer_append(&c->line_buf, 0, 0);

	for (;;) {
		//screen_set_n(&c->screen, (char*)c->render_buf.data, c->render_buf.width * c->render_buf.height, 0, 0);
		//render(c, scroll);
		
		//print_buffer_content(&c->gap_buf);
		linebuffer_print(&c->line_buf);
		gapbuffer_print(&c->gap_buf);

		key_event key = read_key();  // returns immediately
        if (key == 'q')
            break;
		if (key == ':')
			printf("start command\n");

		uint32_t special = key & KEY_MASK;

		if (special == KEY_ARROW_LEFT) cursor_move_left(&c->cursor);
		if (special == KEY_ARROW_RIGHT) cursor_move_right(&c->cursor);
		if (special == KEY_ARROW_UP) cursor_move_up(&c->cursor);
		if (special == KEY_ARROW_DOWN) cursor_move_down(&c->cursor);

		if (special == KEY_ENTER) {
			if (!gapbuffer_append(&c->gap_buf, '\n')) return 0;
			if (!linebuffer_scale_line(&c->line_buf, c->cursor.line_num, 1)) return 0;
			if (!linebuffer_append(&c->line_buf, c->gap_buf.gap_start, 0)) return 0; // TODO: need to split if in middle of line
			c->cursor.line_num++;
			c->cursor.line_pos = 0;
		}

		if (key == 127) { // backspace
			gapbuffer_remove(&c->gap_buf); // TODO: Do line removal aswell
			continue;
		}

		if (key & KEY_SPECIAL) continue;
		
		size_t n = utf8_encode(special, utf8_bytes);
       	if (!gapbuffer_append_n(&c->gap_buf, (const uint8_t*)utf8_bytes, n)) return 0;
		if (!linebuffer_scale_line(&c->line_buf, c->cursor.line_num, 1)) return 0;
	}

	terminal_disable_rawmode(&c->terminal);
	terminal_main_screen();

	deinit(c);
	return 1;
}
