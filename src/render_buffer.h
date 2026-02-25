#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H

#include <stdint.h>

typedef struct {
	uint8_t* data;
	uint16_t width;
	uint16_t height;

	// TODO: Add color data
} render_buffer;

void render_buffer_init(render_buffer* buf, uint16_t width, uint16_t height);
void render_buffer_deinit(render_buffer* buf);

void render_buffer_resize(render_buffer* buf, uint16_t width, uint16_t height);

#endif
