
;TODO compress with macros

%macro EXCEPTION_ERR 1
    global exception%1
    extern exception%1_handler
exception%1:
    cli
    ;error code already on stack for error exceptions
    push byte %1

    pusha

    mov ax, ds
    push eax

    mov ax, 0x10; load kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp

    call exception%1_handler

    pop esp

    pop ebx; restore original data segment
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa

    add esp, 0x08

    sti
    iret
%endmacro
 
%macro EXCEPTION 1
    global exception%1
    extern exception%1_handler
exception%1:
    cli
    push byte 0 ; error code is allways-0 for nonerror exceptions
    push byte %1

    pusha

    mov ax, ds
    push eax

    mov ax, 0x10; load kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp

    call exception%1_handler

    pop esp

    pop ebx; restore original data segment
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa

    add esp, 0x08

    sti
    iret
%endmacro

EXCEPTION 0
EXCEPTION 1
EXCEPTION 2
EXCEPTION 3
EXCEPTION 4
EXCEPTION 5
EXCEPTION 6
EXCEPTION 7
EXCEPTION_ERR 8
EXCEPTION 9
EXCEPTION_ERR 10
EXCEPTION_ERR 11
EXCEPTION_ERR 12
EXCEPTION_ERR 13
EXCEPTION_ERR 14
EXCEPTION 15
EXCEPTION 16
EXCEPTION 17
EXCEPTION 18
EXCEPTION 19
EXCEPTION 20
EXCEPTION 21
EXCEPTION 22
EXCEPTION 23
EXCEPTION 24
EXCEPTION 25
EXCEPTION 26
EXCEPTION 27
EXCEPTION 28
EXCEPTION 29
EXCEPTION 30
EXCEPTION 31