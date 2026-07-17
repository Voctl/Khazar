
CC   = gcc
NASM = nasm

CFLAGS = -m64 -ffreestanding -mcmodel=kernel -mno-red-zone \
         -fno-pic -fno-PIC -fno-pie -fno-stack-protector \
         -mgeneral-regs-only \
         -Isrc/include -Isrc/libk -Isrc/mm -Isrc -Isrc/drivers -Isrc/drivers/pit -Isrc/drivers/keyboard

BUILD      = src/build
ISO_DIR    = src/iso
KERNEL_BIN = $(BUILD)/kernel.bin
ISO_KERNEL = $(ISO_DIR)/boot/kernel.elf

# Auto-detect all C and Assembly sources
C_SRCS = $(shell find src -name '*.c')
S_SRCS = $(shell find src -name '*.s')

# Generate corresponding object file names
OBJS = $(patsubst src/%.c, $(BUILD)/%.o, $(C_SRCS)) \
       $(patsubst src/%.s, $(BUILD)/%.o, $(S_SRCS))

# Ensure loader is linked first
OBJS_NO_LOADER = $(filter-out $(BUILD)/boot/loader.o, $(OBJS))
LINK_OBJS = $(BUILD)/boot/loader.o $(OBJS_NO_LOADER)

all: iso

# Rule to compile C files
$(BUILD)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile Assembly files
$(BUILD)/%.o: src/%.s
	@mkdir -p $(dir $@)
	$(NASM) -f elf64 $< -o $@

$(KERNEL_BIN): $(OBJS) src/boot/link.ld
	@mkdir -p $(BUILD)
	ld -n -o $(KERNEL_BIN) -T src/boot/link.ld $(LINK_OBJS)

iso: $(KERNEL_BIN)
	@mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_KERNEL)
	grub-mkrescue -o khazar.iso $(ISO_DIR)

run: iso
	qemu-system-x86_64 -cdrom khazar.iso -m 2048 -display gtk -audiodev pa,id=speaker -machine pcspk-audiodev=speaker

clean:
	rm -rf $(BUILD)
	rm -f khazar.iso
