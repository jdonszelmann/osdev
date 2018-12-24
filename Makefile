
version = 0.0.1
arch ?= x86_64

#names
kernel_fullname := kernel-$(arch).bin
kernel := build/$(kernel_fullname)
iso := build/kernel-$(version)-$(arch).iso

#include paths
dirs = $(shell find src/$(arch)/kernel -type d -print)
includedirs :=  $(sort $(foreach dir, $(foreach dir1, $(dirs), $(shell dirname $(dir1))), $(wildcard $(dir)/include)))

#linker
linker_script := src/$(arch)/kernel/link.ld
LFLAGS := -nostdlib -z max-page-size=0x1000	-m elf_i386

#flags
CFLAGS= -m32 -fno-pie -fno-builtin -nostdinc -nostdlib -ffreestanding -g -Wall -Wextra \
               -Werror -I. -MMD -mno-red-zone -Wl,--build-id=none 
# Wunused-variable will be ignored!

#automatically include any header in dirs called include
CFLAGS += $(foreach dir, $(includedirs), -I./$(dir))

#assembly
ASMFLAGS = -m32 -nostdlib
ASMFLAGS += $(foreach dir, $(includedirs), -I./$(dir))

NASMFLAGS = -felf32

#support for .S
assembly_source_files := $(foreach dir,$(dirs),$(wildcard $(dir)/*.S))
assembly_object_files := $(patsubst src/$(arch)/kernel/%.S, \
	build/$(arch)/kernel/%.o, $(assembly_source_files))

#support for .asm
nassembly_source_files := $(foreach dir,$(dirs),$(wildcard $(dir)/*.asm))
nassembly_object_files := $(patsubst src/$(arch)/kernel/%.asm, \
	build/$(arch)/kernel/%.o, $(nassembly_source_files))


c_source_files := $(foreach dir,$(dirs),$(wildcard $(dir)/*.c))
c_object_files := $(patsubst src/$(arch)/kernel/%.c, \
    build/$(arch)/kernel/%.o, $(c_source_files))
#qemu

qemumem := 1
qemuflags := -device isa-debug-exit,iobase=0xf4,iosize=0x04 -serial stdio

.PHONY: all clean run runrel iso

all: $(iso)

install:
	@sudo apt-get install qemu nasm xorriso grub2

clean:
	@rm -r build

run: $(iso)
	@echo starting emulator...
	qemu-system-x86_64 -cdrom $(iso) -m $(qemumem)G $(qemuflags)

run-wsl: $(iso)
	@echo starting emulator...
	qemu-system-x86_64 -cdrom $(iso) -m $(qemumem)G $(qemuflags) -display curses

$(iso): $(kernel)
	@./scripts/generate_iso.sh $(kernel) $(iso)


$(kernel): $(assembly_object_files) $(c_object_files) $(nassembly_object_files) $(linker_script)
	@echo linking...
	@ld $(LFLAGS) -T $(linker_script) -o $(kernel) $(assembly_object_files) $(nassembly_object_files) $(c_object_files)


# compile assembly files
build/$(arch)/kernel/%.o: src/$(arch)/kernel/%.S
	@mkdir -p $(shell dirname $@)
	@echo compiling $<
	@gcc -c $(ASMFLAGS) $< -o $@

# compile assembly files
build/$(arch)/kernel/%.o: src/$(arch)/kernel/%.asm
	@mkdir -p $(shell dirname $@)
	@echo compiling $<
	@nasm $(NASMFLAGS) $< -o $@

# compile c files
build/$(arch)/kernel/%.o: src/$(arch)/kernel/%.c
	@mkdir -p $(shell dirname $@)
	@echo compiling $<
	@gcc -c $(CFLAGS) $< -o $@