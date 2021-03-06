#ifndef UTILS_H_INCLUDE
#define UTILS_H_INCLUDE

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define bool char
#define true 1
#define false 0



#define DEBUG(s)  printf("\ngot here:%s\n", s);
#define PACKED __attribute__((packed))

//type either struct or something else
#define TYPE(name, type) typedef type name
#define END_TYPE(name) name
#define string_t const char*
#define size_t uint32_t
//#define NULL 0
typedef unsigned char byte_t;
typedef const char* c_string_t;

//#define byte_t char

#define MIN(a, b) ((a < b) ? a : b)

#define K_FREE(x) do { \
   free(x); \
   x = NULL; \
} while (0)



#include "algo.h"


bool is_spacy(char c);
bool is_alpha(char c);
bool is_digit(char c);
bool is_hex_digit(char c);


int parse_hex_num(char* s, char* endp);
int parse_human_num(char* s, char* endp);

char* strip(string_t str);
char** split_at(string_t str, string_t split);
char** split_at_every(string_t str, string_t split, int* num_splits);

char* read_file(char* fname);
byte_t* read_file_bin(char *fname, int* len);
void write_file(char* fname, char* data, int data_len);


//prints memory in hexadecimal
void print_mem(char* bytes, int num_bytes);


void to_lower_str(char* p);

#endif //UTILS_H_INCLUDE

