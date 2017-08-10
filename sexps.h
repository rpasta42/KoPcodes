#ifndef SEXPS_H_INCLUDE
#define SEXPS_H_INCLUDE

#include "utils.h"

#define S_NULL NULL

#define S_ATOM_NUM -1
#define S_ATOM_CHAR -2
#define S_ATOM_STR -3

TYPE(sexps_str_t, struct) {
   size_t len;
   char* s;
} END_TYPE(sexps_str_t);

TYPE(sexps_t, struct) {
   int num_sub; //num_sub < 0 = atom

   union {
      uint64_t atom;

      struct {
         struct sexps_t* car;
         struct sexps_t* cdr;
      };
   };
} END_TYPE(sexps_t);

void test_sexps();

#endif //SEXPS_H_INCLUDE
