
#include <stdio.h>
#include <kheap.h>
#include <paging.h>
#include <memory.h>
#include <debug.h>
#include <interrupts.h>
#include <util.h>
#include <bootconstants.h>


// Where we want to place all the paging structure data
bool paging_enabled = false;



//convert a virtual address to a physical one.
//if the address is in the boot page dir just subtract 0xC00000000. else search through 
//the page tables until the address is found
void * virtual2phys(page_directory_t * dir, void * virtual_addr) {
	if(!paging_enabled) {
		return (void*)(virtual_addr - KERNEL_BASE_ADDR);
	}
	uint32_t page_dir_idx = PAGEDIR_INDEX(virtual_addr), page_tbl_idx = PAGETBL_INDEX(virtual_addr), page_frame_offset = PAGEFRAME_INDEX(virtual_addr);
	if(!dir->ref_tables[page_dir_idx]) {
		PANIC("this page directory entry does not exist");
		return NULL;
	}
	page_table_t * table = dir->ref_tables[page_dir_idx];
	if(!table->pages[page_tbl_idx].present) {
		PANIC("this page table entry does not exist");
		return NULL;
	}
	uint32_t t = table->pages[page_tbl_idx].frame;
	t = (t << 12) + page_frame_offset;
	return (void*)t;
}

//linear malloc. cant free. be careful, use as little as possible!
void * dumb_kmalloc(uint32_t size, bool align) {
	void * ret = temp_mem;
	//page-allign the regions
	if(align && !IS_ALIGN(ret)){
		ret = (void*)PAGE_ALIGN(ret);
	}
	temp_mem = temp_mem + size;
	return ret;
}

//allocate any page that's needed for the region to be allocated
void allocate_region(page_directory_t * dir, uint32_t start_va, uint32_t end_va, bool iden_map, bool is_kernel, bool is_writable) {
	uint32_t start = start_va & 0xfffff000;
	uint32_t end = end_va & 0xfffff000;
	while(start <= end) {
		if(iden_map){
			allocate_page(dir, start, start / PAGE_SIZE, is_kernel, is_writable);
		}else{
			allocate_page(dir, start, 0, is_kernel, is_writable);
		}
		start = start + PAGE_SIZE;
	}
}

//get a block from the memory manager, mark it used and set flags. 
//TODO security. PDE and PTE have user R/W permissions
void allocate_page(page_directory_t * dir, uint32_t virtual_addr, uint32_t frame, bool is_kernel, bool is_writable) {

	page_table_t * table = NULL;
	if(!dir) {
		PANIC("empty page directory"); //TODO soft panic?
		return;
	}

	//look up the index of this page's address in the page directory and it's page table
	uint32_t page_directory_index = PAGEDIR_INDEX(virtual_addr);
	uint32_t page_table_index = PAGETBL_INDEX(virtual_addr);

	//check if this page exists using the references left from ANCHOR1
	table = dir->ref_tables[page_directory_index];
	if(!table) {
		//before the kernel heap exists do a dumb malloc. (to get the kheap working ASAP!)
		if(!kheap_enabled){
			table = dumb_kmalloc(sizeof(page_table_t), 1);
		}else{
			//else do a normal alloc (page alligned of course)
			table = kmalloc_a(sizeof(page_table_t));
		}

		//zero out this page directory entry
		memset(table, 0, sizeof(page_table_t));

		//mallocs return virtual address. page table stores physical ones. convert
		uint32_t t = (uint32_t)virtual2phys(kpage_dir, table);
		dir->tables[page_directory_index].frame = t >> 12;
		dir->tables[page_directory_index].present = 1;
		dir->tables[page_directory_index].rw = (uint32_t)is_writable;
		dir->tables[page_directory_index].user = (uint32_t)(!is_kernel);
		dir->tables[page_directory_index].page_size = 0;
		
		//put a reference so we can later check if this table exists (see ANCHOR1)
		dir->ref_tables[page_directory_index] = table;
	}

	// now that the page is allocated, put it in the page table. if it exists... else allocate a new one
	if(!table->pages[page_table_index].present) {
		uint32_t t;
		//use the frame parameter for basically reallocating the same page twice.
		//this is useful for inter process shared memory
		if(frame){
			t = frame;
		}else{
			t = allocate_block();
		}

		table->pages[page_table_index].frame = t;
		table->pages[page_table_index].present = 1;
		table->pages[page_table_index].rw = 1;
		table->pages[page_table_index].user = 1;
	}
}

//free all pages in region. this is easy. just allign address to page and iterate
void free_region(page_directory_t * dir, uint32_t start_va, uint32_t end_va, bool free) {
	uint32_t start = start_va & 0xfffff000;
	uint32_t end = end_va & 0xfffff000;
	while(start <= end) {
		free_page(dir, start, free);
		start = start + PAGE_SIZE;
	}
}

/*
 * Find the corresponding page table entry, and set frame to 0
 * Also, clear corresponding bit in pmm bitmap
 * @parameter free:
 *      0: only clear the page table entry, don't actually free the frame
 *      1 : free the frame
 * */
void free_page(page_directory_t * dir, uint32_t virtual_addr, bool free) {
	if(dir == boot_page_directory){
		PANIC("trying to free boot page"); //todo soft panic?
		return;
	}
	//search page in directo
	uint32_t page_dir_idx = PAGEDIR_INDEX(virtual_addr), page_tbl_idx = PAGETBL_INDEX(virtual_addr);
	if(!dir->ref_tables[page_dir_idx]) {
		PANIC("trying to free non existant page"); //TODO soft panic?
		return;
	}
	page_table_t * table = dir->ref_tables[page_dir_idx];
	if(!table->pages[page_tbl_idx].present) {
		PANIC("trying to free page in non existant page directory"); //TODO soft panic?
		return;
	}

	if(free){
		free_block(table->pages[page_tbl_idx].frame);
	}
	//set it to non present and delete it's frame so it can be found for reallocation
	table->pages[page_tbl_idx].present = 0;
	table->pages[page_tbl_idx].frame = 0;
}

//reenable paging (enabled and disabled in boot.asm) 
bool init_paging() {

	//place paging data directly after the memory bitmap
	temp_mem = bitmap + bitmap_size;

	//allocate and zero out a page directory. this cannot be freed since it's using dumb malloc
	kpage_dir = dumb_kmalloc(sizeof(page_directory_t), 1);
	memset(kpage_dir, 0, sizeof(page_directory_t));

	printf("allocating kernel pages...\n");

	//map the first 4MB (formerly boot page directory) into this page directory
	uint32_t i = KERNEL_BASE_ADDR;
	while(i < KERNEL_BASE_ADDR + (4 * 1024 * 1024)) {
		allocate_page(kpage_dir, i, 0, 1, 1);
		i = i + PAGE_SIZE;
	}

	//map memory for the kernel heap
	i = KERNEL_BASE_ADDR + (4 * 1024 * 1024);
	while(i < KERNEL_BASE_ADDR + (4 * 1024 * 1024) + KHEAP_INITIAL_SIZE) {
		allocate_page(kpage_dir, i, 0, 1, 1);
		i = i + PAGE_SIZE;
	}

	interrupt_register_handler(14, page_fault_handler);

	// Load kernel directory
	set_page_directory(kpage_dir, 0);

	// Enable Paging
	enable_paging();
	// Identity map the first
	allocate_region(kpage_dir, 0x0, 0x10000, 1, 1, 1);

	return true;
}

//set the hardware page directory to this one
void set_page_directory(page_directory_t * page_dir, uint32_t phys) {
	uint32_t t;
	if(!phys){
		t = (uint32_t)virtual2phys(boot_page_directory, page_dir);
	}else{
		t = (uint32_t)page_dir;
	}
	asm volatile("mov %0, %%cr3" :: "r"(t));
}


//enable paging. this encludes disabling 4MB pages that were on for the boot page directory
void enable_paging() {
	uint32_t cr0;
	uint32_t cr4;

	asm volatile("mov %%cr4, %0" : "=r"(cr4));
	CLEAR_PSEBIT(cr4);
	asm volatile("mov %0, %%cr4" :: "r"(cr4));

	asm volatile("mov %%cr0, %0" : "=r"(cr0));
	SET_PGBIT(cr0);
	asm volatile("mov %0, %%cr0" :: "r"(cr0));

	paging_enabled = 1;
}

void * ksbrk(int size) {
	void * runner = NULL;
	void * new_boundary;
	void * old_heap_curr;
restart_sbrk:
	old_heap_curr = heap_curr;
	if(size == 0) {
		goto ret;
	}
	//if heap has to expand
	else if(size > 0) {
		new_boundary = heap_curr + (uint32_t)size;
		//if the new boundary is lower than the heap size, just return
		if(new_boundary <= heap_end){
			goto update_boundary;
		}
		//return null if the heap cant expand anymore
		else if(new_boundary > heap_max){
			return NULL;
		}
		//if the new boundary is larger than the current size, but less than the max: expand the heap
		//by allocating some new pages for it
		else if(new_boundary > heap_end) {
			runner = heap_end;
			while(runner < new_boundary) {
				allocate_page(kpage_dir, (uint32_t)runner, 0, 1, 1);
				runner = runner +  PAGE_SIZE;
			}
			//repeat until we are sure everything will fit.
			if(old_heap_curr != heap_curr) {
				goto restart_sbrk;
			}
			heap_end = runner;
			goto update_boundary;
		}
	}
	//if heap can shrink
	else if(size < 0){
		//first free as many pages as can be freed, and then update all boundaries so the heap can keep working
		new_boundary = (void*)((uint32_t)heap_curr - (uint32_t)abs(size));
		if(new_boundary < heap_start + HEAP_MIN_SIZE) {
			new_boundary = heap_start + HEAP_MIN_SIZE;
		}
		runner = heap_end - PAGE_SIZE;
		while(runner > new_boundary) {
			free_page(kpage_dir, (uint32_t)runner, 1);
			runner = runner - PAGE_SIZE;
		}
		heap_end = runner + PAGE_SIZE;
		goto update_boundary;
	}
update_boundary:
	heap_curr = new_boundary;
ret:
	return old_heap_curr;
}

//Copy a page directory
void copy_page_directory(page_directory_t * dst, page_directory_t * src) {
	for(uint32_t i = 0; i < 1024; i++) {
		if(kpage_dir->ref_tables[i] == src->ref_tables[i]) {
			// Link kernel pages
			dst->tables[i] = src->tables[i];
			dst->ref_tables[i] = src->ref_tables[i];
		}
		else {
			// For non-kernel pages, copy the pages (for example, when forking process, you don't want the parent process mess with child process's memory)
			dst->ref_tables[i] = copy_page_table(src, dst, i, src->ref_tables[i]);
			uint32_t phys = (uint32_t)virtual2phys(src, dst->ref_tables[i]);
			dst->tables[i].frame = phys >> 12;
			dst->tables[i].user = 1;
			dst->tables[i].rw = 1;
			dst->tables[i].present = 1;
		}
	}
}

//copy a page table
page_table_t * copy_page_table(page_directory_t * src_page_dir, page_directory_t * dst_page_dir, uint32_t page_dir_idx, page_table_t * src) {
	page_table_t * table = (page_table_t*)kmalloc_a(sizeof(page_table_t));
	for(int i = 0; i < 1024; i++) {
		if(!table->pages[i].frame){
			continue;
		}
		// Source frame's virtual address
		uint32_t src_virtual_address = (page_dir_idx << 22) | (i << 12) | (0);
		// Destination frame's virtual address
		uint32_t dst_virtual_address = src_virtual_address;
		// Temporary virtual address
		uint32_t tmp_virtual_address = 0;

		//create a new page table
		allocate_page(dst_page_dir, dst_virtual_address, 0, 0, 1);
		allocate_page(src_page_dir, tmp_virtual_address, (uint32_t)virtual2phys(dst_page_dir, (void*)dst_virtual_address), 0, 1);
		//set all flags from previous table
		if (src->pages[i].present) table->pages[i].present = 1;
		if (src->pages[i].rw)      table->pages[i].rw = 1;
		if (src->pages[i].user)    table->pages[i].user = 1;
		if (src->pages[i].accessed)table->pages[i].accessed = 1;
		if (src->pages[i].dirty)   table->pages[i].dirty = 1;
		//copy contents
		memcpy((void*)tmp_virtual_address, (void*)src_virtual_address, PAGE_SIZE);
		//free old page
		free_page(src_page_dir, tmp_virtual_address, 0);
	}
	return table;
}



bool page_fault_handler(registers_t * reg) {

	uint32_t faulting_addr;
	//poll cr2 for the address producing the problem
	asm volatile("mov %%cr2, %0" : "=r" (faulting_addr));

	uint32_t present = reg->err_code & ERR_PRESENT; //check if the page was present
	uint32_t rw = reg->err_code & ERR_RW; //check if the page was writable
	uint32_t user = reg->err_code & ERR_USER; //check if the page was a user page
	uint32_t reserved = reg->err_code & ERR_RESERVED; //reserved bits were overwritten
	uint32_t inst_fetch = reg->err_code & ERR_INST;  // error in instruction fetch from this page

	char res[200] = {'\0'};

	if(!present){
		strcat(res,"page wasn't present, ");
	}
	if(rw){
		strcat(res,"page is read only, ");
	}
	if(user){
		strcat(res,"page is read only, ");
	}
	if(reserved){
		strcat(res,"page is unreadable, ");
	}
	if(inst_fetch){
		strcat(res,"error in instruction fetch, ");
	}

	int length = strlen(res);
	if(length > 0){
		res[length-2] = '\0'; 
	}

	PANIC("a page fault occured at 0x%x because %s",faulting_addr,res);

	return true;
}