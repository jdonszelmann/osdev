
#include <stdio.h>
#include <kheap.h>
#include <paging.h>
#include <memory.h>
#include <debug.h>
#include <util.h>

//place the bitmap directly after the end of the kernel
uint8_t * bitmap = (uint8_t*)(&KERNEL_VIRT_END);

//initialize memory manager by creating an empty bitmap
bool init_memory(uint32_t mem_size) {
	total_blocks = mem_size / BLOCK_SIZE;
	
	//every block of size BLOCK_SIZE (normally 4kb) needs a space in the memory map. find out how many spaces are needed
	bitmap_size = total_blocks / BLOCKS_PER_BUCKET;
	//by default integer division rounds down. this detects if we actually need one more entry because of this rounding
	if(bitmap_size * BLOCKS_PER_BUCKET < total_blocks){
		bitmap_size++;
	}

	//clearning in multiple memset passes. this has improved stability slightly for some reason
	printf("clearing memory tables...\n");
	#if DEBUG
	// LOG("clearning memory tables");
	#endif

	uint32_t bytesperpass = 4096;

	uint32_t passes = bitmap_size/bytesperpass;
	int32_t rest = (int32_t)bitmap_size - (int32_t)(passes*bytesperpass);

	if(rest < 0){
		passes --;
		rest += bytesperpass;
	}

	uint32_t offset = 0;

	for(uint32_t i = 0; i < passes; i++){
		printf("clearing: 0x%x - 0x%x (%i)\n", bitmap+offset, bitmap+offset+bytesperpass, bytesperpass);
		#if DEBUG
		// LOG("clearing: 0x%x - 0x%x (%i)\n", bitmap+offset, bitmap+offset+bytesperpass, bytesperpass);
		#endif
		memset(bitmap + offset, 0, bytesperpass);
		offset += bytesperpass;
	}

	if(rest != 0){
		printf("clearing: 0x%x - 0x%x (%i)\n", bitmap+offset, bitmap+offset+rest, rest);
		#if DEBUG
		// LOG("clearing: 0x%x - 0x%x (%i)\n", bitmap+offset, bitmap+offset+rest, rest);
		#endif
		memset(bitmap + offset, 0, rest);
	}

	//save where the first memory block is for reference
	mem_start = (uint8_t*)BLOCK_ALIGN(((uint32_t)(bitmap + bitmap_size)));


	//test bitmap
	for(uint32_t i = 0; i < bitmap_size; i++) {
		if(bitmap[i] != 0) {
			#if DEBUG
			// LOG("BITMAP TEST FAILED");
			#endif

			return false;
		}
	}

	printf("memory: initialized %i MB split into %i blocks starting at 0x%x\n", mem_size/(1024*1024), total_blocks,mem_start);

	return true;
}

//find new free block
uint32_t allocate_block() {
	uint32_t free_block = find_first_free_block();
	SETBIT(free_block);
	return free_block;
}

//allow for reallocating
void free_block(uint32_t blk_num) {
	CLEARBIT(blk_num);
}

//linear search through the memory space to find a free block. TODO: some kind of binary search?
uint32_t find_first_free_block() {
	for(uint32_t i = 0; i < total_blocks; i++) {
		if(!ISSET(i)){
			return i;
		}
	}
	#if DEBUG
	LOG("no more free blocks left");
	#endif

	PANIC("no more free blocks left, memory full"); //TODO maybe not panic but let the page fault handler fix this with swapping
	return (uint32_t) -1;
}

