#include "cursor.h"
#include "gapbuffer.h"
#include "linebuffer.h"

void cursor_init(cursor* cur, linebuffer* line_buf, gapbuffer* gap_buf) {
	cur->curr_line_buf = line_buf;
	cur->curr_gap_buf = gap_buf;
	cur->line_pos = 0;
	cur->line_num = 0;
	cur->hidden = 0;
}

void cursor_move_up(cursor* cur) {
	if (cur->line_num <= 0) return;
	cur->line_num--;
	linebuffer_move(cur->curr_line_buf, -1);

	line_data line;
	linebuffer_line(cur->curr_line_buf, &line, cur->line_num);

	if (line.length <= cur->line_pos) {
		cur->line_pos = line.length - 1;
	}

	gapbuffer_moveto(cur->curr_gap_buf, line.offset + cur->line_pos);
}

void cursor_move_down(cursor* cur) {
	if (cur->line_num >= cur->curr_line_buf->count - 1) return;
	cur->line_num++;
	linebuffer_move(cur->curr_line_buf, 1);

	line_data line;
	linebuffer_line(cur->curr_line_buf, &line, cur->line_num);

	if (line.length <= cur->line_pos) {
		cur->line_pos = line.length - 1;
	}

	gapbuffer_moveto(cur->curr_gap_buf, line.offset + cur->line_pos);
}

void cursor_move_left(cursor* cur) {
	if (cur->line_pos <= 0) return;
	gapbuffer_move(cur->curr_gap_buf, -1);
	cur->line_pos--;
}

void cursor_move_right(cursor* cur) {
	line_data line;
	linebuffer_line(cur->curr_line_buf, &line, cur->line_num);
	if (cur->line_pos >= line.length - 1) return;

	gapbuffer_move(cur->curr_gap_buf, 1);
	cur->line_pos++;
}

void cursor_moveto(cursor* cur, size_t pos) {
	
}
