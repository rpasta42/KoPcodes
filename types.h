#ifndef TYPES_H_INCLUDE
#define TYPES_H_INCLUDE

#include "utils.h"

/***********LEXER*******/

//enum lex_type_t
TYPE(lex_type_t, enum) {
   LEX_SYM, //LEX_STR
   LEX_HEX, LEX_INT,
   LEX_REG,
   LEX_DOT_SYM
   //LEX_COMMA, LEX_SEMI,
   //LEX_OPEN_SQUARE, LEX_CLOSE_SQUARE
} END_TYPE(lex_type_t);

//struct lexeme_t
TYPE(lexeme_t, struct) {
   char* str;
   int len;
   //lex_type_t type;
   //int int_val;
} END_TYPE(lexeme_t);

TYPE(lexed_line_t, struct) {
   lexeme_t* lexemes;
   int num_lexemes;
} END_TYPE(lexed_line_t);

/*******END*LEXER*******/

/********PARSER*******/

static char* instr_names_str[] = {
   //"none",
   "section",
   "label",
   "add",
   "and",
   "call",
   "cmp",
   "dec",
   "div",
   "inc",
   "int",
   "ja",
   "jae",
   "jb",
   "jbe",
   "jcxz",
   "je",
   "jg",
   "jge",
   "jl",
   "jle",
   "jmp",
   "loop",
   "mov",
   "mul",
   "or",
   "pop",
   "push",
   "sub"
};

static char* reg_names_str[] = {
   "eax",
   "ebx",
   "ecx",
   "edx",
};

TYPE(reg_name_t, enum) {
   RegEax,
   RegEbx,
   RegEcx,
   RegEdx
} END_TYPE(reg_name_t);

//typedef enum instr_arg_types {} instr_arg_types;
TYPE(instr_arg_type_t, enum) {
   ArgNone,
   ArgSym,
   ArgReg8,
   ArgReg16,
   ArgReg32,
   ArgConst,
   ArgAddr
} END_TYPE(instr_arg_type_t);

TYPE(instr_name_t, enum) {
   //OpNone,
   OpSection,
   OpLabel,
   OpAdd,
   OpAnd,
   OpCall,
   OpCmp,
   OpDec,
   OpDiv,
   OpInc,
   OpInt,
   OpJa,
   OpJae,
   OpJb,
   OpJbe,
   OpJcxz,
   OpJe,
   OpJg,
   OpJge,
   OpJl,
   OpJle,
   OpJmp,
   OpLoop,
   OpMov,
   OpMul,
   OpOr,
   OpPop,
   OpPush,
   OpSub
} END_TYPE(instr_name_t);

TYPE(instr_arg_value_t, union) {
   reg_name_t reg_index; //-1 not a reg
   int const_num;
   uint32_t const_num_u;
   uint8_t const_num_b;
   char* sym_str;
} END_TYPE(instr_arg_value_t);

//typedef struct instr_t {} instr_t;
TYPE(instr_t, struct) {
   instr_name_t name; //from instruction enum
   //instr_name_sym
   char* sym; //some custom crap. //TODO: NEED TO FREE IF NOT NULL
   instr_arg_type_t arg1_t;
   instr_arg_type_t arg2_t;
   instr_arg_value_t arg1_v;
   instr_arg_value_t arg2_v;
} END_TYPE(instr_t);

#define NUM_INSTRUCTS OpSub + 1
#define NUM_REGS RegEdx + 1


/******END PARSER*****/

/********SYM TABLE*******/


/* Symbol Table
Flags
   FLAG_ADD_ADDR = add current address
   FLAG_ADD_TEXT_ENTRY_ADDR = add virtual address where text entry begins to val
*/

//TYPE(elf_sym_tab_flags_t)
#define ELF_SYM_TAB_FLAG_ADD_ADDR (1 << 8)
#define ELF_SYM_TAB_FLAG_SUB_ADDR (1 << 7)
#define ELF_SYM_TAB_FLAG_ADD_TEXT_ENTRY_ADDR (1 << 6)
#define ELF_SYM_TAB_FLAG_SUB_TEXT_ENTRY_ADDR (1 << 5)
#define ELF_SYM_TAB_FLAG_EXPR (1 << 4)
//END_TYPE(elf_sym_tab_flags_t);

TYPE(sym_table_entry_t, struct) {
   char* name;
   //size_t int_name;
   void* expr_val; //if symbol value is complex expression (ELF_SYM_TAB_FLAG_EXPR)
   uint32_t val;
   int flags; //elf_sym_tab_Flags_t

   //this is needed for dynamic stuff
   uint32_t* opcode_indices; //index in opcodes where the symbol appears
   uint16_t num_opcode_indices;
   uint16_T len_opcode_indices;
} END_TYPE(sym_table_entry_t);


TYPE(sym_table_t, struct) {
   sym_table_entry_t* entries;
   size_t num_entries;
   size_t len_entries;
} END_TYPE(sym_table_t);


/*****END SYM TABLE******/



#endif //TYPES_H_INCLUDE

