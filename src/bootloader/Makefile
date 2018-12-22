
#include paths
dirs = $(shell find $(bootloader_PATH)/src/$(arch) -type d -print)
includedirs :=  $(sort $(foreach dir, $(foreach dir1, $(dirs), $(shell dirname $(dir1))), $(wildcard $(dir)/include)))

#linker
linker_script := $(bootloader_PATH)/src/$(arch)/link.ld
LFLAGS := -nostdlib -z max-page-size=0x1000

#flags
CFLAGS= -m32 -fno-pie -fno-builtin -O2 -nostdinc -nostdlib -ffreestanding -g -Wall -Wextra \
               -Werror -I. -MMD -mno-red-zone -Wl,--build-id=none 
# Wunused-variable will be ignored!

#automatically include any header in dirs called include
CFLAGS += $(foreach dir, $(includedirs), -I./$(dir))

#assembly
ASMFLAGS = 
ASMFLAGS += $(foreach dir, $(includedirs), -I./$(dir))

NASMFLAGS = -felf32

#support for .S
assembly_source_files := $(foreach dir,$(dirs),$(wildcard $(dir)/*.S))
assembly_object_files := $(patsubst $(bootloader_PATH)/src/$(arch)/%.S, \
	$(bootloader_PATH)/build/$(arch)/%.o, $(assembly_source_files))

#support for .asm
nassembly_source_files := $(foreach dir,$(dirs),$(wildcard $(dir)/*.asm))
nassembly_object_files := $(patsubst $(bootloader_PATH)/src/$(arch)/%.asm, \
	$(bootloader_PATH)/build/$(arch)/%.o, $(nassembly_source_files))


c_source_files := $(foreach dir,$(dirs),$(wildcard $(dir)/*.c))
c_object_files := $(patsubst $(bootloader_PATH)/src/$(arch)/%.c, \
    $(bootloader_PATH)/build/$(arch)/%.o, $(c_source_files))


.PHONY: all clean

all: $(bootloader)


clean:
	@rm -r $(bootloader_PATH)/build



$(bootloader): $(assembly_object_files) $(c_object_files) $(nassembly_object_files) $(linker_script)
	@echo linking...
	@ld $(LFLAGS) -T $(linker_script) -o $(bootloader) $(assembly_object_files) $(nassembly_object_files) $(c_object_files)


# compile assembly files
$(bootloader_PATH)/build/$(arch)/%.o: $(bootloader_PATH)/src/$(arch)/%.S
	@mkdir -p $(shell dirname $@)
	@echo compiling $<
	@gcc -c $(ASMFLAGS) $< -o $@

# compile assembly files
$(bootloader_PATH)/build/$(arch)/%.o: $(bootloader_PATH)/src/$(arch)/%.asm
	@mkdir -p $(shell dirname $@)
	@echo compiling $<
	@nasm $(NASMFLAGS) $< -o $@

# compile c files
$(bootloader_PATH)/build/$(arch)/%.o: $(bootloader_PATH)/src/$(arch)/%.c
	@mkdir -p $(shell dirname $@)
	@echo compiling $<
	@gcc -c $(CFLAGS) $< -o $@