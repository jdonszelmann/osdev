#ifndef PAGING_H
#define PAGING_H

// #include <kernel.h>
#include <stdio.h>
#include <interrupts.h>
#include <kheap.h>
#include <memory.h>

#define PAGE_SIZE 4096

// Alignment related macro
#define IS_ALIGN(addr) ((((uint32_t)(addr)) | 0xFFFFF000) == 0)
#define PAGE_ALIGN(addr) ((((uint32_t)(addr)) & 0xFFFFF000) + 0x1000)

// Defone some address calculation macro
#define PAGEDIR_INDEX(vaddr) (((uint32_t)vaddr) >> 22)
#define PAGETBL_INDEX(vaddr) ((((uint32_t)vaddr) >>12) & 0x3ff)
#define PAGEFRAME_INDEX(vaddr) (((uint32_t)vaddr) & 0xfff)

// Paging register manipulation macro
#define SET_PGBIT(cr0) (cr0 = cr0 | 0x80000000)
#define CLEAR_PSEBIT(cr4) (cr4 = cr4 & 0xffffffef)

// Err code interpretation
#define ERR_PRESENT     0x1
#define ERR_RW          0x2
#define ERR_USER        0x4
#define ERR_RESERVED    0x8
#define ERR_INST        0x10


typedef struct page_dir_entry {
    uint32_t present    : 1;
    uint32_t rw         : 1;
    uint32_t user       : 1;
    uint32_t w_through  : 1;
    uint32_t cache      : 1;
    uint32_t access     : 1;
    uint32_t reserved   : 1;
    uint32_t page_size  : 1;
    uint32_t global     : 1;
    uint32_t available  : 3;
    uint32_t frame      : 20;
}page_dir_entry_t;

typedef struct page_table_entry {
    uint32_t present    : 1;
    uint32_t rw         : 1;
    uint32_t user       : 1;
    uint32_t reserved   : 2;
    uint32_t accessed   : 1;
    uint32_t dirty      : 1;
    uint32_t reserved2  : 2;
    uint32_t available  : 3;
    uint32_t frame      : 20;
}page_table_entry_t;


typedef struct page_table{
    page_table_entry_t pages[1024];
} page_table_t;

typedef struct page_directory{
    // The actual page directory entries(note that the frame number it stores is physical address)
    page_dir_entry_t tables[1024];
    // We need a table that contains virtual address, so that we can actually get to the tables
    page_table_t * ref_tables[1024];
} page_directory_t;

//from boot.asm
page_directory_t * boot_page_directory;

page_directory_t * kpage_dir;
uint8_t * temp_mem;



void * virtual2phys(page_directory_t * dir, void * virtual_addr);
void * dumb_kmalloc(uint32_t size, bool align);
void allocate_region(page_directory_t * dir, uint32_t start_va, uint32_t end_va, bool iden_map, bool is_kernel, bool is_writable);
void allocate_page(page_directory_t * dir, uint32_t virtual_addr, uint32_t frame, bool is_kernel, bool is_writable);
void free_region(page_directory_t * dir, uint32_t start_va, uint32_t end_va, bool free);
void free_page(page_directory_t * dir, uint32_t virtual_addr, bool free);
bool init_paging();
void set_page_directory(page_directory_t * page_dir, uint32_t phys);
void enable_paging();
void * ksbrk(int size);
void copy_page_directory(page_directory_t * dst, page_directory_t * src);
page_table_t * copy_page_table(page_directory_t * src_page_dir, page_directory_t * dst_page_dir, uint32_t page_dir_idx, page_table_t * src);
bool page_fault_handler(registers_t * reg);

#endif