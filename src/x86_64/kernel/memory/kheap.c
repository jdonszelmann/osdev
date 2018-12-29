#include <stdio.h>
#include <kheap.h>
#include <paging.h>
#include <memory.h>
#include <debug.h>
#include <util.h>
#include <serial.h>


Block_t * head = NULL;       // First memory block
Block_t * tail = NULL;	  // Last memory block
Block_t * freelist = NULL;	  // All the memory blocks that are freed

bool kheap_enabled = false;

uint32_t placement_address = (uint32_t)&KERNEL_VIRT_END;

void * kmalloc_cont(uint32_t sz, bool align, uint32_t *phys) {

	if (align == 1 && (placement_address & 0xFFFFF000) ){
		// Align the placement address;
		placement_address &= 0xFFFFF000;
		placement_address += 0x1000;
	}
	if (phys){
		*phys = placement_address;
	}
	uint32_t tmp = placement_address;
	placement_address += sz;
	return (void*)tmp;

}

//allocate memory block using the dynamic memory allocator
uint32_t kmalloc_int(uint32_t sz, bool align, uint32_t *phys)
{
	if (heap_start != NULL){
		if(align){
			sz = sz + 4096;
		}
		void * addr = malloc(sz);
		uint32_t align_addr = ((uint32_t)addr & 0xFFFFF000) + 0x1000;
		if (phys != 0){

			uint32_t t = (uint32_t)addr;
			if(align){
				t = align_addr;
			}
			*phys = (uint32_t)virtual2phys(kpage_dir, (void*)t);

		}

		if(align){
			return align_addr;
		}
		return (uint32_t)addr;
	}
	else{
		if (align == 1 && (placement_address & 0xFFFFF000)){
			// Align the address;
			placement_address &= 0xFFFFF000;
			placement_address += 0x1000;
		}
		if (phys){
			*phys = placement_address;
		}
		uint32_t tmp = placement_address;
		placement_address += sz;
		return tmp;
	}
}

//allocate page alligned
void * kmalloc_a(uint32_t sz){
	return (void *)kmalloc_int(sz, true, 0);
}

//allocate to a physical address
uint32_t kmalloc_p(uint32_t sz, uint32_t *phys){
	return kmalloc_int(sz, 0, phys);
}

//allocate to a physical, page alligned adderss
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys){
	return kmalloc_int(sz, 1, phys);
}

//normal allocate
void * kmalloc(uint32_t sz){
	return (void*)kmalloc_int(sz, false, 0);
}

//malloc and zero. slower!
void *kcalloc(uint32_t num, uint32_t size) {
	void * ptr = malloc(num * size);
	memset(ptr, 0, num*size);
	return ptr;
}

//realloc. just malloc and move. 
void * krealloc(void * ptr, uint32_t size) {
	return realloc(ptr, size);
}

//free memory
void kfree(void * ptr) {
	free(ptr);
}

//initialize heap
bool init_kheap() {
	heap_start = KHEAP_START;
	heap_end = KHEAP_START + KHEAP_INITIAL_SIZE;
	heap_max = KHEAP_MAX_ADDRESS;
	heap_curr = KHEAP_START;
	kheap_enabled = 1;
	return true;
}

uint32_t getRealSize(uint32_t size){
	return ((size >> 1) << 1);
}

//print a kheap map for debugging
void print_kheap_map() {
	if(!head) {
		//heap empty
		return;
	}
	uint32_t total = 0;
	uint32_t total_overhead = 0;
	Block_t * curr = head;
	while(1) {
		// print it
		char c = 'A'; //allocated
		if(isFree(curr)){
			c = 'F'; //free
		}
		uint32_t a = getRealSize(curr->size);
		printf("| %u bytes %c", a, c);
		
		total = total + getRealSize(curr->size);
		total_overhead = total_overhead + OVERHEAD;
		if(isEnd(curr)){
			break;
		}
		void * ptr = (void*)curr + OVERHEAD + getRealSize(curr->size);
		curr = ptr;
	}
	printf("\n");
	printf("total usable bytes: %d\n", total);
	printf("total overhead bytes: %d\n", total_overhead);
	printf("total bytes: %d\n", total + total_overhead);
	printf("freelist: ");

	Block_t * ite = freelist;
	while(ite) {
		printf("(%p)->", ite);
		ite = ite->next;
	}
	printf("\n");
	return;
}

//print a kheap map for debugging (serial)
void print_kheap_map_serial() {
	if(!head) {
		//heap empty
		return;
	}
	uint32_t total = 0;
	uint32_t total_overhead = 0;
	Block_t * curr = head;
	while(1) {
		// print it
		char c = 'A'; //allocated
		if(isFree(curr)){
			c = 'F'; //free
		}
		uint32_t a = getRealSize(curr->size);
		printf_serial("| %u bytes %c", a, c);
		
		total = total + getRealSize(curr->size);
		total_overhead = total_overhead + OVERHEAD;
		if(isEnd(curr)){
			break;
		}
		void * ptr = (void*)curr + OVERHEAD + getRealSize(curr->size);
		curr = ptr;
	}
	printf_serial("\n");
	printf_serial("total usable bytes: %d\n", total);
	printf_serial("total overhead bytes: %d\n", total_overhead);
	printf_serial("total bytes: %d\n", total + total_overhead);
	printf_serial("freelist: ");

	Block_t * ite = freelist;
	while(ite) {
		printf_serial("(%p)->", ite);
		ite = ite->next;
	}
	printf_serial("\n");
	return;
}


//returns true if this block is the last block
bool isEnd(Block_t * n) {
	return n == tail;
}

//returns true if the block fits in the size
bool doesItFit(Block_t * n, uint32_t size) {
	return n->size >= getRealSize(size) && isFree(n);
}

//mark a block as free
void setFree(uint32_t *size, bool x) {
	if(x) {
		*size = *size | 1;
		return;
	}
	*size = *size & 0xFFFFFFFE;
}

//return true if a block is free
bool isFree(Block_t * n) {

	if(!n){
		return 0;
	}
	return n->size & 0x1;
}

// remove a block from the list of free nodes
void removeNodeFromFreelist(Block_t * x) {
	if(!x){
		return;
	}
	if(x->prev) {
		x->prev->next = x->next;
		if(x->next){
			x->next->prev = x->prev;
		}
	}
	else {
		freelist = x->next;
		if(freelist){
			freelist->prev = NULL;
		}
	}
}

//add a block to the list of free nodes
void addNodeToFreelist(Block_t * x) {
	if(!x){
		return;
	}
	x->next = freelist;
	if(freelist){
		freelist->prev = x;
	}
	freelist = x;
	freelist->prev = NULL;
}

//go through free blocks. check which block fits best. TODO stpo looking if an exact fit is found. this is automatically the best
Block_t * bestfit(uint32_t size) {
	// extend this, may be, optimize for certain size of type
	if(!freelist){
		return NULL;
	}
	Block_t * curr = freelist;
	Block_t * currBest = NULL;
	while(curr) {
		if(doesItFit(curr, size)) {
			if(currBest == NULL || curr->size < currBest->size){
				currBest = curr;
			}
		}
		curr = curr ->next;
	}
	return currBest;
}

//get the block before this block. easy since its a DLL basically
Block_t * getPrevBlock(Block_t * n) {
	if(n == head){
		return NULL;
	}
	// get previous block size
	void * ptr = n;
	uint32_t * uptr = ptr - sizeof(uint32_t);
	uint32_t prev_size = getRealSize(*uptr);
	void * ret = ptr - OVERHEAD - prev_size;
	return ret;
}

//get the block after this block. easy again.
Block_t * getNextBlock(Block_t * n) {
	if(n == tail){
		return NULL;
	}
	void * ptr = n;
	ptr = ptr + OVERHEAD + getRealSize(n->size);
	return ptr;
}

//malloc!
void *malloc(uint32_t size) {
	if(size == 0){
		#if DEBUG
		LOG("tried to allocate 0 bytes");
		#endif
		return NULL;
	}
	// get the real size of this block, rounded to 16 bytes.
	uint32_t roundedSize = ((size + 15)/16) * 16; //is this correct?
	uint32_t blockSize = roundedSize + OVERHEAD;

	// printf("roundedsize %d\n",roundedSize);
	// printf("blockSize %d\n",blockSize);

	//find a block with the best fit fot this size    
	Block_t * best = bestfit(roundedSize);

	uint32_t * trailingSize = NULL;
	if(best) {
		// printf("best\n");

		//store te size in the last few bytes of the chunk. yes there is some overhead
		void * ptr = (void*)best;
		void * saveNextBlock = getNextBlock(best);
		uint32_t chunkSize = getRealSize(best->size) + OVERHEAD;
		uint32_t rest = chunkSize - blockSize; // what's left. the best fit might not be an exact fit
		uint32_t whichSize;
		
		if(rest < 8 + OVERHEAD){
			whichSize = chunkSize;
		}else{
			whichSize = blockSize;
		}
		best->size = whichSize - OVERHEAD;
		setFree(&(best->size), 0);
		void * base = ptr;
		trailingSize = ptr + whichSize - sizeof(uint32_t);
		*trailingSize = best->size;
		ptr = (void*)(trailingSize + 1);

		//if the resulting block after splitting is too small to be used (due to overhead) discard it
		if(rest < 8 + OVERHEAD){
			goto noSplit;
		}
		if(rest >= 8) {
			//if the next block is free, merge it with it
			if(base != tail && isFree(saveNextBlock)) {
				void * nextblock = saveNextBlock;
				Block_t * n_nextblock = nextblock;
				//remove next block since it will be merged with current and current will be put in the list
				removeNodeFromFreelist(n_nextblock);
				Block_t * t = ptr;
				t->size = rest - OVERHEAD + getRealSize(n_nextblock->size) + OVERHEAD;
				setFree(&(t->size), 1);
				ptr = ptr + sizeof(Block_t) + getRealSize(t->size);
				trailingSize = ptr;
				*trailingSize = t->size;

				if(nextblock == tail){
					tail = t;
					//test if this works
					//int reclaimSize = getRealSize(t->size) + OVERHEAD;
					//ksbrk(-reclaimSize);
					//goto noSplit;
				}
				//add merged node to list
				addNodeToFreelist(t);
			}
			else {
				//just put the leftover into the list to use
				Block_t * putThisBack = ptr;
				putThisBack->size = rest - OVERHEAD;
				setFree(&(putThisBack->size), 1);
				trailingSize = ptr + sizeof(Block_t) + getRealSize(putThisBack->size);
				*trailingSize = putThisBack->size;
				if(base == tail){
					tail = putThisBack;
					//test if this works again
					//int reclaimSize = getRealSize(putThisBack->size) + OVERHEAD;
					//ksbrk(-reclaimSize);
					//goto noSplit;
				}
				addNodeToFreelist(putThisBack);

			}
		}
		noSplit:
		//return the pointer to the block
		removeNodeFromFreelist(base);
		return base + sizeof(Block_t);
	}else {
		//no freed block fit this size. use sbrk to allocate more blocks

		uint32_t realsize = blockSize;
		Block_t * ret = ksbrk(realsize);
		ASSERT(ret != NULL, "Heap is running out of space\n");
		if(!head){
			head = ret;
		}
		void * ptr = ret;
		void * save = ret;
		tail = ptr;



		// after sbrk(), split the block into half [blockSize  | the rest], and put the rest into the tree.
		ret->size = blockSize - OVERHEAD;
		setFree(&(ret->size), 0);
		ptr = ptr + blockSize - sizeof(uint32_t);
		trailingSize = ptr;
		*trailingSize = ret->size;
		// now, return it!
		return save + sizeof(Block_t);
	}
	return NULL;
}

//free!
void free(void *ptr) {

	Block_t * curr = ptr - sizeof(Block_t);
	Block_t * prev = getPrevBlock(curr);
	Block_t * next = getNextBlock(curr);

	//if both prev and next are free, merge
	if(isFree(prev) && isFree(next)) {
		prev->size = getRealSize(prev->size) + 2*OVERHEAD + getRealSize(curr->size) + getRealSize(next->size);
		setFree(&(prev->size), 1);
		uint32_t * trailingSize = (void*)prev + sizeof(Block_t) + getRealSize(prev->size);
		*trailingSize = prev->size;
		// if next used to be tail, set prev = tail
		if(tail == next){
			tail = prev;
		}
		removeNodeFromFreelist(next);
	}	
	//if only prev is free, still merge it with it
	else if(isFree(prev)) {
		prev->size = getRealSize(prev->size) + OVERHEAD + getRealSize(curr->size);
		setFree(&(prev->size), 1);
		uint32_t * trailingSize = (void*)prev + sizeof(Block_t) + getRealSize(prev->size);
		*trailingSize = prev->size;
		if(tail == curr){
			tail = prev;
		}
	}
	//if only next is free, still merge it with it
	else if(isFree(next)) {
		curr->size = getRealSize(curr->size) + OVERHEAD + getRealSize(next->size);
		setFree(&(curr->size), 1);
		uint32_t * trailingSize = (void*)curr + sizeof(Block_t) + getRealSize(curr->size);
		*trailingSize = curr->size;
		if(tail == next){
			tail = curr;
		}
		removeNodeFromFreelist(next);
		addNodeToFreelist(curr);
	}
	//else just mark this block as free, dont merge with anything
	else {
		setFree(&(curr->size), 1);
		uint32_t * trailingSize = (void*)curr + sizeof(Block_t) + getRealSize(curr->size);
		*trailingSize = curr->size;
		addNodeToFreelist(curr);
	}
}

//realloc
void *realloc(void *ptr, uint32_t size) {
	uint32_t * trailingSize = NULL;
	//if the pointer is null, just allocate this size for you
	if(!ptr){
		return malloc(size);
	}
	if(size == 0 && ptr != NULL) {
		free(ptr);
		return NULL;
	}
	uint32_t roundedSize = ((size + 15)/16) * 16;//TODO is this rounding right again? see malloc
	uint32_t blockSize = roundedSize + OVERHEAD;
	Block_t * nextBlock, * prevBlock;
	// shrink or expand ?
	// shrink:
	// now, we would just return the same address, later we may split this block
	// expand:
	// first, try if the actual size of the memory block is enough to to hold the current size
	// second, if not, try if merging the next block works
	// third, if none of the above works, malloc another block, move all the data there, and then free the original block
	Block_t * nptr = ptr - sizeof(Block_t);
	nextBlock = getNextBlock(nptr);
	prevBlock = getPrevBlock(nptr);
	//you stupid. you havent changed the size at all.
	if(nptr->size == size){
		return ptr;
	}
	if(nptr->size < size) {
		//expand this block
		//first try if the next block is free. merge if it generates enough space. 
		if(tail != nptr && isFree(nextBlock) && (getRealSize(nptr->size) + OVERHEAD + getRealSize(nextBlock->size)) >= roundedSize) {
			removeNodeFromFreelist(nextBlock);
			nptr->size = getRealSize(nptr->size) + OVERHEAD + getRealSize(nextBlock->size);
			setFree(&(nptr->size), 0);
			trailingSize = (void*)nptr + sizeof(Block_t) + getRealSize(nptr->size);
			*trailingSize = nptr->size;
			if(tail == nextBlock) {
				//TODO reclaim like with malloc (still has to be tested)
				tail = nptr;
			}
			return nptr + 1;
		}
		//maybe the previous block has space. merge. this is a lot more expensive since data has to be moved
		else if(head != nptr && isFree(prevBlock) && (getRealSize(nptr->size) + OVERHEAD + getRealSize(prevBlock->size)) >= roundedSize) {
			uint32_t originalSize = getRealSize(nptr->size);
			removeNodeFromFreelist(prevBlock);
			prevBlock->size = originalSize + OVERHEAD + getRealSize(prevBlock->size);
			setFree(&(prevBlock->size), 0);
			trailingSize = (void*)prevBlock + sizeof(Block_t) + getRealSize(prevBlock->size);
			*trailingSize = prevBlock->size;
			if(tail == nptr) {
				tail = prevBlock;
			}
			//actually copy the data
			memcpy(prevBlock+1, ptr, originalSize);
			return prevBlock + 1;
		}
		//if nothing works, allocate a new area and copy everything over
		void * newplace = malloc(size);
		memcpy(newplace, ptr, getRealSize(nptr->size));
		//free old area
		free(ptr);
		return newplace;
	}
	else {
		//if the new size is smaller, just shrink this block

		uint32_t rest = getRealSize(nptr->size) + OVERHEAD - blockSize;
		if(rest < 8 + OVERHEAD){
			return ptr;
		}

		nptr->size = blockSize - OVERHEAD;
		setFree(&(nptr->size), 0);
		trailingSize = (void*)nptr + sizeof(Block_t) + getRealSize(nptr->size);
		*trailingSize = nptr->size;

		//test if this reclaiming works. see malloc and free too	
		// if(tail == nptr) {
		// 	ksbrk(-reclaimSize);
		// 	return ptr;
		// }

		Block_t * splitBlock = (void*)trailingSize + sizeof(uint32_t);

		if(nextBlock && isFree(nextBlock)) {
			splitBlock->size = rest + getRealSize(nextBlock->size);
			setFree(&(splitBlock->size), 1);
			trailingSize = (void*)splitBlock + sizeof(Block_t) + getRealSize(splitBlock->size);
			*trailingSize = splitBlock->size;

			removeNodeFromFreelist(nextBlock);

			//needed?
			if(tail == nextBlock) {
				tail = splitBlock;
			}
			addNodeToFreelist(splitBlock);

			return ptr;
		}

		splitBlock->size = rest - OVERHEAD;
		setFree(&(splitBlock->size), 1);
		trailingSize = (void*) splitBlock + sizeof(Block_t) + getRealSize(splitBlock->size);
		*trailingSize = splitBlock->size;
		addNodeToFreelist(splitBlock);

		return ptr;
	}
	return NULL;
}