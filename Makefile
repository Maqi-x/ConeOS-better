CC   ?= gcc
NASM ?= nasm
LD   ?= ld
QEMU ?= qemu-system-i386

SRC_DIR   := src/
BUILD_DIR ?= build/
ISO_DIR   ?= iso/

CFLAGS  := -Wall -Wextra -O2 -pipe -m32 -ffreestanding -fno-stack-protector \
		   -fno-stack-check -fno-lto -mno-80387 -mno-mmx -mno-sse -mno-sse2 \
		   -I$(SRC_DIR)

LDFLAGS := -m elf_i386 -nostdlib -static -T linker.ld

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRCS_C   := $(call rwildcard,$(SRC_DIR),*.c)
SRCS_ASM := $(call rwildcard,$(SRC_DIR),*.asm)

OBJS     := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS_C)) \
            $(patsubst $(SRC_DIR)/%.asm,$(BUILD_DIR)/%.o,$(SRCS_ASM))

.PHONY: all clean run iso
all: iso

$(BUILD_DIR)/boot/boot.o: src/boot/boot.asm
	@mkdir -p $(dir $@)
	$(NASM) -f elf32 $< -o $@

$(BUILD_DIR)/cpu/irq.o: src/cpu/irq.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(ISO_DIR)/boot/kernel.elf: $(OBJS)
	@mkdir -p $(ISO_DIR)/boot
	$(LD) $(LDFLAGS) $(OBJS) -o $@

iso: $(ISO_DIR)/boot/kernel.elf grub.cfg
	@mkdir -p $(ISO_DIR)/boot/grub
	cp grub.cfg $(ISO_DIR)/boot/grub/
	grub-mkrescue -o ConeOS.iso $(ISO_DIR)

run: iso
	$(QEMU) -m 2G -cdrom ConeOS.iso -boot d -serial stdio

clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR) ConeOS.iso
