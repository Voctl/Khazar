CC   = gcc
CXX  = g++
NASM = nasm

CFLAGS = -Wall -Wextra -m64 -ffreestanding -mcmodel=kernel -mno-red-zone \
         -fno-pic -fno-PIC -fno-pie -fno-stack-protector \
         -mgeneral-regs-only \
         -Isrc/include -Isrc/libk -Isrc/mm -Isrc -Isrc/drivers -Isrc/drivers/pit -Isrc/drivers/keyboard -Isrc/shell -Isrc/shell/commands

CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -fno-unwind-tables -fno-asynchronous-unwind-tables

BUILD      = src/build
ISO_DIR    = src/iso
KERNEL_BIN = $(BUILD)/kernel.bin
ISO_KERNEL = $(ISO_DIR)/boot/kernel.elf

# Auto-detect all C, C++ and Assembly sources
C_SRCS   = $(shell find src -name '*.c')
CPP_SRCS = $(shell find src -name '*.cpp')
S_SRCS   = $(shell find src -name '*.s')

# Generate corresponding object file names
OBJS = $(patsubst src/%.c, $(BUILD)/%.o, $(C_SRCS)) \
       $(patsubst src/%.cpp, $(BUILD)/%.o, $(CPP_SRCS)) \
       $(patsubst src/%.s, $(BUILD)/%.o, $(S_SRCS))

# Ensure loader is linked first
OBJS_NO_LOADER = $(filter-out $(BUILD)/boot/loader.o, $(OBJS))
LINK_OBJS = $(BUILD)/boot/loader.o $(OBJS_NO_LOADER)

all: iso

# Rule to compile C files
$(BUILD)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile C++ files
$(BUILD)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

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
	qemu-system-x86_64 -enable-kvm -cpu host -smp 2 -m 2048 -cdrom khazar.iso -vga virtio -display gtk,gl=on -full-screen -audiodev pa,id=speaker -machine pc,pcspk-audiodev=speaker
clean:
	rm -rf $(BUILD)
	rm -f khazar.iso
