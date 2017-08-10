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



/*
enum instr_args {
   InstrNone,
   InstrConst, //0x80 43
   InstrAddr,
   InstrReg,
   InstrRegConst2, //mov eax, 0x04
   Instr

}

enum opcodes {
   mov_r16_const
};
*/


/******END PARSER*****/





#endif //TYPES_H_INCLUDE

