#ifndef SHELL_H
#define SHELL_H

/* shell.h - KhazarOS Shell Interface */

#include "../include/typint.h"
#include "../include/vga.h"
#include "../include/keybrd/keybrd.h"

// Arrow key 
#define ARROW_KEY_UP    0x80
#define ARROW_KEY_DOWN  0x81
#define ARROW_KEY_LEFT  0x82
#define ARROW_KEY_RIGHT 0x83

#define KB_BUFFER_SIZE 64

void keyboard_buffer_push(uint8_t c);
int  keyboard_poll_char(uint8_t *out); 

// VGA wrapper-lar
void print_character(char c);
void print_string(const char *s);
void print_hex_byte(uint8_t b);
void move_cursor(int delta);   

/* Utility funksiyalar */
uint64_t string_to_hex(const char *s);
void     itoa(uint32_t n, char *buf); /* decimal string */


/* Random (LCG - basuc, seed = PIT tick counter) */
uint32_t random(void);
void     random_seed(uint32_t s);


void shell(void);

#endif /* SHELL_H */