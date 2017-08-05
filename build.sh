#!/bin/bash

rm a.out
gcc -g main.c tests.c utils.c lexer.c
#./a.out life2.asm
#gdb --args a.out life2.asm
valgrind ./a.out
