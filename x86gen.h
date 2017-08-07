#ifndef X86GEN_H_INCLUDE
#define X86GEN_H_INCLUDE

#define OP_MOV_R32_R32 0xC089
#define OP_MOV_R32_C 0xB8

#define OP_EAX 0x0
#define OP_ECX 0x1
#define OP_EDX 0x2
#define OP_EBX 0x3
/*#define EBX_ (EAX_+3)
#define ECX_ (EBX_+3)
#define EDX_ (ECX_+3)*/


byte_t* run_asm(char* str, int* ret_len);
char* gen_op(instr_t* instructs, int num_instructs, int* ret_size);


#endif //X86GEN_H_INCLUDE

