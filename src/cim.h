#ifndef CIM_H
#define CIM_H

#include "cursor.h"
#include "gapbuffer.h"
#include "screen.h"
#include "terminal.h"
#include <stdint.h>

typedef struct {
	gapbuffer buffer;
	screen screen;
	terminal terminal;
	cursor cursor;
} cim;

cim* cim_init();
uint8_t cim_run(cim* c);

#endif
