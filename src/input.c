#include "input.h"
#include <unistd.h>
#include <stdio.h>

static key_event read_escape_sequence(void) {
    uint8_t seq[2];

    if (read(STDIN_FILENO, &seq[0], 1) != 1)
        return KEY_SPECIAL;

    if (read(STDIN_FILENO, &seq[1], 1) != 1)
        return KEY_SPECIAL;

    if (seq[0] == '[') {
        switch (seq[1]) {
            case 'A': return KEY_SPECIAL | KEY_ARROW_UP;
            case 'B': return KEY_SPECIAL | KEY_ARROW_DOWN;
            case 'C': return KEY_SPECIAL | KEY_ARROW_RIGHT;
            case 'D': return KEY_SPECIAL | KEY_ARROW_LEFT;
        }
    }

    return KEY_SPECIAL;
}

static key_event read_utf8(uint8_t first) {
    if ((first & 0x80) == 0)
        return first;

    int additional = 0;

    if ((first & 0xE0) == 0xC0)
        additional = 1;
    else if ((first & 0xF0) == 0xE0)
        additional = 2;
    else if ((first & 0xF8) == 0xF0)
        additional = 3;
    else
        return 0;

    uint8_t buf[3];

    for (int i = 0; i < additional; i++) {
        if (read(STDIN_FILENO, &buf[i], 1) != 1)
            return 0;

        if ((buf[i] & 0xC0) != 0x80)
            return 0;
    }

    uint32_t cp = 0;

    if (additional == 1)
        cp = ((first & 0x1F) << 6) |
             (buf[0] & 0x3F);

    else if (additional == 2)
        cp = ((first & 0x0F) << 12) |
             ((buf[0] & 0x3F) << 6) |
             (buf[1] & 0x3F);

    else if (additional == 3)
        cp = ((first & 0x07) << 18) |
             ((buf[0] & 0x3F) << 12) |
             ((buf[1] & 0x3F) << 6) |
             (buf[2] & 0x3F);

    return cp;
}

key_event read_key(void) {
    uint8_t c;

    if (read(STDIN_FILENO, &c, 1) != 1)
        return 0;

	if (c == '\r' || c == '\n') {
		return KEY_SPECIAL | KEY_ENTER;
	}

    if (c == 0x1B)
        return read_escape_sequence();

    // Ctrl keys (ASCII control range)
    if (c <= 0x1F) {
        return KEY_CTRL | (c + 'A' - 1);
    }

    return read_utf8(c);
}

/* Takes in a utf-8 codepoint and encodes it into bytes.
 * The maximum number of bytes is 4.
 * @param cp The utf-8 codepoint
 * @param out The byte pointer output
 *
 * @return The number of bytes
 */
size_t utf8_encode(uint32_t cp, char *out)
{
    if (cp <= 0x7F) {
        out[0] = cp & 0x7F;
        return 1;
    } else if (cp <= 0x7FF) {
        out[0] = 0xC0 | ((cp >> 6) & 0x1F);
        out[1] = 0x80 | (cp & 0x3F);
        return 2;
    } else if (cp <= 0xFFFF) {
        out[0] = 0xE0 | ((cp >> 12) & 0x0F);
        out[1] = 0x80 | ((cp >> 6) & 0x3F);
        out[2] = 0x80 | (cp & 0x3F);
        return 3;
    } else if (cp <= 0x10FFFF) {
        out[0] = 0xF0 | ((cp >> 18) & 0x07);
        out[1] = 0x80 | ((cp >> 12) & 0x3F);
        out[2] = 0x80 | ((cp >> 6) & 0x3F);
        out[3] = 0x80 | (cp & 0x3F);
        return 4;
    }
    return 0;
}
