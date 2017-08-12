#ifndef TYPES_H_INCLUDE
#define TYPES_H_INCLUDE

#include "utils.h"

/***********LEXER*******/

//enum lex_type_t
TYPE(lex_type_t, enum) {
   LEX_SYM, //LEX_STR
   LEX_HEX, LEX_INT,
   LEX_REG, //register
   LEX_DOT_SYM,
   LEX_COMMA, LEX_SEMI,
   LEX_OPEN_SQUARE, LEX_CLOSE_SQUARE,
   LEX_OP, //-, +, *
   LEX_STR,
   LEX_DOLLAR,
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
   "dd",
   "db",
   "equ",
   "resb",
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
   "sub",
   "nop"
};

static char* reg_names_str[] = {
   "eax",
   "ebx",
   "ecx",
   "edx",
   "ax", "bx", "cx", "dx",
   "al", "ah",
   "bl", "bh",
   "cl", "ch",
   "dl", "dh"
};

TYPE(reg_name_t, enum) {
   RegEax,
   RegEbx,
   RegEcx,
   RegEdx,
   RegAx, RegBx, RegCx, RegDx,
   RegAl, RegAh,
   RegBl, RegBh,
   RegCl, RegCh,
   RegDl, RegDh
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
   OpDd,
   OpDb,
   OpEqu,
   OpResb,
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
   OpSub,
   OpNop
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

#define NUM_INSTRUCTS OpNop + 1
#define NUM_REGS RegDh + 1


/******END PARSER*****/

/********SYM TABLE*******/


/* Symbol Table
Flags
   FLAG_ADD_ADDR = add current address
   FLAG_ADD_BIN_ADDR = add virtual address where opcodes begin to val
*/

//TYPE(sym_tab_flags_t)
#define SYM_TAB_FLAG_ADD_ADDR (1 << 8)
#define SYM_TAB_FLAG_SUB_ADDR (1 << 7)
#define SYM_TAB_FLAG_ADD_BIN_ADDR (1 << 6)
#define SYM_TAB_FLAG_SUB_BIN_ADDR (1 << 5)
#define SYM_TAB_FLAG_EXPR (1 << 4)
#define SYM_TAB_FLAG_PURE_SYM (1 << 3) //reference; replace with symtable val
//END_TYPE(sym_tab_flags_t);


/* TODO: SYM TABLE SHOULD BE IN A BALANCED SORTED TREE */

TYPE(sym_complex_t, struct) {
   int is_reserve; //dd, db, equ, resp
   int is_square_brackets;
} END_TYPE(sym_complex_t);

TYPE(sym_table_entry_t, struct) {
   char* name;
   int int_name;
   //not null if flag ELF_SYM_TAB_FLAG_EXPR
   sym_complex_t* expr_val;
   int flags; //elf_sym_tab_flags_t
   uint32_t val;

   /*//this is needed for dynamic stuff
   uint32_t* opcode_indices; //index in opcodes where the symbol appears
   uint16_t num_opcode_indices;
   uint16_t len_opcode_indices;*/
} END_TYPE(sym_table_entry_t);

TYPE(sym_table_reference_t, struct) {
   int int_name; //symbol name
   int sym_index; //symbol index
   int flags; //elf_sym_tab_flags_t
   //not null if flag ELF_SYM_TAB_FLAG_EXPR
   sym_complex_t* expr_val;
} END_TYPE(sym_table_reference_t);

TYPE(sym_table_t, struct) {
   //ONE ARRAY STORES ALL NAMES INSTEAD OF char**
   //all MEMCPY'd in 1. THIS WAY IS GOOD FOR ELF
   char* sym_names;
   int num_name_chars;
   int len_names;

   sym_table_entry_t* entries;
   size_t num_entries;
   size_t len_entries;

   sym_table_reference_t* refs;
   int num_refs;
   int len_refs;

} END_TYPE(sym_table_t);


/*****END SYM TABLE******/


TYPE(asm_seg_info_t, struct) {
   uint32_t offset;
   char* name;
} END_TYPE(asm_seg_info_t);


TYPE(asm_op_data_t, struct) {
   instr_t* instruct;
   byte_t* mem;
   int* mem_index;
   sym_table_t* sym_table;

   int num_segs;
   int len_segs;
   asm_seg_info_t* segs;

} END_TYPE(asm_op_data_t);



TYPE(stage_line_t, struct) {
   int num_lexemes; //either
   int* lexeme_old_indices;
   int* lexeme_new_indices;

   int old_line_num;
   int new_line_num;
} END_TYPE(stage_line_t);

TYPE(stage_t, struct) {
   char* src;
   size_t src_len;

   int len_lines;
   int num_lines;
   char** lines;
   int* line_indices;
   //int*
} END_TYPE(stage_t);


TYPE(preproc_conf_t, struct) {

} END_TYPE(preproc_conf_t);

inline void preprocessor(stage_t* in) {
   stage_t* out;

}

TYPE(lexed_file_t, struct) {
   char* str_data;
   int str_len;

   lexed_line_t* lines;
   int num_lines;
   int len_lines;

} END_TYPE(lexed_file_t);



#endif //TYPES_H_INCLUDE

