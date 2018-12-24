#include <kernel.h>



void irqmaster_handler(uint32_t intid){
	interrupt_exec_handler(intid+32);
}

//TODO clean up with macros

int64_t time = 0;
// timer
void irq0_handler(){
	irqmaster_handler(0);
	outportb(0x20, 0x20); //EOI
}

// kbd
void irq1_handler(){
	irqmaster_handler(1);
	printf("kbd");
	outportb(0x20, 0x20); //EOI
}
 
void irq2_handler(){
	irqmaster_handler(2);
	outportb(0x20, 0x20); //EOI
}
 
void irq3_handler(){
	irqmaster_handler(3);
	outportb(0x20, 0x20); //EOI
}
 
void irq4_handler(){
	irqmaster_handler(4);
	outportb(0x20, 0x20); //EOI
}
 
void irq5_handler(){
	irqmaster_handler(5);
	outportb(0x20, 0x20); //EOI
}
 
void irq6_handler(){
	irqmaster_handler(6);
	outportb(0x20, 0x20); //EOI
}
 
void irq7_handler(){
	irqmaster_handler(7);
	outportb(0x20, 0x20); //EOI
}
 
void irq8_handler(){
	irqmaster_handler(8);
	outportb(0xA0, 0x20);
	outportb(0x20, 0x20); //EOI          
}
 
void irq9_handler(){
	irqmaster_handler(9);
	outportb(0xA0, 0x20);
	outportb(0x20, 0x20); //EOI
}
 
void irq10_handler(){
	irqmaster_handler(10);
	outportb(0xA0, 0x20);
	outportb(0x20, 0x20); //EOI
}
 
void irq11_handler(){
	irqmaster_handler(11);
	outportb(0xA0, 0x20);
	outportb(0x20, 0x20); //EOI
}
 
void irq12_handler(){
	irqmaster_handler(12);
	outportb(0xA0, 0x20);
	outportb(0x20, 0x20); //EOI
}
 
void irq13_handler(){
	irqmaster_handler(13);
	outportb(0xA0, 0x20);
	outportb(0x20, 0x20); //EOI
}
 
void irq14_handler(){
	irqmaster_handler(14);
	outportb(0xA0, 0x20);
	outportb(0x20, 0x20); //EOI
}
 
void irq15_handler(){
	irqmaster_handler(15);
	outportb(0xA0, 0x20);
	outportb(0x20, 0x20); //EOI
}

