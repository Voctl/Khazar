#include "types.h"
#include "vga.h"
#include "vga50.h"

U0 vga50(char *args) {
	(U0)args;

	vga_set_80x50();
}
