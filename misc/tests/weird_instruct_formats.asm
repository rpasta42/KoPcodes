
lea array, [esp + 0]
mov [ebx + eax * 4], 0x80
lea ecx, SS:[ebp + 8]

global idt_load ;export to C code
extern idtp ;from C code

idt_load:
	lidt [idtp]
	ret

cli

push byte 0

jmp fault_handler

mov [0x2000], eax
pusha
push ds
push es
push fs
mov ex, 10
iret

SECTION .bss
	resp 8192

msg: 	db 'Hello, world',0xa
len:	equ $ - msg


=================

db 


