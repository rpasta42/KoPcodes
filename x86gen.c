#include "types.h"
#include "utils.h"
#include "main.h"
#include "x86gen.h"

extern char* reg_names_str[];
extern char* instr_names_str[];


//returns opcode from instr_arg_value_t
byte_t op_gen_reg(instr_arg_value_t* arg_v) {
   int ret = -1;
   switch (arg_v->reg_index) {
      case RegEax:
         ret = OP_EAX;
      break;

      case RegEbx:
         ret = OP_EBX;
      break;

      case RegEcx:
         ret = OP_ECX;
      break;

      case RegEdx:
         ret = OP_EDX;
      break;

      default:
         printf("bad register");
         exit(-1);
         //return -1;
         ret = -1;
      break;
   }
   return ret;
}


void op_gen_int(instr_t* instruct, byte_t* mem, int* mem_index,
                sym_table_entry_t* sym_table)
{
   instr_arg_type_t arg1_t = instruct->arg1_t;
   instr_arg_type_t arg2_t = instruct->arg2_t;

   instr_arg_value_t arg1_v = instruct->arg1_v;

   if (arg2_t != ArgNone || arg1_t != ArgConst) {
      //ERROR
      printf("\n!!!!error\n");
      return;
   }

   int op_len = 2;
   byte_t* opcode = mem + (*mem_index);

   opcode[0] = OP_INT_C;
   opcode[1] = (uint8_t)arg1_v.const_num_b;

   print_side_by_side(instruct, mem, *mem_index, op_len);

   *mem_index += op_len;

}

//mem_index contains where we should place next instruction
//op_gen_* functions increment it after adding
void op_gen_mov(instr_t* instruct, byte_t* mem, int* mem_index,
                sym_table_entry_t* sym_table)
{

   instr_arg_type_t arg1_t = instruct->arg1_t;
   instr_arg_type_t arg2_t = instruct->arg2_t;

   instr_arg_value_t arg1_v = instruct->arg1_v;
   instr_arg_value_t arg2_v = instruct->arg2_v;

   int i;
   int op_len = -1;
   byte_t* opcode = mem + (*mem_index); //char opcode[op_len];

   if (arg1_t == ArgReg32) {
      byte_t reg_op = op_gen_reg(&arg1_v);

      if (arg2_t == ArgConst || arg2_t == ArgSym) {
         op_len = 5;
         memset(opcode, 0, op_len);
         //*(uint32_t*)(opcode) = 0;
         //opcode[4] = 0;

         opcode[0] = OP_MOV_R32_C;
         opcode[0] |= reg_op;


         if (arg2_t == ArgConst) {
            *(uint32_t*)(opcode+1) = arg2_v.const_num;
         }
         else {
            //TODO: symbol table
         }

      }
      else if (arg2_t == ArgReg32) {
         op_len = 2;
         memset(opcode, 0, op_len);

         byte_t reg_op2 = op_gen_reg(&arg2_v);

         *(uint16_t*)opcode = OP_MOV_R32_R32;
         opcode[1] |= reg_op << 6;
         opcode[1] |= reg_op2 << 3;


         if (reg_op == OP_EBX) { //idk why but it's needed
            opcode[1] |= 3; //mem[1] |= 3; //(1 << 1) | (1 << 2);
         }

      }
      else {
         op_len = 0;
      }
   } else {
      op_len = 0;
   }

   print_side_by_side(instruct, mem, *mem_index, op_len);

   *mem_index += op_len;
}


void op_gen_label(instr_t* instruct, byte_t* mem, int* mem_index,
                  sym_table_t* sym_table)
{
   sym_table_t* sm = sym_-table;

   char* label_name = instruct->sym;
   LIST_ADD_ITEM_SPACE(sm->entries, sym_table_entry_t,
                       sm->num_entries, sm->len_entries);

   sym_table_entry_t* new_entry = &sm->entries[sm->num_entries++];
   new_entry->name = instruct->sym;
   new_entry->val = mem_index;
   new_entry->flags = ELF_SYM_TAB_FLAG_ADD_TEXT_ENTRY_ADDR;
   new_entry->expr_val = NULL;


   LIST_NEW(new_entry->opcode_indices, uint32_t,
            new_entry->num_opcode_indices,
            new_entry->len_opcode_indices);

}


void sym_table_init(sym_table_t* sym_table)
{
   LIST_NEW(sym_table->entries, sym_table_entry_t,
            sym_table->num_entries, sym_table->len_entries);
}

//converts instr_t to x86 opcode
//ret_size is set to size of return pointer
char* gen_op(instr_t* instructs, int num_instructs, int* ret_size,
             sym_table_t* sym_table)
{
   *ret_size = 5*num_instructs;
   char* op = malloc(*ret_size);
   memset(op, 0, *ret_size);

   //op_i stores current index in op
   int op_i = 0; //op index where current instruction in for loop is being written

   int i;

   //init_sym_table(sym_table);

   for (i = 0; i < num_instructs; i++) {
      instr_t* instruct = &instructs[i];

      if (instruct->sym != NULL) {
         printf("sym instruction: %s", instruct->sym);
         continue;
      }
      switch (instruct->name) {
         case OpMov:
            op_gen_mov(instruct, op, &op_i, sym_table);

         break;

         case OpInt:
            op_gen_int(instruct, op, &op_i, sym_table);

         break;

         case OpLabel:
            op_gen_label(instruct, op, *op_i, sym_table);
         break;


         default:

         break;

      }
   }

   return op;


}


//generates opcode, writes them to kopcode.test and returns the bytes
byte_t* assemble_str(char* str, int* ret_len)
{
   int num_lines;
   lexed_line_t* lexed = lex(str, &num_lines);

   DEBUG("run_asm")


   instr_t* instructs = gen_instructions(lexed, num_lines);

   printf("\nerror if 0: %lu\n", (uint64_t)instructs);
   if (instructs) {
      print_instructs(instructs, num_lines);
   }

   sym_table_t sym_table;
   sym_table_init(&sym_table);

   int opcode_len = 0;
   char* opcode = gen_op(instructs, num_lines, &opcode_len,
                         &sym_table);

   //write_file("kopcode.test", opcode, opcode_len);
#ifdef DEBUG_BIN_ASM
   write_file(DEBUG_BIN_ASM, opcode, opcode_len);
#endif

   *ret_len = opcode_len;
   return opcode;
}



