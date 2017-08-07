#ifndef MAIN_H_INCLUDE
#define MAIN_H_INCLUDE

#include "types.h"

lexed_line_t* lex(char* file, int *num_lines1);
instr_t* gen_instructions(lexed_line_t* lines, int num_lines);

void print_instructs(instr_t* instructs, int num_instructs);


//print instruction parse list and raw output memory
void print_side_by_side(instr_t* instruct, char* mem, int mem_index, int op_len);


#endif //MAIN_H_INCLUDE

