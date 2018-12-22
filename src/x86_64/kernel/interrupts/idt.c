
#include <interrupts.h>
#include <IO.h>


extern uint32_t load_idt();


void idt_set_entry(int index, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entry * this = &idt[index];
    
    this->offset_lower = base & 0xFFFF;
    this->offset_higher = (base >> 16) & 0xFFFF;
    
    this->zero = 0;
    
    this->selector = sel;
    
    this->type_attr = flags | 0x60;
}

void idt_init(void) {
	memset(idt, 0, sizeof(idt));

	
	//exceptions
	extern void e0();
	idt_set_entry(0, (uint32_t)e0, 0x08, 0x8E);
	extern void e1();
	idt_set_entry(1, (uint32_t)e1, 0x08, 0x8E);
	extern void e2();
	idt_set_entry(2, (uint32_t)e2, 0x08, 0x8E);
	extern void e3();
	idt_set_entry(3, (uint32_t)e3, 0x08, 0x8E);
	extern void e4();
	idt_set_entry(4, (uint32_t)e4, 0x08, 0x8E);
	extern void e5();
	idt_set_entry(5, (uint32_t)e5, 0x08, 0x8E);
	extern void e6();
	idt_set_entry(6, (uint32_t)e6, 0x08, 0x8E);
	extern void e7();
	idt_set_entry(7, (uint32_t)e7, 0x08, 0x8E);
	extern void e8();
	idt_set_entry(8, (uint32_t)e8, 0x08, 0x8E);
	extern void e9();
	idt_set_entry(9, (uint32_t)e9, 0x08, 0x8E);
	extern void e10();
	idt_set_entry(10, (uint32_t)e10, 0x08, 0x8E);
	extern void e11();
	idt_set_entry(11, (uint32_t)e11, 0x08, 0x8E);
	extern void e12();
	idt_set_entry(12, (uint32_t)e12, 0x08, 0x8E);
	extern void e13();
	idt_set_entry(13, (uint32_t)e13, 0x08, 0x8E);
	extern void e14();
	idt_set_entry(14, (uint32_t)e14, 0x08, 0x8E);
	extern void e15();
	idt_set_entry(15, (uint32_t)e15, 0x08, 0x8E);
	extern void e16();
	idt_set_entry(16, (uint32_t)e16, 0x08, 0x8E);
	extern void e17();
	idt_set_entry(17, (uint32_t)e17, 0x08, 0x8E);
	extern void e18();
	idt_set_entry(18, (uint32_t)e18, 0x08, 0x8E);
	extern void e19();
	idt_set_entry(19, (uint32_t)e19, 0x08, 0x8E);
	extern void e20();
	idt_set_entry(20, (uint32_t)e20, 0x08, 0x8E);
	extern void e21();
	idt_set_entry(21, (uint32_t)e21, 0x08, 0x8E);
	extern void e22();
	idt_set_entry(22, (uint32_t)e22, 0x08, 0x8E);
	extern void e23();
	idt_set_entry(23, (uint32_t)e23, 0x08, 0x8E);
	extern void e24();
	idt_set_entry(24, (uint32_t)e24, 0x08, 0x8E);
	extern void e25();
	idt_set_entry(25, (uint32_t)e25, 0x08, 0x8E);
	extern void e26();
	idt_set_entry(26, (uint32_t)e26, 0x08, 0x8E);
	extern void e27();
	idt_set_entry(27, (uint32_t)e27, 0x08, 0x8E);
	extern void e28();
	idt_set_entry(28, (uint32_t)e28, 0x08, 0x8E);
	extern void e29();
	idt_set_entry(29, (uint32_t)e29, 0x08, 0x8E);
	extern void e30();
	idt_set_entry(30, (uint32_t)e30, 0x08, 0x8E);
	extern void e31();
	idt_set_entry(31, (uint32_t)e31, 0x08, 0x8E);

	//interrupts
	extern void irq0();
	idt_set_entry(32, (uint32_t)irq0, 0x08, 0x8E);
	extern void irq1();
	idt_set_entry(33, (uint32_t)irq1, 0x08, 0x8E);
	extern void irq2();
	idt_set_entry(34, (uint32_t)irq2, 0x08, 0x8E);
	extern void irq3();
	idt_set_entry(35, (uint32_t)irq3, 0x08, 0x8E);
	extern void irq4();
	idt_set_entry(36, (uint32_t)irq4, 0x08, 0x8E);
	extern void irq5();
	idt_set_entry(37, (uint32_t)irq5, 0x08, 0x8E);
	extern void irq6();
	idt_set_entry(38, (uint32_t)irq6, 0x08, 0x8E);
	extern void irq7();
	idt_set_entry(39, (uint32_t)irq7, 0x08, 0x8E);
	extern void irq8();
	idt_set_entry(40, (uint32_t)irq8, 0x08, 0x8E);
	extern void irq9();
	idt_set_entry(41, (uint32_t)irq9, 0x08, 0x8E);
	extern void irq10();
	idt_set_entry(42, (uint32_t)irq10, 0x08, 0x8E);
	extern void irq11();
	idt_set_entry(43, (uint32_t)irq11, 0x08, 0x8E);
	extern void irq12();
	idt_set_entry(44, (uint32_t)irq12, 0x08, 0x8E);
	extern void irq13();
	idt_set_entry(45, (uint32_t)irq13, 0x08, 0x8E);
	extern void irq14();
	idt_set_entry(46, (uint32_t)irq14, 0x08, 0x8E);
	extern void irq15();
	idt_set_entry(47, (uint32_t)irq15, 0x08, 0x8E);


	idtdescriptor * descriptor = &(idtdescriptor){
		(uint16_t)(sizeof(idt)-1),
		(uint32_t)idt 
	};

	load_idt(descriptor);
 
	asm volatile ("sti");
}

bool initialize_interrupts(){
	idt_init();

	return true;
}

