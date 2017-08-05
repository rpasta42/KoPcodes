#!/bin/bash

rm a.out
gcc main.c tests.c utils.c lexer.c
./a.out

