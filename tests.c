#include "utils.h"
#include "tests.h"

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

void strip_at_every_test(char* test_lines) {
   char* lines = strip(test_lines);

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


void strip_at_every_tests() {
   char* line1 = "hello\nworld\nI\nam\nkostya";
   char* line2 = "hello\nworld\nIdsf\nadsdfm\nkostya";
   char* line3 = "\nhello\n\nworld\nhello\n\n";
   strip_at_every_test(line1);
   strip_at_every_test(line2);
   strip_at_every_test(line3);
}

void run_tests() {
   strip_at_every_tests();
   split_at_tests();
   strip_tests();
}


