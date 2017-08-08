#!/bin/bash

rm a.out
gcc -g main.c tests.c utils.c lexer.c x86gen.c elf.c helpers.c elf_info.c
./a.out test.asm #life2.asm
#gdb --args a.out test.asm #life2.asm
#valgrind ./a.out
