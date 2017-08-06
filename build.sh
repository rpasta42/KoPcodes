#!/bin/bash

rm a.out
gcc -g main.c tests.c utils.c lexer.c x86gen.c
./a.out test.asm #life2.asm
#gdb --args a.out test.asm #life2.asm
#valgrind ./a.out
