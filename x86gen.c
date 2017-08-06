

#define OP_MOV_R32_R32 0xC089
#define OP_MOV_R32_C 0xB8

#define OP_EAX 0x0
#define OP_ECX 0x1
#define OP_EDX 0x2
#define OP_EBX 0x3
/*#define EBX_ (EAX_+3)
#define ECX_ (EBX_+3)
#define EDX_ (ECX_+3)*/

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


void print_mem(char* bytes, int num_bytes) {
   int i;
   for (i = 0; i < num_bytes; i++) {
      byte_t b = *(uint8_t*)(bytes+i);
      //printf("__%i__", i);
      if (b <= 0xF)
         printf("0");
      printf("%01X ", b);
   }
}

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


//mem_index contains where we should place next instruction
//op_gen_* functions increment it after adding
void op_gen_mov(instr_t* instruct, byte_t* mem, int* mem_index) {

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

   print_side_by_side(instruct, mem, mem_index, op_len);

   *mem_index += op_len;
}

//converts instr_t to x86 opcode
//ret_size is set to size of return pointer
char* gen_op(instr_t* instructs, int num_instructs, int* ret_size) {
   *ret_size = 5*num_instructs;
   char* op = malloc(*ret_size);
   memset(op, 0, *ret_size);

   //op_i stores current index in op
   int op_i = 0; //op index where current instruction in for loop is being written

   int i;

   for (i = 0; i < num_instructs; i++) {
      instr_t* instruct = &instructs[i];

      if (instruct->sym != NULL) {
         printf("sym instruction: %s", instruct->sym);
         continue;
      }
      switch (instruct->name) {
         case OpMov:
            op_gen_mov(instruct, op, &op_i);

         break;

         default:

         break;

      }
   }

   return op;


}



void run_asm(char* str) {
   int num_lines;
   lexed_line_t* lexed = lex(str, &num_lines);

   DEBUG("run_asm")


   instr_t* instructs = gen_instructions(lexed, num_lines);

   printf("\nerror if 0: %i\n", (uint64_t)instructs);
   if (instructs) {
      print_instructs(instructs, num_lines);
   }

   int opcode_len = NULL;
   char* opcode = gen_op(instructs, num_lines, &opcode_len);

   write_file("kopcode.test", opcode, opcode_len);

   return;
}



