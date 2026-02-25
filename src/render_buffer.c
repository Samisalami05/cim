#include "render_buffer.h"
#include <stdlib.h>

void render_buffer_init(render_buffer* buf, uint16_t width, uint16_t height) {
	buf->width = width;
	buf->height = height;
	buf->data = calloc(width * height, 1);
}

void render_buffer_deinit(render_buffer* buf) {
	free(buf->data);
}

void render_buffer_resize(render_buffer* buf, uint16_t width, uint16_t height) {
	if (buf->width * buf->height >= width * height) return; // buffer is already big enough

	buf->data = realloc(buf->data, width * height);
}
