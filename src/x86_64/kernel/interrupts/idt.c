
#include <kernel.h>
#include <timer.h>

void remap_PIC()
{
	outportb(0x20, 0x11);
	outportb(0xA0, 0x11);
	outportb(0x21, 0x20);
	outportb(0xA1, 40);
	outportb(0x21, 0x04);
	outportb(0xA1, 0x02);
	outportb(0x21, 0x01);
	outportb(0xA1, 0x01);
	outportb(0x21, 0x0);
	outportb(0xA1, 0x0);

	printf("PIC remap OK\n");
}

void idt_set_entry(uint32_t intid, uint32_t handler)
{
	IDT[intid].offset_lowerbits = handler & 0xffff;
	IDT[intid].selector = 0x08; //KERNEL_CODE_SEGMENT_OFFSET
	IDT[intid].zero = 0;
	IDT[intid].type_attr = 0x8e; //INTERRUPT_GATE
	//(uint16_t)(((address) >> 16) & 0xFFFF)
	IDT[intid].offset_higherbits = (handler >> 16) & 0xffff;
}

void idt_print_entry(uint32_t num)
{
	if (num >= IDTSIZE)
	{
		PANIC("trying to print idt entry; interrupt id invalid");
	}

	idt_entry_t entry = IDT[num];

	printf("idt entry %i; ", num);
	printf("flags:[%b], ", entry.selector);
	printf("sel:[%b]\n", entry.type_attr);
	printf("handler location: 0x%x\n", entry.offset_lowerbits | entry.offset_higherbits << 16);
}

bool init_idt(void)
{

	uint32_t idt_ptr[2];

	idt_set_entry(32, (uint32_t)irq0);

	idt_set_entry(0, (uint32_t)exception0);
	idt_set_entry(1, (uint32_t)exception1);
	idt_set_entry(2, (uint32_t)exception2);
	idt_set_entry(3, (uint32_t)exception3);
	idt_set_entry(4, (uint32_t)exception4);
	idt_set_entry(5, (uint32_t)exception5);
	idt_set_entry(6, (uint32_t)exception6);
	idt_set_entry(7, (uint32_t)exception7);
	idt_set_entry(8, (uint32_t)exception8);
	idt_set_entry(9, (uint32_t)exception9);
	idt_set_entry(10, (uint32_t)exception10);
	idt_set_entry(11, (uint32_t)exception11);
	idt_set_entry(12, (uint32_t)exception12);
	idt_set_entry(13, (uint32_t)exception13);
	idt_set_entry(14, (uint32_t)exception14);
	idt_set_entry(15, (uint32_t)exception15);
	idt_set_entry(16, (uint32_t)exception16);
	idt_set_entry(17, (uint32_t)exception17);
	idt_set_entry(18, (uint32_t)exception18);
	idt_set_entry(19, (uint32_t)exception19);
	idt_set_entry(20, (uint32_t)exception20);
	idt_set_entry(21, (uint32_t)exception21);
	idt_set_entry(22, (uint32_t)exception22);
	idt_set_entry(23, (uint32_t)exception23);
	idt_set_entry(24, (uint32_t)exception24);
	idt_set_entry(25, (uint32_t)exception25);
	idt_set_entry(26, (uint32_t)exception26);
	idt_set_entry(27, (uint32_t)exception27);
	idt_set_entry(28, (uint32_t)exception28);
	idt_set_entry(29, (uint32_t)exception29);
	idt_set_entry(30, (uint32_t)exception30);
	idt_set_entry(31, (uint32_t)exception31);


	idt_set_entry(32, (uint32_t)irq0);
	init_timer(100);
	idt_set_entry(33, (uint32_t)irq1);
	idt_set_entry(34, (uint32_t)irq2);
	idt_set_entry(35, (uint32_t)irq3);
	idt_set_entry(36, (uint32_t)irq4);
	idt_set_entry(37, (uint32_t)irq5);
	idt_set_entry(38, (uint32_t)irq6);
	idt_set_entry(39, (uint32_t)irq7);
	idt_set_entry(40, (uint32_t)irq8);
	idt_set_entry(41, (uint32_t)irq9);
	idt_set_entry(42, (uint32_t)irq10);
	idt_set_entry(43, (uint32_t)irq11);
	idt_set_entry(44, (uint32_t)irq12);
	idt_set_entry(45, (uint32_t)irq13);
	idt_set_entry(46, (uint32_t)irq14);
	idt_set_entry(47, (uint32_t)irq15);

	/* fill the IDT descriptor */
	uint32_t idt_address = (uint32_t)IDT;
	idt_ptr[0] = (sizeof(idt_entry_t) * 286) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16;
	remap_PIC();

	load_idt(idt_ptr);
	asm("sti");

	return true;
}