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


sexps_t* s_cons(sexps_t* a, sexps_t* b) {
   sexps_t* s = malloc(sizeof(sexps_t));
   s->num_sub = 2;

   s->car = a;
   s->cdr = b;
   return s;
}

sexps_t* s_car(sexps_t* s) {
   return s->car;
}

sexps_t* s_cdr(sexps_t* s) {
   return s->cdr;
}

sexps_t* s_mk_atom_num(int n) {
   sexps_t* s = malloc(sizeof(sexps_t));
   s->num_sub = S_ATOM_NUM;
   s->atom = (uint32_t)n;
   return s;
}

int s_get_atom_val_num(sexps_t* e) {
   return e->atom;
}

sexps_str_t* s_get_atom_val_str(sexps_t* e) {
   return (sexps_str_t*)e->atom;
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



