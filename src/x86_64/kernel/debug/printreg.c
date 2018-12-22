
#include <debug.h>
#include <stdint.h>
#include <stdio.h>
#include <interrupts.h>



void print_registers(registers * r){
	printf("gs:%i,fs:%i,es:%i,ds:%i",r->fs,r->fs,r->es,r->ds);
	printf("edi:%i,esi:%i,ebp:%i,esp:%i,ebx:%i,edx:%i,ecx:%i,eax:%i",r->edi,r->esi,r->ebp,r->esp,r->ebx,r->edx,r->ecx,r->eax);
	printf("int_no:%i,err_code:%i",r->int_no,r->err_code);
	printf("eip:%i,cs:%i,eflags:%i,useresp:%i,ss:%i",r->eip,r->cs,r->eflags,r->useresp,r->ss);
}