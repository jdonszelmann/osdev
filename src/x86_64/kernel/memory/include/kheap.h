#ifndef KHEAP_H
#define KHEAP_H

#include <stdio.h>

#define KHEAP_START         (void*)0xC0400000
#define KHEAP_INITIAL_SIZE  48 * 1024 * 1024
#define KHEAP_MAX_ADDRESS   (void*)0xCFFFFFFF
#define HEAP_MIN_SIZE       4 * 1024 * 1024

#define PAGE_SIZE 4096
#define OVERHEAD (sizeof(Block_t) + sizeof(uint32_t))


typedef struct Block{
    uint32_t size;
    struct Block * prev;
    struct Block * next;
}Block_t;


Block_t * head;
Block_t * tail;
Block_t * freelist;

void * heap_start;
void * heap_end;
void * heap_curr;
void * heap_max;

bool kheap_enabled;

uint32_t kmalloc_int(uint32_t sz, bool align, uint32_t *phys);
void * kmalloc_cont(uint32_t sz, bool align, uint32_t *phys);
void * kmalloc_a(uint32_t sz);
uint32_t kmalloc_p(uint32_t sz, uint32_t *phys);
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys);
void * kmalloc(uint32_t sz);
void kfree(void *p);
void * krealloc(void * ptr, uint32_t size);
void print_kheap_map();
void print_kheap_map_serial();
bool doesItFit(Block_t * n, uint32_t size);
void setFree(uint32_t *size, bool x);
void removeNodeFromFreelist(Block_t * x);
void addNodeToFreelist(Block_t * x);
bool isBetter(Block_t * node1, Block_t * node2);
Block_t * bestfit(uint32_t size);
Block_t * getPrevBlock(Block_t * n);
Block_t * getNextBlock(Block_t * n);
uint32_t getRealSize(uint32_t size);
uint32_t getSbrkSize(uint32_t size);
bool isFree(Block_t * n);
bool isEnd(Block_t * n);
bool init_kheap();
void *malloc(uint32_t size);
void free(void * ptr);
void *kcalloc(uint32_t num, uint32_t size);
void *realloc(void *ptr, uint32_t size);

#endif