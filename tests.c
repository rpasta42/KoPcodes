#include <assert.h>
#include "utils.h"
#include "tests.h"
#include "main.h"
#include "elf.h"

//comment out for non-silent
#define SILENT_TEST 1
#define SUCCESS_TEST(name) printf("\n****test passed: %s\n", name)

void split_at_test(string_t str, string_t splitter) {
   char** parts = split_at(str, splitter);
   if (parts == NULL)
      printf("splitting error");
   else
      printf("fst part: %s\nsnd part: %s\n", parts[0], parts[1]);

   free(parts[0]);
   free(parts[1]);
   free(parts);
}

void run_split_at_tests() {
   char* str1 = "testaooahi";
   char* splitter1 = "oo";
   split_at_test(str1, splitter1);


   char* str2 = "Hello world!";
   char* splitter2 = " ";
   split_at_test(str2, splitter2);
}


void lex_test(char* str) {

   int num_lines;
   lexed_line_t* lexed = lex(str, &num_lines);

   int i, j;
   for (i = 0; i < num_lines; i++) {
      lexed_line_t* lexed_line = &lexed[i];
      printf("line with %i lexemes: ", lexed_line->num_lexemes);
      for (j = 0; j < lexed_line->num_lexemes; j++) {
         lexeme_t* lexeme = &lexed_line->lexemes[j];
         printf("\t%i:%s", lexeme->len, lexeme->str);
      }
      printf("\n");
   }
   printf("\n\n");
}


void split_at_every_test(char* test_lines) {
   char* lines = strip(test_lines);
   //printf("\noriginal:%s\tnew:%s\n", test_lines, lines);

   int num_lines;
   char** splitted = split_at_every(lines, "\n", &num_lines);
   printf("number splits: %i\n", num_lines);

   int i = 0;
   for (i = 0; i < num_lines; i++) {
      printf("%s\n", splitted[i]);
      free(splitted[i]);
   }
   free(splitted);
   free(lines);
}



void run_byte_string_tests() {
   byte_string_t bs1;
   byte_string_init_from_bytes(&bs1, "hello", 5);
   //byte_string_init_from_str(&bs1, "hello");

   byte_string_t* bs2 = byte_string_new_from_str(" world!");

   byte_string_t* bs3 = byte_string_add(&bs1, bs2);

   c_string_t s = byte_string_to_c_str(bs3);
   assert(s != NULL);

   #ifndef SILENT_TEST
      printf("%s\n", s);
   #endif

   byte_string_t* bs4 = byte_string_new_from_str("hello world!");
   byte_string_t* bs5 = byte_string_new_from_str("hello world! hello");
   assert(byte_string_cmp(bs3, bs4) == 0);


   byte_string_append_char(bs3, ' ');
   byte_string_append(bs3, &bs1);
   assert(byte_string_cmp(bs3, bs5) == 0);

   byte_string_free(&bs1, false);
   byte_string_free(bs2, true);
   byte_string_free(bs3, true);
   byte_string_free(bs4, true);
   byte_string_free(bs5, true);

   free(s);

   SUCCESS_TEST("run_byte_string_tests()");
}

void run_rb_tree_tests() {

   rb_tree_t rb_tree;
   rb_tree_new(&rb_tree);

   rb_tree_insert(&rb_tree, 100, "hello");
   rb_tree_print(&rb_tree);
   rb_tree_insert(&rb_tree, 8, "world");
   rb_tree_print(&rb_tree);
   rb_tree_insert(&rb_tree, 4, "test");
   rb_tree_print(&rb_tree);
   rb_tree_insert(&rb_tree, 10, "aaa");
   rb_tree_print(&rb_tree);

   //rb_tree_insert(&rb_tree, 90, "bbb");



   void* res1 = rb_tree_search(&rb_tree, 100);
   void* res2 = rb_tree_search(&rb_tree, 8);
   void* res3 = rb_tree_search(&rb_tree, 4);
   void* res4 = rb_tree_search(&rb_tree, 10);
   //void* res5 = rb_tree_search(&rb_tree, 90);

   void* res6 = rb_tree_search(&rb_tree, 99); //bad

   assert(strcmp(res1, "hello") == 0);
   assert(strcmp(res2, "world") == 0);
   assert(strcmp(res3, "test") == 0);
   assert(strcmp(res4, "aaa") == 0);
   //assert(strcmp(res5, "bbb") == 0);

   assert(res6 == NULL);

   SUCCESS_TEST("run_rb_tree_tests()");

}

void run_strip_tests() {
   char* stripped1 = strip("   hello  world!   f  ");
   char* stripped2 = strip("  mov eax, 5   \nmov   edx,  9 ;\n\n  hello    dsfds f  fsd");

   printf("%s\n", stripped1);
   printf("%s\n", stripped2);
   free(stripped1);
   free(stripped2);


   assert(strcmp(stripped1, " hello world! f "));

   SUCCESS_TEST("run_rb_tree_tests()");

}

void run_split_at_every_tests() {
   char* line1 = "hello\nworld\nI\nam\nkostya";
   char* line2 = "hello\nworld\nIdsf\nadsdfm\nkostya";
   char* line3 = "\nhello\n\nworld\nhello\n\n";
   split_at_every_test(line1);
   split_at_every_test(line2);
   split_at_every_test(line3);
}

void run_lex_tests() {
   //constants 4 and 5 have length of 2 for some reason
   //lex_test("mov eax, 4\nmov ebx, 5\npush edx");
   //lex_test("mov ;test world\nin");
   //lex_test("mov ;test world\nin\npush");
   char* lex_test_str1 = "mov eax, 4\nmov ebx, 5\npush edx";
   char* lex_test_str2 = "mov ;test world\nin";

   char* lex_test_str3 = "SECTION .data\n\n   ;message\n fopen";

   char* lex_test_str4 = "SECTION .data\nmov ;hi\n;message\n fopen";
   char* lex_test_str5 = "mov eax, 0x4a\nmov ebx, 0x5\npush edx";

   char* lex_test_str6 = "fopen_succ_str db 'Successfully opened file', 0xa\n";



   char* str = lex_test_str6;

   int num_lines;
   lexed_line_t* lexed = lex(str, &num_lines);

   instr_t* instructs = gen_instructions(lexed, num_lines);

   printf("\nerror if 0: %lu\n", (uint64_t)instructs);
   if (instructs) {
      print_instructs(instructs, num_lines);
   }

}

void run_elf_header_size_tests() {
   //printf("%i", is_alpha('a'));
   int elf_prog_header_size = sizeof(elf32_program_header_t);
   int total_headers = ELF32_HEADER_SIZE + elf_prog_header_size*3;

   printf("elf header size: %i\nprog header size:%i\ntogether:%i\nhex:%04X\n\n",
          ELF32_HEADER_SIZE,
          elf_prog_header_size,
          total_headers,
          total_headers);
}

void run_tests() {
   run_byte_string_tests();

   run_strip_tests();
   //run_rb_tree_tests();

   return;

   run_lex_tests();
   run_elf_header_size_tests();


   run_split_at_every_tests();
   run_split_at_tests();
}


