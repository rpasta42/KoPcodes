#ifndef UTILS_H_INCLUDE
#define UTILS_H_INCLUDE

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define bool char
#define true 1
#define false 0



#define DEBUG   printf("\ngot here\n");
#define PACKED __attribute__((packed))

//type either struct or something else
#define TYPE(name, type) typedef type name
#define END_TYPE(name) name
#define string_t const char*
#define size_t uint32_t
//#define NULL 0

bool is_spacy(char c);
bool is_alpha(char c);
bool is_digit(char c);
bool is_hex_digit(char c);


int parse_hex_num(char* s);
int parse_human_num(char* s);

char* strip(string_t str);
char** split_at(string_t str, string_t split);
char** split_at_every(string_t str, string_t split, int* num_splits);

char* read_file(char *fname);

#endif //UTILS_H_INCLUDE
