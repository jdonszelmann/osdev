export version = 0.0.1
export arch ?= x86_64

#names
export bootloader_fullname := bootloader-$(arch)
export bootloader_PATH := src/bootloader
export bootloader := $(bootloader_PATH)/build/$(bootloader_fullname)

export kernel_fullname := kernel-$(arch)
export kernel_PATH := src/kernel
export kernel := $(kernel_PATH)/build/$(kernel_fullname)

.PHONY: bootloader all run

all: bootloader kernel

bootloader:|harddrive
	$(MAKE) -f $(bootloader_PATH)/Makefile
	dd if=$(bootloader) of=harddrive conv=notrunc

kernel:|harddrive
	$(MAKE) -f $(kernel_PATH)/Makefile
	dd if=$(kernel) of=harddrive bs=512 seek=17 conv=notrunc

harddrive:
	dd if=/dev/zero of=$@ count=512

run: all
	qemu-system-x86_64 harddrive --curses

debug:
	qemu-system-x86_64 -s -S harddrive --curses

clean:
	rm -f harddrive


