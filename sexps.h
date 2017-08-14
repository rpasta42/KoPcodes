#ifndef SEXPS_H_INCLUDE
#define SEXPS_H_INCLUDE

#include "utils.h"
#include "types.h"

#define S_NULL NULL

//TYPE(S_ATOM_TYPE_T, enum)
#define S_ATOM_CHAR -1
#define S_ATOM_STR -2
#define S_ATOM_INT32 -3
#define S_ATOM_INT64 -4
#define S_ATOM_CONS -5
//END_TYPE(S_ATOM_TYPE_T);

TYPE(sexps_str_t, struct) {
   size_t len;
   char* s;
} END_TYPE(sexps_str_t);

TYPE(sexps_t, struct) {

   //union n.
   //cons num_sub_exps = 2, len_sub_exps = 0. vector list: len_sub_exps > 0
   union { //if (n > 0) { num_sub_exp } else if (n < 0) { s_auto_type_t } else if (n = 0) { error }
      int num_sub_exps; //num_sub_exp < 0 = atom type (S_ATOM_TYPE_T)
      int atom_type;
   };

   union {
      struct {
         uint32_t atom_val32;
         uint64_t atom_val64;
      };

      struct {
         uint32_t len_sub_exps; //0 == cons pair, else list
         struct sexps_t* sub_exps;
      };

      struct {
         struct sexps_t* car;
         struct sexps_t* cdr;
      }
   };
} END_TYPE(sexps_t);



TYPE(s_lex_type_t, enum) {
   S_LEX_SYM,
   S_LEX_NUM,
   S_LEX_OPEN_PAREN, S_LEX_CLOSE_PAREN,
   S_LEX_OPEN_SQUARE_BRACK, S_LEX_CLOSE_SQUARE_BRACK,
   S_LEX_STR, S_LEX_CHAR,
   S_LEX_QUOTE
} END_TYPE(s_lex_type_t);

TYPE(s_quote_type_t, enum) {
   S_QUOTE_BACK,
   S_QUOTE_COMMA,
   S_QUOTE_SINGLE
} END_TYPE(s_quote_type_t);

TYPE(s_lex_t, struct) {
   s_lex_type_t type;

   stage_data_t* lex_data;

   union {
      uint64_t num_val;
      byte_string_t* bs_val;
      s_quote_type_t quote_type;
   };

} END_TYPE(s_lex_t);


TYPE(s_lexed_file, struct) {


} END_TYPE(s_lexed_file);

void test_sexps();

#endif //SEXPS_H_INCLUDE

