#ifndef X86GEN_H_INCLUDE
#define X86GEN_H_INCLUDE


#define OP_INT_C 0xCD

#define OP_MOV_R32_R32 0xC089
#define OP_MOV_R32_C 0xB8

#define OP_EAX 0x0
#define OP_ECX 0x1
#define OP_EDX 0x2
#define OP_EBX 0x3
/*#define EBX_ (EAX_+3)
#define ECX_ (EBX_+3)
#define EDX_ (ECX_+3)*/



byte_t* assemble_str(char* str, int* ret_len,
                     sym_table_t* sym_table);
char* gen_op(instr_t* instructs, int num_instructs, int* ret_size,
             sym_table_t* sym_table);


int sym_table_add_ref(sym_table_t* st,
                      char* name,
                      int val,
                      int flags,
                      sym_complex_t* expr_val
                      byte_t* ref_loc, //we modify bytes in ref_loc
                      int ref_loc_len);



#endif //X86GEN_H_INCLUDE

