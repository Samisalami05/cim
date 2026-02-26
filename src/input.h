#ifndef INPUT_H
#define INPUT_H

#include <stddef.h>
#include <stdint.h>

#define KEY_SPECIAL  (1u << 31)
#define KEY_CTRL     (1u << 30)
#define KEY_ALT      (1u << 29)
#define KEY_SHIFT    (1u << 28)
#define KEY_MASK     0x0FFFFFFFu

enum {
    KEY_ARROW_UP = 1,
    KEY_ARROW_DOWN,
    KEY_ARROW_RIGHT,
    KEY_ARROW_LEFT,
    KEY_HOME,
    KEY_END,
	KEY_ENTER,
    KEY_DELETE,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN
};

typedef uint32_t key_event;

key_event read_key();
size_t utf8_encode(uint32_t cp, char *out);


#endif
