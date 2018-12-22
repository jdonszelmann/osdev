ENTRY(start)
OUTPUT_FORMAT(elf32-i386)

KERNEL_VIRT_ADDR = 0xC0100000;
KERNEL_PHYS_ADDR = 0x00100000;
KERNEL_BASE_ADDR = (KERNEL_VIRT_ADDR - KERNEL_PHYS_ADDR);

SECTIONS{

    . = KERNEL_VIRT_ADDR;

    KERNEL_VIRT_START = .;

    .text ALIGN (4K) : AT (ADDR (.text) - KERNEL_BASE_ADDR)
    {
        *(.multiboot)
        *(.text)
    }
    .rodata ALIGN (4K) : AT (ADDR (.rodata) - KERNEL_BASE_ADDR)
    {
        *(.rodata)
    }
    .data ALIGN (4K) : AT (ADDR (.data) - KERNEL_BASE_ADDR)
    {
        *(.data)
    }
    .bss ALIGN (4K) : AT (ADDR (.bss) - KERNEL_BASE_ADDR)
    {
        *(COMMON)
        *(.bss)
        *(.stack)
    }

    KERNEL_VIRT_END = .;
}