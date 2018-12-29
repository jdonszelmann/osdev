
#ifndef PMM_H
#define PMM_H
#include <kernel.h>

#define BLOCK_SIZE 4096
#define BLOCKS_PER_BUCKET 8

// easy macros to clean up some code
#define SETBIT(i) bitmap[i / BLOCKS_PER_BUCKET] = bitmap[i / BLOCKS_PER_BUCKET] | (1 << (i % BLOCKS_PER_BUCKET))
#define CLEARBIT(i) bitmap[i / BLOCKS_PER_BUCKET] = bitmap[i / BLOCKS_PER_BUCKET] & (~(1 << (i % BLOCKS_PER_BUCKET)))
#define ISSET(i) ((bitmap[i / BLOCKS_PER_BUCKET] >> (i % BLOCKS_PER_BUCKET)) & 0x1)
#define GET_BUCKET32(i) (*((uint32_t*) &bitmap[i / 32]))

#define BLOCK_ALIGN(addr) (((addr) & 0xFFFFF000) + 0x1000)

//from link.ld
uint32_t KERNEL_VIRT_END;

uint8_t * bitmap;
uint8_t * mem_start;
uint32_t total_blocks;
uint32_t bitmap_size;

bool init_memory(uint32_t mem_size);
uint32_t allocate_block();
void free_block(uint32_t blk_num);
uint32_t find_first_free_block();

#endif