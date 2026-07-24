#include "multiboot.h"
#include "types.h"
#include "vga.h"

U0 testFrameb(U64 multiboot_addr){
    multiboot_info_t *mbi = (multiboot_info_t*)multiboot_addr;
    
    if(mbi->flags & (1 << 12)){
        putstr((STR8_C)"FrameBuffer detected !");


        putstr((STR8_C)"Addr:  "); putdec((U0*)mbi->framebuffer_addr);
        putstr((STR8_C)"Width:  "); putdec(mbi->framebuffer_width);
        putstr((STR8_C)"Height:  "); putdec(mbi->framebuffer_height);
        putstr((STR8_C)"Pitch:  "); putdec(mbi->framebuffer_pitch);
        putstr((STR8_C)"BPP:  "); putdec(mbi->framebuffer_bpp);
    }else {
        putstr((STR8_C)"\nThere is no framebuffer\n");
    }

}
