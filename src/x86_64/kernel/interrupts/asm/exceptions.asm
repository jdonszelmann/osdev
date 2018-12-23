
;TODO compress with macros

global exception0
global exception1
global exception2
global exception3
global exception4
global exception5
global exception6
global exception7
global exception8
global exception9
global exception10
global exception11
global exception12
global exception13
global exception14
global exception15
global exception16
global exception17
global exception18
global exception19
global exception20
global exception21
global exception22
global exception23
global exception24
global exception25
global exception26
global exception27
global exception28
global exception29
global exception30
global exception31
 
global load_idt
 
extern exception0_handler
extern exception1_handler
extern exception2_handler
extern exception3_handler
extern exception4_handler
extern exception5_handler
extern exception6_handler
extern exception7_handler
extern exception8_handler
extern exception9_handler
extern exception10_handler
extern exception11_handler
extern exception12_handler
extern exception13_handler
extern exception14_handler
extern exception15_handler
extern exception16_handler
extern exception17_handler
extern exception18_handler
extern exception19_handler
extern exception20_handler
extern exception21_handler
extern exception22_handler
extern exception23_handler
extern exception24_handler
extern exception25_handler
extern exception26_handler
extern exception27_handler
extern exception28_handler
extern exception29_handler
extern exception30_handler
extern exception31_handler
 
exception0:
  pusha
  call exception0_handler
  popa
  iret
 
exception1:
  pusha
  call exception1_handler
  popa
  iret
 
exception2:
  pusha
  call exception2_handler
  popa
  iret
 
exception3:
  pusha
  call exception3_handler
  popa
  iret
 
exception4:
  pusha
  call exception4_handler
  popa
  iret
 
exception5:
  pusha
  call exception5_handler
  popa
  iret
 
exception6:
  pusha
  call exception6_handler
  popa
  iret
 
exception7:
  pusha
  call exception7_handler
  popa
  iret
 
exception8:
  pusha
  call exception8_handler
  popa
  iret
 
exception9:
  pusha
  call exception9_handler
  popa
  iret
 
exception10:
  pusha
  call exception10_handler
  popa
  iret
 
exception11:
  pusha
  call exception11_handler
  popa
  iret
 
exception12:
  pusha
  call exception12_handler
  popa
  iret
 
exception13:
  pusha
  call exception13_handler
  popa
  iret
 
exception14:
  pusha
  call exception14_handler
  popa
  iret
 
exception15:
  pusha
  call exception15_handler
  popa
  iret

exception16:
  pusha
  call exception16_handler
  popa
  iret

exception17:
  pusha
  call exception17_handler
  popa
  iret

exception18:
  pusha
  call exception18_handler
  popa
  iret

exception19:
  pusha
  call exception19_handler
  popa
  iret

exception20:
  pusha
  call exception20_handler
  popa
  iret

exception21:
  pusha
  call exception21_handler
  popa
  iret

exception22:
  pusha
  call exception22_handler
  popa
  iret

exception23:
  pusha
  call exception24_handler
  popa
  iret

exception24:
  pusha
  call exception24_handler
  popa
  iret

exception25:
  pusha
  call exception25_handler
  popa
  iret

exception26:
  pusha
  call exception26_handler
  popa
  iret

exception27:
  pusha
  call exception27_handler
  popa
  iret

exception28:
  pusha
  call exception28_handler
  popa
  iret

exception29:
  pusha
  call exception29_handler
  popa
  iret

exception30:
  pusha
  call exception30_handler
  popa
  iret

exception31:
  pusha
  call exception31_handler
  popa
  iret

