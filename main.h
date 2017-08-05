#ifndef MAIN_H_INCLUDE
#define MAIN_H_INCLUDE

#include "types.h"

lexed_line_t* lex(char* file, int *num_lines1);
instr_t* gen_instructions(lexed_line_t* lines, int num_lines);

void print_instructs(instr_t* instructs, int num_instructs);

#endif //MAIN_H_INCLUDE

