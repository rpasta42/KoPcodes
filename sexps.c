#include "sexps.h"
#include "types.h"




void lex2(lexed_file_t* f) {
   char* stripped = strip(f->str_data);

   LIST_NEW(f->lines, lexed_line_t,
            f->num_lines, f->len_lines);

   int index_str_start = 0;
   int index_str_end = 0;

   char c;
   int i = 0;
   for (i = 0; i < f->str_len; i++) {
      c = f->str_data[i];
      switch (c) {
         case ';':
            while (c != '\n' && c != '\0') {
               c = f->str_data[++i];
            }
            continue;
         break;

         case '\'':
            index_str_start = i;
            while (c != '\'') {
               c = f->str_data[++i];
            }

         break;

         default:

         break;
      }

   }
}



sexps_t* sexps_parse(byte_string_t* bs) {


   int num_lexemes = 0;
   s_lex_t* lexemes = sexps_lex(bs, &num_lexemes);


   int nestedness = 0;

}


//void s_new_lex_sym_str(

s_lex_t* sexps_lex(byte_string_t* bs, int* num_lexs) {

   s_lex_t* lexs_ret;
   s_lex_t* l;

   int len_lexs;

   LIST_NEW(lexs_ret, s_lex_t, *num_lexs, len_lexs);

   char c;
   size_t i;

   for (i = 0; i < bs->len; i++) {
      c = bs->str[i];

      switch (c) {
         case '(':
            LIST_ADD_ITEM_SPACE(lexes_ret, s_lex_t, *num_lexs, len_lexs);
            l = &lexs_ret[(*num_lexs)++];


      }

   }

}

sexps_t* s_cons(sexps_t* a, sexps_t* b) {
   sexps_t* s = malloc(sizeof(sexps_t));
   s->num_sub = 2;
   s->len_sub_exps = 0;

   s->car = a;
   s->cdr = b;
   return s;
}

sexps_t* s_car(sexps_t* s) {
   return &s->sub_exps[0];
}

sexps_t* s_cdr(sexps_t* s) {
   return &s->sub_exps[1];
}

sexps_t* s_mk_atom_int32(int n) {
   sexps_t* s = malloc(sizeof(sexps_t));
   s->num_sub = S_ATOM_INT32;
   s->atom_val32 = (int32_t)n;
   return s;
}

sexps_t* s_mk_atom_str(byte_string_t* bs) {
   sexps_t* s  = malloc(sizeof(sexps_t));
   s->atom_type = S_ATOM_STR;
   s->atom_val64 = byte_string_copy(bs);
}

int s_get_atom_val_int32(sexps_t* e) {
   return *(int32_t*)e->atom_val32;
}

byte_string_t* s_get_atom_val_str(sexps_t* e) {
   return (sexps_str_t*)e->atom_val64;
}

void s_print(sexps_t* exp) {
   sexps_str_t* s;

   switch (exp->num_sub) {
      case S_ATOM_NUM:
         printf("number: %i", s_get_atom_val_num(exp));
         break;
      case S_ATOM_CHAR:
         printf("char: %c", (char)s_get_atom_val_num(exp));
         break;
      case S_ATOM_STR:
         s = s_get_atom_val_str(exp);
         printf("str: %s", s->s);
         break;
      case 2:
         printf("cons pair: ");
         break;
      default:
         if (exp->num_sub > 2) {
            printf("list");
         } else if (exp->num_sub == 0 || exp->num_sub == 1) {
            printf("BAD SEXPS!");
         } else {
            printf("unknown SEXPS!!");
         }

         break;
   }

}



void test_sexps() {
   sexps_t* x = s_mk_atom_num(4);
   sexps_t* y = s_cons(x, S_NULL);

   s_print(x);
   s_print(y);

}



