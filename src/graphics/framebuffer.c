#include "multiboot.h"
#include "types.h"
#include "vga.h"

U0 testFrameb(multiboot2_tag_framebuffer_t *fb){
    putstr((STR8_C)"FrameBuffer detected !");

    putstr((STR8_C)"Addr:  "); putdec((U0*)fb->framebuffer_addr); putstr((STR8_C)"\n");
    putstr((STR8_C)"Width:  "); putdec(fb->framebuffer_width); putstr((STR8_C)"\n");
    putstr((STR8_C)"Height:  "); putdec(fb->framebuffer_height); putstr((STR8_C)"\n");
    putstr((STR8_C)"Pitch:  "); putdec(fb->framebuffer_pitch); putstr((STR8_C)"\n");
    putstr((STR8_C)"BPP:  "); putdec(fb->framebuffer_bpp); putstr((STR8_C)"\n");
}
