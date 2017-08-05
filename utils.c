
#include "utils.h"


bool is_spacy(char c) {
   return c == '\t' || c == ' ' || c == '\n';
}

char* strip(string_t str) {
   size_t str_len = strlen(str);
   char* ret = malloc(str_len);

   bool last_space = true;
   int i = 0, j = 0;
   char c;
   for (i = 0; i < str_len; i++) {
      c = str[i];
      if (is_spacy(c)) {

         if (last_space) {
            if (c == '\n') {
               if (i == 0)
                  continue;
               do {
                  j--;
               } while (is_spacy(ret[j]));
               j++;
               //ret[j++] = '\n';
            }
            else
               continue;
         }
         last_space = true;
         if (c == '\t' || c == ' ')
            ret[j++] = ' ';
         else //if (c == '\n')
            ret[j++] = '\n';
      }
      else {
         last_space = false;
         ret[j++] = c;
      }
   }

   int last_ret = strlen(ret) - 1;
   if (ret[last_ret] == '\n')
      ret[last_ret] = '\0';
   return ret;
}


/* splits str where left part (return_value[0]) is everything before split,
   and right part (return_value[1]) is everything after split
   edge cases:
      1. split not found: returns NULL
      2.
*/
char** split_at(string_t str, string_t split) {
   uint32_t i = 0;
   size_t str_len = strlen(str);
   size_t split_len = strlen(split);

   string_t occurance = strstr(str, split);

   if (!occurance)
      return NULL;

   int fst_len = occurance - str;
   int snd_len = str_len - fst_len - split_len;

   //printf("fst len: %i\tsnd len:%i\n", fst_len, snd_len);

   if (snd_len < 0 || fst_len < 0)
      return NULL;

   char** ret = malloc(sizeof(char*)*2);
   ret[0] = malloc(fst_len+1);
   ret[1] = malloc(snd_len+1);

   char* c = str;
   i = 0;
   for (; c != occurance; c++) {
      ret[0][i] = *c;
      i++;
   }
   ret[0][i] = '\0';

   c = occurance + split_len;
   i = 0;
   for (; c != str+str_len; c++) {
      ret[1][i] = *c;
      i++;
   }
   ret[1][i] = '\0';
   return ret;
}


//TODO OPTIMIZE: can make this a lot faster for lexer line splitting
//by not re-using split_at and just checking newline character
char** split_at_every(string_t str, string_t split, int* num_splits) {
   int num_ret = 0;
   int len_ret = 5;
   char** ret = malloc(sizeof(char*)*len_ret);

   char* rest = NULL;
   char** result = split_at(str, split);
   while (result != NULL) {

      if (num_ret + 1 >= len_ret) {
         len_ret *= 5;
         ret = realloc(ret, len_ret * sizeof(char*));
      }

      if (rest != NULL) {
         free(rest);
         rest = NULL;
      }


      char* fst = result[0];
      rest = result[1];
      free(result);

      //printf("kk:%s\n", fst);
      ret[num_ret++] = fst; //malloc(strlen(result[0]));

      result = split_at(rest, split);
      //free(rest);
   }


   if (num_ret + 1 >= len_ret) {
      len_ret += 1;
      ret = realloc(ret, len_ret * sizeof(char*));
   }
   ret[num_ret++] = rest;

   *num_splits = num_ret;

   return ret;
}


