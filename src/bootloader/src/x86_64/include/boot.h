#ifndef BOOT_H
#define BOOT_H

#define bootaddr 0x7C00
#define bootaddr2 0x7C00
#define kerneladdr 0x1000

#define video_mem_start 0xB8000

#define PML4 0x100000 //paging tables; the one and only master
#define PDP 0x101000 //paging tables; the one and only PDP
#define PD_start 0x102000 //paging tables

//indices:
#define gdt_code_segment 0x08
#define gdt_data_segment 0x10
#define gdt_tss_segment 0x18 //NB: twice as big

//misc constants:
#define video_background 0x2a //green monochrome


#endif
