
#include "../include/vga.h"
#include "../arch/x86_64/port_io.h"
#include "../include/typint.h"

#define VGA_CTRL_REGISTER 0x3d4 // vganin control registri
#define VGA_DATA_REGISTER 0x3d5 // vganin data registri
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

void cursor_set(int offset) // cursorun memory adressini setleyirik
{
  offset /= 2;
  byte_o(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
  byte_o(VGA_DATA_REGISTER, (U8)(offset >> 8));
  byte_o(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
  byte_o(VGA_DATA_REGISTER, (U8)(offset & 0xff));
}
int cursor_get() // cursorun memory adressini tapiriq
{
  byte_o(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
  int offset = byte_i(VGA_DATA_REGISTER) << 8;
  byte_o(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
  offset += byte_i(VGA_DATA_REGISTER);
  return offset * 2; // burda da qaqas her character 2 bit yer aldigi ucun ikiye
                     // vururuq yeni bele bisey
}

#define ADRESS ((uintptr_t)0xb8000)
#define MAX_ROW 25
#define MAX_COL 80
#define WHITE_BLACK 0x0f

void set_char_in_memory(U8 character, I32 offset) {
  U8 *vidmem = (U8 *)ADRESS;
  vidmem[offset] = character;
  vidmem[offset + 1] = WHITE_BLACK;
} // bunla da sen goturub ekranin hansisa memory adressine erisim edib bit
  // yazdira bilersen

// Komekci Funksiyalar

I32 get_row(I32 offset) {
  return offset / (2 * MAX_COL);
} // memory offsetini alir ve corresponding cellin row nomresini verir

I32 get_offset(I32 col, I32 row) {
  return 2 * (row * MAX_COL + col);
} // bize verilen cellin memory numberini qaytarir

I32 move_newl(I32 offset) { return get_offset(0, get_row(offset) + 1); }

// Scrolling and memory copy functions

void memorycpy(U8 *source, U8 *dest, I32 nbyte) {
  I32 i;
  for (i = 0; i < nbyte; i++) {
    *(dest + i) =
        *(source + i); // yeni eslinde 0x1000den 0x2000e bit kocurur tm?
  }
} /*We can implement the row movement by copying a chunk of the video memory.
First, we will write a function that copies a given number of bytes nbytes in
memory from *source to *dest.*/

I32 scrolln(I32 offset) {
  memorycpy((U8 *)(ADRESS + get_offset(0, 1)),
            (U8 *)(get_offset(0, 0) + ADRESS), MAX_COL * (MAX_ROW - 1) * 2);

  for (I32 col = 0; col < MAX_COL; col++) {
    set_char_in_memory(
        ' ',
        get_offset(col,
                   MAX_ROW - 1)); // linei temizleyirik və boşluqla doldururuq
  }

  return offset - 2 * MAX_COL;

  // bu funksiya terminal ve ya konsole ekranini yuxari kocurur

  /*
  Scrolldan əvvəl:        Scrolldan sonra:
┌──────────────┐        ┌──────────────┐
│  setir 1     │        │  setir 2     │  ← yuxarı çıxdı
│  setir 2     │  ───►  │  setir 3     │
│  setir 3     │        │              │  ← cleaned
└──────────────┘        └──────────────┘
  */
}

// alternative of printf in vga text

void putstr(STR8_C string) {
  I32 offset = cursor_get(); // cursorun oldugu yeri gotururuk offsete veririy
  I32 i = 0;
  while (string[i] != 0) {
    if (string[i] == '\n') {
      offset = move_newl(offset);
    } else {
      if (offset >= MAX_ROW * MAX_COL * 2) {
        offset = scrolln(offset);
      }
      set_char_in_memory(string[i], offset);
      offset += 2;
    }
    i++;
  }
  cursor_set(offset); // sonda da cursorun yerini set edirik
}

void putstr_color(STR8_C string, U8 color) {
  I32 offset = cursor_get();
  I32 i = 0;
  while (string[i] != 0) {
    if (string[i] == '\n') {
      offset = move_newl(offset);
    } else {
      if (offset >= MAX_ROW * MAX_COL * 2) {
        offset = scrolln(offset);
      }
      set_char_w_color(string[i], color, offset);
      offset += 2;
    }
    i++;
  }
  cursor_set(offset);
}

void clear() {
  I32 i;
  for (i = 0; i < MAX_COL * MAX_ROW; i++) {
    set_char_in_memory(' ', i * 2);
  }
  cursor_set(get_offset(0, 0));
}

void puthex(U64 n) {
  STR8_C hex_chars = "0123456789ABCDEF";
  I8 buffer[19];
  buffer[0] = '0';
  buffer[1] = 'x';
  for (int i = 0; i < 16; i++) {
    buffer[17 - i] = hex_chars[(n >> (i * 4)) & 0xF];
  }
  buffer[18] = 0;
  putstr((STR8_C)buffer);
}

void putdec(U64 n) {
  I8 buffer[21];
  buffer[20] = 0;

  if (n == 0) {
    putstr("0");
    return;
  }
  int i = 19;

  while (n > 0) {
    buffer[i] = '0' + (n % 10);
    n = n / 10;
    i--;
  }
  putstr((STR8_C)(buffer + i + 1));
}
// putdec komandasi ededi ekrana yazzdirmaga komek edir

// ekrana rengli yazi yazdirmaq
void set_char_w_color(U8 character, U8 color, I32 offset) {
  U8 *vidmem = (U8 *)ADRESS;
  vidmem[offset] = character;
  vidmem[offset + 1] = color;
}

// it helps to print characters to the screen
U0 kbd_putchar(char c) {
	int offset = cursor_get();

	if (c == '\n') {
		offset = move_newl(offset);
	} else if (c == '\b') {
		if (offset > 0) {
			offset -= 2; // char + color is 2 bytes
			set_char_in_memory(' ', offset);
		}
	} else {
		set_char_in_memory((U8)c, offset);
		offset += 2;
	}

	if (offset >= MAX_ROW * MAX_COL * 2) {
		offset = scrolln(offset);
	}
	cursor_set(offset);
}
