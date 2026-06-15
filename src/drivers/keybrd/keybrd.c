/* keybrd.c - KhazarOS Keyboard Driver
 * IRQ1 callback scancode-u ASCII-ye çevirir,
 * shell-in ring bufferine (keyboard_buffer_push) yazir.
 */

#include "../../include/keybrd/keybrd.h"
#include "../../include/shell.h"      /* keyboard_buffer_push, ARROW_KEY_* */
#include "../../include/idt.h"
#include "../../include/isr.h"
#include "../../include/io.h"
#include "../../include/typint.h"

#define IRQ1        33
#define KB_DATA     0x60
#define KB_STATUS   0x64

/* ─── Scancode Set 1 → ASCII Tablosu ──────────────────────────
*/
static const uint8_t scancode_ascii[128] = {
/*00*/  0,
/*01*/  0,           /* Esc          */
/*02*/  '1','2','3','4','5','6','7','8','9','0','-','=',
/*0e*/  '\b',        /* Backspace    */
/*0f*/  '\t',        /* Tab          */
/*10*/  'q','w','e','r','t','y','u','i','o','p','[',']',
/*1c*/  '\n',        /* Enter        */
/*1d*/  0,           /* L-Ctrl       */
/*1e*/  'a','s','d','f','g','h','j','k','l',';','\'','`',
/*2a*/  0,           /* L-Shift      */
/*2b*/  '\\',
/*2c*/  'z','x','c','v','b','n','m',',','.','/',
/*36*/  0,           /* R-Shift      */
/*37*/  '*',
/*38*/  0,           /* L-Alt        */
/*39*/  ' ',
/*3a*/  0,           /* Caps Lock    */
/* F1-F10 (3b-44) */
        0,0,0,0,0,0,0,0,0,0,
/*45*/  0,           /* Num Lock     */
/*46*/  0,           /* Scroll Lock  */
/* Numpad 7-9, -, 4-6, +, 1-3, 0, . */
        0,0,0,0,0,0,0,0,0,0,0,0,0,
/*54*/  0,0,0,
/*57*/  0,           /* F11          */
/*58*/  0,           /* F12          */
/* 59-7f: doldurma */
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

/* Shift basılı olduqda alternate ASCII */
static const uint8_t scancode_shift[128] = {
/*00*/  0,
/*01*/  0,
/*02*/  '!','@','#','$','%','^','&','*','(',')','_','+',
/*0e*/  '\b',
/*0f*/  '\t',
/*10*/  'Q','W','E','R','T','Y','U','I','O','P','{','}',
/*1c*/  '\n',
/*1d*/  0,
/*1e*/  'A','S','D','F','G','H','J','K','L',':','"','~',
/*2a*/  0,
/*2b*/  '|',
/*2c*/  'Z','X','C','V','B','N','M','<','>','?',
/*36*/  0,
/*37*/  '*',
/*38*/  0,
/*39*/  ' ',
/*3a*/  0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,
        0,
        0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

/* Driver state */
static int shift_pressed = 0;  /* L veya R Shift */
static int e0_prefix     = 0;  /* Extended scancode prefiksi */

// IRQ1 Callback
static void keyboard_callback(registers_t *regs)
{
    (void)regs;
    uint8_t sc = byte_i(KB_DATA);

    // Extended key prefiksi
    if (sc == 0xE0) {
        e0_prefix = 1;
        return;
    }

    if (e0_prefix) {
        e0_prefix = 0;
        /* Break code (key release)*/
        if (sc & 0x80) return;

        switch (sc) {
            case 0x48: keyboard_buffer_push(ARROW_KEY_UP);    return;
            case 0x50: keyboard_buffer_push(ARROW_KEY_DOWN);  return;
            case 0x4B: keyboard_buffer_push(ARROW_KEY_LEFT);  return;
            case 0x4D: keyboard_buffer_push(ARROW_KEY_RIGHT); return;
            default:   return;  
        }
    }

    /* Break code yalnız Shift-i izlə */
    if (sc & 0x80) {
        uint8_t make = sc & 0x7F;
        if (make == 0x2A || make == 0x36)   /* L/R Shift release */
            shift_pressed = 0;
        return;
    }

    if (sc == 0x2A || sc == 0x36) {
        shift_pressed = 1;
        return;
    }

    if (sc < 128) {
        uint8_t ch = shift_pressed ? scancode_shift[sc] : scancode_ascii[sc];
        if (ch != 0)
            keyboard_buffer_push(ch);
    }
}

// Init
void init_keyboard(void)
{
    register_interrupt_handler(IRQ1, keyboard_callback);
}