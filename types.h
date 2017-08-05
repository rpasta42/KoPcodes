#ifndef TYPES_H_INCLUDE
#define TYPES_H_INCLUDE

#include "utils.h"



TYPE(lex_type_t, enum) {
   LEX_SYM, //LEX_STR
   LEX_HEX, LEX_INT,
   LEX_COMMA, LEX_SEMI,
   LEX_OPEN_SQUARE, LEX_CLOSE_SQUARE
} END_TYPE(lex_type_t);

//typedef struct lexeme_t {} lexeme_t;
TYPE(lexeme_t, struct) {
   char* str;
   int len;
   lex_type_t type;
   int int_val;
} END_TYPE(lexeme_t);

TYPE(lexed_line_t, struct) {
   lexeme_t* lexemes;
   int num_lexemes;
} END_TYPE(lexed_line_t);



#endif //TYPES_H_INCLUDE

