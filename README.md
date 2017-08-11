
## Status

kOPcodes can assemble and run this 32-bit x86 elf Linux program:

```asm
mov eax, 1
mov ebx, 0
int 0x80
```


ROADMAP
- [x] simple mov generation
- [x] simple interrupts



```c
/*
elf linking format:
linking view:
   ELF header
   Program header table (optional)
   Section 1
   Section 2
   Section header table

execution view:
   ELF header
   Program Header Table
   Segment 1
   Segment 2
   Section header table (optional)
*/
```


random thoughts
- what the heck, our sym table will take care of complex expressions too (mov ebx, [ebx + eax * 4])
