#include "types.h"
#include "utils.h"
#include "main.h"

extern char* reg_names_str[];
extern char* instr_names_str[];


//print instruction and assembled code side by side
//mem_index is current instruction start in mem
//op_len is current instruction length
void print_side_by_side(instr_t* instruct, char* mem, int mem_index, int op_len)
{
   //print_instruct
   printf("\n%s ", instr_names_str[instruct->name]);
   int j;
   j = 0;
   while (j++ < 2) {

      instr_arg_type_t arg_t;
      instr_arg_value_t arg_v;
      if (j == 1) {
         arg_t = instruct->arg1_t;
         arg_v = instruct->arg1_v;
      } else if (j == 2) {
         arg_t = instruct->arg2_t;
         arg_v = instruct->arg2_v;
      }

      switch (arg_t) {
         case ArgNone:
            printf("(none) ");
            break;
         case ArgReg32:
            printf("(reg)%s ", reg_names_str[arg_v.reg_index]);
            break;
         case ArgConst:
            printf("(num#)%i ", arg_v.const_num);
            break;
         case ArgSym:
            printf("(sym)%s ", arg_v.sym_str);
            break;
      }
   }
   //end print_instruct


   printf("\t\t");
   //printf("\nhex:0x%08x\n", *((uint32_t*)opcode));
   print_mem(mem, op_len);
   printf("\n");
}


