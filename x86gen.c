#include <assert.h>
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
         else if (arg2_t == ArgSym) {
            int ret_stat = sym_table_add_ref(sym_table,
                                             arg2_v.sym_str,
                                             0,
                                             SYM_TAB_FLAG_PURE_SYM,
                                             NULL,
                                             (opcode+1),
                                             4);
            if (ret_stat != 0) {
               goto err;
            }

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

   err:
   printf("\n:symbol not found? (op_gen_mov)..;.\n");

}


void op_gen_label(instr_t* instruct, byte_t* mem, int* mem_index,
                  sym_table_t* sym_table)
{
   sym_table_t* sm = sym_table;

   char* label_name = instruct->sym;
   /*LIST_ADD_ITEM_SPACE(sm->entries, sym_table_entry_t,
                       sm->num_entries, sm->len_entries);

   sym_table_entry_t* e = &sm->entries[sm->num_entries++];
   e->name = instruct->sym;
   e->val = *mem_index;
   e->flags = ELF_SYM_TAB_FLAG_ADD_BIN_ADDR;
   e->expr_val = NULL;


   LIST_NEW(e->opcode_indices, uint32_t,
            e->num_opcode_indices, e->len_opcode_indices);*/
   int ret_val =
      sym_table_add_sym(sym_table,
                        instruct->sym,
                        *mem_index, //value
                        SYM_TAB_FLAG_ADD_BIN_ADDR,
                        NULL);

}

void op_gen_nop(instr_t* instruct, byte_t* mem, int* mem_index,
                  sym_table_t* sym_table)
{
   byte_t* opcode = mem + (*mem_index); //char opcode[op_len];
   opcode[0] = 0x90;
   *mem_index += 1;
}

void op_gen_sect(asm_op_data_t* data) {
   LIST_ADD_ITEM_SPACE(data->segs, asm_seg_info_t,
                       data->num_segs, data->len_segs);

   asm_seg_info_t* seg = &data->segs[data->num_segs++];
   seg->offset = *data->mem_index;

   instr_t* inst = data->instruct;

   assert(inst->arg1_t == ArgSym);
   seg->name = inst->arg1_v.sym_str;
}


void op_gen_sym(asm_op_data_t* data) {
   instr_t* instruct = data->instruct;
   //instruct->sym
}


void init_asm_op_data(asm_op_data_t* data,
                      instr_t* instruct,
                      byte_t* mem,
                      int* mem_index,
                      sym_table_t* sym_tab)
{

   data->instruct = instruct;
   data->mem = mem;
   data->mem_index = mem_index;
   data->sym_table = sym_tab;

   LIST_NEW(data->segs, asm_seg_info_t,
            data->num_segs, data->len_segs);

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

   asm_op_data_t data;
   init_asm_op_data(&data, NULL,
                   op, &op_i,
                   sym_table);

   //init_sym_table(sym_table);

   for (i = 0; i < num_instructs; i++) {
      instr_t* instruct = &instructs[i];

      data.instruct = instruct;

      if (instruct->sym != NULL) {
         printf("sym instruction: %s", instruct->sym);
         op_gen_sym(&data);
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
            op_gen_label(instruct, op, &op_i, sym_table);
         break;

         case OpNop:
            op_gen_nop(instruct, op, &op_i, sym_table);
         break;

         case OpSection:
            op_gen_sect(&data);

         default:
            printf("\nunknown opcode\n");
         break;

      }
   }

   return op;


}


//generates opcode, writes them to kopcode.test and returns the bytes
byte_t* assemble_str(char* str, int* ret_len, sym_table_t* sym_table)
{
   int num_lines;
   lexed_line_t* lexed = lex(str, &num_lines);

   DEBUG("run_asm")


   instr_t* instructs = gen_instructions(lexed, num_lines);

   printf("\nerror if 0: %lu\n", (uint64_t)instructs);
   if (instructs) {
      print_instructs(instructs, num_lines);
   }

   int opcode_len = 0;
   char* opcode = gen_op(instructs, num_lines, &opcode_len,
                         sym_table);

   //write_file("kopcode.test", opcode, opcode_len);
#ifdef DEBUG_BIN_ASM
   write_file(DEBUG_BIN_ASM, opcode, opcode_len);
#endif

   *ret_len = opcode_len;
   return opcode;
}


//generates opcode, writes them to kopcode.test and returns the bytes
byte_t* assemble_str2(char* str, int str_len,
                      int* ret_len, sym_table_t* sym_table)
{
   int num_lines;

   lexed_file_t f;
   f.str_data = str;
   f.str_len = str_len;

   lex2(f); //sets f->lines

   DEBUG("run_asm")

/*
   instr_t* instructs = gen_instructions(lexed, num_lines);

   printf("\nerror if 0: %lu\n", (uint64_t)instructs);
   if (instructs) {
      print_instructs(instructs, num_lines);
   }

   int opcode_len = 0;
   char* opcode = gen_op(instructs, num_lines, &opcode_len,
                         sym_table);

   //write_file("kopcode.test", opcode, opcode_len);
#ifdef DEBUG_BIN_ASM
   write_file(DEBUG_BIN_ASM, opcode, opcode_len);
#endif

   *ret_len = opcode_len;
   return opcode;*/
   return NULL;
}




/* SYMBOL TABLE FUNCTIONS */

void sym_table_init(sym_table_t* st)
{
   LIST_NEW(st->entries, sym_table_entry_t,
            st->num_entries, st->len_entries);

   LIST_NEW(st->refs, sym_table_reference_t,
            st->num_refs, st->len_refs);

   LIST_NEW(st->sym_names, char,
            st->num_name_chars, st->len_names);
}

//TODO: error (non-zero) value if it already exists
int sym_table_add_sym(sym_table_t* st,
                       char* name,
                       int val,
                       int flags,
                       sym_complex_t* expr_val)
{
   int name_len = strlen(name) + 1;

   LIST_ADD_ITEM_SPACE(st->entries, sym_table_entry_t,
                       st->num_entries, st->len_entries);

   LIST_ADD_ITEM_SPACE(st->sym_names, char,
                       st->num_name_chars + name_len,
                       st->len_entries);

   char* new_name_loc = st->sym_names + st->num_name_chars;

   memcpy(new_name_loc,
          name, name_len);

   st->num_name_chars += name_len;

   sym_table_entry_t* e = &st->entries[st->num_entries++];
   e->name = new_name_loc;
   e->val = val;
   e->flags = flags;
   e->expr_val = expr_val;

   return 0;
}


int sym_table_add_ref(sym_table_t* st,
                      char* name,
                      int val,
                      int flags,
                      sym_complex_t* expr_val,
                      byte_t* ref_loc, //we modify bytes in ref_loc
                      int ref_loc_len)
{

   sym_table_entry_t* e = NULL;
   bool found_sym = false;
   int i;
   for (i = 0; i < st->num_entries; i++) {
      e = &st->entries[i];
      if (0 == strncmp(e->name, name, strlen(e->name))) {
         found_sym = true;
         break;
      }
   }

   if (!found_sym)
      goto err;

   LIST_ADD_ITEM_SPACE(st->refs, sym_table_reference_t,
                       st->num_refs, st->len_refs);
   sym_table_reference_t* r = &st->refs[st->num_refs++];

   r->int_name = e->int_name;
   r->sym_index = i;
   r->flags = flags;

   r->expr_val = expr_val;

   if (ref_loc_len == 1) {
      ref_loc[0] = (byte_t)val;
   } else if (ref_loc_len == 2) {
      *((uint16_t*)ref_loc) = (byte_t)val;
   } else if (ref_loc_len == 4) {
      *((uint32_t*)ref_loc) = val;
   }

   return 0;

   err:
   printf("\n=========ERROR\n");
   return -1;
}

/* END SYMBOL TABLE FUNCTIONS */


