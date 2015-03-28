/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Keyboard driver
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>

volatile bool full = true;
volatile char ch = 0;

bool caps_cleared = true;
unsigned int kb_flags = 0;
#define FLAG_SHIFT 1
#define FLAG_CAPS_LOCK 2

#define SHIFT_L 42
#define SHIFT_R 54
#define ALT 55
#define CAPS_LOCK 58
#define ARROW_UP 72
#define ARROW_LEFT 74
#define ARROW_RIGHT 76
#define ARROW_DOWN 80

/* KBDUS means US Keyboard Layout. This is a scancode table
*  used to layout a standard US keyboard. I have left some
*  comments in to give you an idea of what key is what, even
*  though I set it's array index to 0. You can change that to
*  whatever you want using a macro, if you wish! */
unsigned char kbdus[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',		/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,					/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

unsigned char us_shift(const unsigned char c) {
    switch (c) {
        case '=':
            return '+';
        case '[':
            return '{';
        case ']':
            return '}';
        case '1':
            return '!';
        case '2':
            return '@';
        case '3':
            return '#';
        case '4':
            return '$';
        case '5':
            return '%';
        case '6':
            return '^';
        case '7':
            return '&';
        case '8':
            return '*';
        case '9':
            return '(';
        case '0':
            return ')';
        case '`':
            return '~';
        case ';':
            return ':';
        case '\'':
            return '\"';
        case ',':
            return '<';
        case '.':
            return '>';
        case '/':
            return '?';
        case '\\':
            return '|';
    }
    return toupper(c);
}

/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r) {
    unsigned char scancode;

    /* Read from the keyboard's data buffer */
    scancode = inportb(0x60);
    __asm__ __volatile__("cli");

    /* Key was released */
    if (scancode & 0x80) {
        switch (scancode - 0x80) {
            case SHIFT_L:
            case SHIFT_R:
                kb_flags &= ~FLAG_SHIFT;
                break;
            case CAPS_LOCK:
                caps_cleared = true;
                break;
        }
        full = false;
    }
    /* Key was pressed */
    else {
        switch (scancode) {
            case SHIFT_L:
            case SHIFT_R:
                kb_flags |= FLAG_SHIFT;
                break;
            case CAPS_LOCK:
                if (caps_cleared) {
                    caps_cleared = false;
                    kb_flags ^= FLAG_CAPS_LOCK;
                }
                break;
        }
        ch = kbdus[scancode];
        if (kb_flags & FLAG_CAPS_LOCK)
            ch = toupper(ch);
        if (kb_flags & FLAG_SHIFT)
            if (isupper(ch))
                ch = tolower(ch);
            else
                ch = us_shift(ch);
        full = true;
    }
    __asm__ __volatile__("sti");
}

unsigned char getch() {
    getch_start:
    __asm__ __volatile__("hlt");
    while (!full);
    full = false;
    if (ch)
        return ch;
    goto getch_start;
}

/* Installs the keyboard handler into IRQ1 */
void keyboard_install()
{
    irq_install_handler(1, keyboard_handler);
}
