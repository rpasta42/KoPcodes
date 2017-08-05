#include "utils.h"
#include "tests.h"
#include "main.h"

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

void split_at_tests() {
   char* str1 = "testaooahi";
   char* splitter1 = "oo";
   split_at_test(str1, splitter1);


   char* str2 = "Hello world!";
   char* splitter2 = " ";
   split_at_test(str2, splitter2);
}


void strip_tests() {
   char* stripped1 = strip("   hello  world!   f  ");
   char* stripped2 = strip("  mov eax, 5   \nmov   edx,  9 ;\n\n  hello    dsfds f  fsd");

   printf("%s\n", stripped1);
   printf("%s\n", stripped2);
   free(stripped1);
   free(stripped2);
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


void split_at_every_tests() {
   char* line1 = "hello\nworld\nI\nam\nkostya";
   char* line2 = "hello\nworld\nIdsf\nadsdfm\nkostya";
   char* line3 = "\nhello\n\nworld\nhello\n\n";
   split_at_every_test(line1);
   split_at_every_test(line2);
   split_at_every_test(line3);
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



void run_tests() {
   //constants 4 and 5 have length of 2 for some reason
   //lex_test("mov eax, 4\nmov ebx, 5\npush edx");
   //lex_test("mov ;test world\nin");
   //lex_test("mov ;test world\nin\npush");
   //return;

   char* lex_test_str1 = "mov eax, 4\nmov ebx, 5\npush edx";
   char* lex_test_str2 = "mov ;test world\nin";

   char* lex_test_str3 = "SECTION .data\n\n   ;message\n fopen";
   char* str = lex_test_str3;

   int num_lines;
   lexed_line_t* lexed = lex(str, &num_lines);

   instr_t* instructs = gen_instructions(lexed, num_lines);

   printf("\nerror if 0: %i\n", (uint32_t)instructs);
   if (instructs) {
      print_instructs(instructs, num_lines);
   }

   return;

   strip_tests();
   return;


   //split_at_every_tests();

   //return;




   return;
   split_at_tests();
}


