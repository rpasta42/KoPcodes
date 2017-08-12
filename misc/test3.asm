SECTION .data
   msg_ha db 'Successfully  opened', 0xa
   msg_ha_len equ $ - msg_ha

   timeval:
	tv_sec dd 0
	tv_usec dd 0

   ;buffer resp 1000
   msg: 	db 'Hello, world',0xa
   len:	equ $ - msg


SECTION .bss
   ;resp 8192
   life_buffer resb 1000
   life_buffer2 resb 1000




SECTION .text

global _start
   xor eax, eax
   xor ebx, ebx
   jmp ha
   ret

;make fname=kopcodes/opcodes2
mov eax, 1
ha2:
mov ebx, ha2
ha:
mov eax, ebx
cmp eax, 4
je ha

cmp eax, 4
jg ha2
mov eax, [edx]
lea ecx, [eax + 4]
mov ebx, [ebx + eax * 4]
lea ebx, [ebx*8+ebx]
mov [0x2000], eax
