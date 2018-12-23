
#include <kernel.h>

#define IDTSIZE 256

idt_entry_t idt[IDTSIZE];
idt_pointer_t idtpointer;

void idt_set_entry(uint8_t num,uint32_t base,uint16_t sel,uint8_t flags){
	idt[num].base_low = (base & 0xFFFF);
	idt[num].base_high = (base >> 16) & 0xFFFF;
	idt[num].sel = sel;
	idt[num].zero = 0; //duh
	idt[num].flags = flags | 0x60;
}


#define INSTALL_ISR_ENTRY(n) \
	void MACROCAT(exception,n)(); \
	idt_set_entry(n, (uint32_t)MACROCAT(exception,n), 0x08, 0x8E)

#define INSTALL_IRQ_ENTRY(n) \
	void MACROCAT(irq,n)(); \
	idt_set_entry((n+32), (uint32_t)MACROCAT(irq,n), 0x08, 0x8E)

//remap interrupt handlers, see https://stackoverflow.com/questions/282983/setting-up-irq-mapping
//http://www.brokenthorn.com/Resources/OSDevPic.html
void irq_remap() {
	outportb(0x20, 0x11);
	outportb(0xA0, 0x11);
	outportb(0x21, 0x20);
	outportb(0xA1, 0x28);
	outportb(0x21, 0x04);
	outportb(0xA1, 0x02);
	outportb(0x21, 0x01);
	outportb(0xA1, 0x01);
	outportb(0x21, 0x0);
	outportb(0xA1, 0x0);

	printf("PIC OK\n");
}


void idt_print_entry(uint32_t num){
	if(num >= IDTSIZE){
		return;
	}

	idt_entry_t entry = idt[num];

	printf("idt entry %i; ",num);
	printf("flags:[%b], ",entry.flags);
	printf("sel:[%b]\n",entry.sel);
	printf("handler location: 0x%x\n",entry.base_low | entry.base_high << 16);

}


//TODO testing and return false if tests failed
bool init_idt(){

	idtpointer.limit = (sizeof(idt_entry_t) * 256) - 1;
	idtpointer.base = (uintptr_t)&idt;
	memset(&idt, 0, sizeof(idt_entry_t) * 256);

	idt_set_entry(0,(uint32_t)exception0,0x08,0x8E);
	idt_set_entry(1,(uint32_t)exception1,0x08,0x8E);
	idt_set_entry(2,(uint32_t)exception2,0x08,0x8E);
	idt_set_entry(3,(uint32_t)exception3,0x08,0x8E);
	idt_set_entry(4,(uint32_t)exception4,0x08,0x8E);
	idt_set_entry(5,(uint32_t)exception5,0x08,0x8E);
	idt_set_entry(6,(uint32_t)exception6,0x08,0x8E);
	idt_set_entry(7,(uint32_t)exception7,0x08,0x8E);
	idt_set_entry(8,(uint32_t)exception8,0x08,0x8E);
	idt_set_entry(9,(uint32_t)exception9,0x08,0x8E);
	idt_set_entry(10,(uint32_t)exception10,0x08,0x8E);
	idt_set_entry(11,(uint32_t)exception11,0x08,0x8E);
	idt_set_entry(12,(uint32_t)exception12,0x08,0x8E);
	idt_set_entry(13,(uint32_t)exception13,0x08,0x8E);
	idt_set_entry(14,(uint32_t)exception14,0x08,0x8E);
	idt_set_entry(15,(uint32_t)exception15,0x08,0x8E);
	idt_set_entry(16,(uint32_t)exception16,0x08,0x8E);
	idt_set_entry(17,(uint32_t)exception17,0x08,0x8E);
	idt_set_entry(18,(uint32_t)exception18,0x08,0x8E);
	idt_set_entry(19,(uint32_t)exception19,0x08,0x8E);
	idt_set_entry(20,(uint32_t)exception20,0x08,0x8E);
	idt_set_entry(21,(uint32_t)exception21,0x08,0x8E);
	idt_set_entry(22,(uint32_t)exception22,0x08,0x8E);
	idt_set_entry(23,(uint32_t)exception23,0x08,0x8E);
	idt_set_entry(24,(uint32_t)exception24,0x08,0x8E);
	idt_set_entry(25,(uint32_t)exception25,0x08,0x8E);
	idt_set_entry(26,(uint32_t)exception26,0x08,0x8E);
	idt_set_entry(27,(uint32_t)exception27,0x08,0x8E);
	idt_set_entry(28,(uint32_t)exception28,0x08,0x8E);
	idt_set_entry(29,(uint32_t)exception29,0x08,0x8E);
	idt_set_entry(30,(uint32_t)exception30,0x08,0x8E);
	idt_set_entry(31,(uint32_t)exception31,0x08,0x8E);
	idt_set_entry(32,(uint32_t)irq0,0x08,0x8E);
	idt_set_entry(33,(uint32_t)irq1,0x08,0x8E);
	idt_set_entry(34,(uint32_t)irq2,0x08,0x8E);
	idt_set_entry(35,(uint32_t)irq3,0x08,0x8E);
	idt_set_entry(36,(uint32_t)irq4,0x08,0x8E);
	idt_set_entry(37,(uint32_t)irq5,0x08,0x8E);
	idt_set_entry(38,(uint32_t)irq6,0x08,0x8E);
	idt_set_entry(39,(uint32_t)irq7,0x08,0x8E);
	idt_set_entry(40,(uint32_t)irq8,0x08,0x8E);
	idt_set_entry(41,(uint32_t)irq9,0x08,0x8E);
	idt_set_entry(42,(uint32_t)irq10,0x08,0x8E);
	idt_set_entry(43,(uint32_t)irq11,0x08,0x8E);
	idt_set_entry(44,(uint32_t)irq12,0x08,0x8E);
	idt_set_entry(45,(uint32_t)irq13,0x08,0x8E);
	idt_set_entry(46,(uint32_t)irq14,0x08,0x8E);
	idt_set_entry(47,(uint32_t)irq15,0x08,0x8E);

	// idt_print_entry(0);

	idt_load((uint32_t)&(idtpointer));

	irq_remap();
	

	asm volatile("sti"); 



	return true;
}