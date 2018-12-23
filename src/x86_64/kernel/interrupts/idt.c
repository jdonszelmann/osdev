
#include <kernel.h>

void idt_set_gate(uint8_t num,uint32_t base,uint16_t sel,uint8_t flags){
	idt[num].base_low = (base & 0xFFFF);
	idt[num].base_high = (base >> 16) & 0xFFFF;
	idt[num].sel = sel;
	idt[num].zero = 0; //duh
	idt[num].flags = flags | 0x60;
}

//TODO testing and return false if tests failed
bool init_idt(){
	idtpointer.limit = (sizeof(idt_entry_t) * 256) - 1;
	idtpointer.base = (uintptr_t)&idt;
	memset(&idt, 0, sizeof(idt_entry_t) * 256);

	idt_load();

	return true;
}