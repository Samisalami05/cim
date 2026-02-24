# CIM

Cim is a light weight text editor written in pure c.

## Panels

## Buffers

## Plugins

## Key Events

The input system consists of key events. The key event 
can both be a printable character like ascii or an control
key like pressing the arrow key.

### Printable Keys

```c
key_event key; // Key to check

if (!(key & KEY_SPECIAL)) {
    // Normal character or UTF-8 codepoint
    uint32_t cp = key & KEY_MASK;

    // Example: encoding into bytes
    char bytes[4];
    size_t n = utf8_encode(cp, bytes);

    // Example: check for specific ASCII
    if (cp == 'q') {
        printf("Pressed q\n");
    }
}
```
### Special Keys

The special keys are given in a enum and are accessible
through the `KEY_MASK`.

```c
if (key & KEY_SPECIAL) {
    uint32_t special = key & KEY_MASK;

    switch (special) {
        case KEY_ARROW_UP: move_cursor_up(); break;
        case KEY_ARROW_DOWN: move_cursor_down(); break;
        case KEY_ARROW_LEFT: move_cursor_left(); break;
        case KEY_ARROW_RIGHT: move_cursor_right(); break;
        case KEY_DELETE: delete_char(); break;
        case KEY_HOME: move_cursor_home(); break;
        case KEY_END: move_cursor_end(); break;
    }
}
```

### Modifiers

The modifiers are also given in a enum. 

```c
if (key & KEY_CTRL) {
    printf("Ctrl is pressed!\n");
}

if (key & KEY_ALT) {
    printf("Alt is pressed!\n");
}

if (key & KEY_SHIFT) {
    printf("Shift is pressed!\n");
}
```

They are stored in high bits and can be combined with 
either text or special keys.

```c
if ((key & KEY_SPECIAL) && (key & KEY_CTRL) && ((key & KEY_MASK) == KEY_ARROW_UP)) {
    printf("Ctrl + Arrow Up pressed!\n");
}
```
