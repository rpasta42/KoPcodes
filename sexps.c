#include "sexps.h"
#include "types.h"


void s_mk_lex(s_lex_t* l,
              s_lex_type_t lex_type,
              stage_data_t lex_data)
{
   l->type = lex_type;
   l->lex_data = lex_data;
   l->num_val = 0;
}


void s_mk_lex_str_sym(s_lex_t* l,
                      s_lex_type_t lex_type,
                      stage_data_t* lex_data,
                      byte_string_t* s)
{
   s_mk_lex(l, lex_type, lex_data);
   l->bs_val = s;
}

void s_mk_lex_num(s_lex_t* l,
                  s_lex_type_t lex_type,
                  stage_data_t lex_data,
                  int64_t val)
{
   s_mk_lex(l, lex_type, lex_data);
   l->num_val = val;
}

//void add_lex(s_lexed_file* f,





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

s_lex_t* sexps_lex(byte_string_t* bs, int* num_lexs) {

   s_lexed_file* lf = s_new_lexed_file();

   s_lex_t* l;
   s_lex_t* lexs_ret; /* TODO: lexs_ret, num_lexes, and len_lexes should be part of s_lexed_file */


   int len_lexs;

   LIST_NEW(lexs_ret, s_lex_t, (*num_lex)s, len_lexs);


   char c;
   size_t i;

   for (i = 0; i < bs->len; i++) {
      c = bs->str[i];

      switch (c) {
         case '(':
            add_lex(lexed_file, S_LEX_OPEN_PAREN

            /* TODO: move this to add_lex() */
            LIST_ADD_ITEM_SPACE(lexs_ret, s_lex_t, *num_lexs, len_lexs);
            l = &lexs_ret[(*num_lexs)++];



      }

   }

}

sexps_t* sexps_parse(byte_string_t* bs) {


   int num_lexemes = 0;
   s_lex_t* lexemes = sexps_lex(bs, &num_lexemes);


   int nestedness = 0;

}


//void s_new_lex_sym_str(



sexps_t* s_cons(sexps_t* a, sexps_t* b) {
   sexps_t* s = malloc(sizeof(sexps_t));
   s->atom_type = S_ATOM_CONS;
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
   s->atom_type = S_ATOM_INT32;
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

   switch (exp->num_sub_exps) {
      case S_ATOM_INT32:
         printf("number: %i", s_get_atom_val_int32(exp));
         break;
      case S_ATOM_CHAR:
         printf("char: %c", (char)s_get_atom_val_int32(exp));
         break;
      case S_ATOM_STR:
         s = s_get_atom_val_str(exp);
         printf("str: %s", s->s);
         break;
      case 2:
         printf("cons pair: ");
         break;
      default:
         if (exp->num_sub_exps > 2) {
            printf("list");
         } else if (exp->num_sub_exps == 0 || exp->num_sub_exps == 1) {
            printf("BAD SEXPS!");
         } else {
            printf("unknown SEXPS!!");
         }

         break;
   }

}



void test_sexps() {
   sexps_t* x = s_mk_atom_int32(4);
   sexps_t* y = s_cons(x, S_NULL);

   s_print(x);
   s_print(y);

}



