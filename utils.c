
#include "utils.h"


bool is_spacy(char c) { //is space-like
   return c == '\t' || c == ' ' || c == '\n';
}

bool is_alpha(char c) {
   return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_digit(char c) {
   return c >= '0' && c <= '9';
}

bool is_hex_digit(char c) {
   return is_digit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}


//returns 0xDEADBAEF if can't parse
int parse_hex_num(char* s, char* endp) {
   /*
   int i;
   for (i = 0; i < strlen(s); i++) {
      if (!is_hex_digit(s[i]) && s[i] != ' ')
         return 0xDEADBAEF;
   }*/
   return strtol(s, endp, 16);
}

//returns 0xDEADBAEF if can't parse
int parse_human_num(char* s, char* endp) {
   int i;
   int s_len = strlen(s);
   /*for (i = 0; i < s_len; i++) {
      if (!is_digit(s[i]) && s[i] != ' ')
         return 0xDEADBAEF;
   }*/


   //return strtol(s, NULL, 10);
   return strtol(s, endp, 10);

}



char* strip(string_t str) {
   size_t str_len = strlen(str);
   char* ret = malloc(str_len+1);

   bool last_space = true;
   int i = 0, j = 0;
   char c;
   for (i = 0; i < str_len; i++) {
      c = str[i];
      if (is_spacy(c)) {

         if (last_space) {
            if (c != '\n')
               continue;

            if (i == 0 || j == 0)
               continue;
            do {
               j--;
            } while (is_spacy(ret[j]) && j > 0);
            j++;
            //ret[j++] = '\n';
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

   ret[j] = '\0';

   int last_ret = strlen(ret) - 1;
   if (ret[last_ret] == '\n')
      ret[last_ret] = '\0';

   //ret[last_ret] = '\0';

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

   if (snd_len <= 0 || fst_len <= 0)
      return NULL;

   char** ret = malloc(sizeof(char*)*2);
   //printf("fst:%i snd:%i\n", fst_len, snd_len);
   ret[0] = malloc(fst_len+1);
   ret[1] = malloc(snd_len+1);

   const char* c = str;
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


char* read_file(char *fname) {
   char *buff = NULL;
   int flen, bytes_read;
   FILE *f = fopen(fname, "r");

   if (f) {
      //printf("\nopened file successfully\n");
	fseek(f, 0, SEEK_END); //last byte of the file

	flen = ftell(f); //offset first to last byte (file size)

	rewind(f); // go back to the start of the file

	buff = (char*) malloc(sizeof(char) * (flen + 1) );

	//read the whole file
	bytes_read = fread(buff, sizeof(char), flen, f);

	// fread doesn't set it so put a \0 in the last position for string
	buff[flen] = '\0';

	if (flen != bytes_read) {
	   //error

         printf("failed to read file\n");
	   free(buff);
	   buff = NULL;
	}

	// Always remember to close the file.
	fclose(f);
   }
   else {
      printf("\nerrorr opening/reading file\n");
   }
   return buff;
}


void write_file(char* fname, char* data, int data_len) {
   char* buff = NULL;
   int flen, bytes_read;
   FILE* f = fopen(fname, "wb");

   if (f) {

      int block_size = 1024;
      int num_blocks = data_len / block_size;

      /*while (data_len > 0) {
         fwrite(data, block_size, 1,
      }*/
      fwrite(data, 1, data_len, f);
	// Always remember to close the file.
	fclose(f);
   }
   else {
      printf("\nerrorr opening/writing file\n");
   }
   return buff;
}

void print_mem(char* bytes, int num_bytes) {
   int i;
   for (i = 0; i < num_bytes; i++) {
      byte_t b = *(uint8_t*)(bytes+i);
      //printf("__%i__", i);
      if (b <= 0xF)
         printf("0");
      printf("%01X ", b);
   }
}



