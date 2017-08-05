#include <unistd.h>

#include "utils.h"
#include "tests.h"
#include "main.h"


void run_asm(char* str) {
   int num_lines;
   lexed_line_t* lexed = lex(str, &num_lines);

   //DEBUG


   instr_t* instructs = gen_instructions(lexed, num_lines);

   printf("\nerror if 0: %i\n", (uint32_t)instructs);
   if (instructs) {
      print_instructs(instructs, num_lines);
   }

   return;
}

int main(int nargs, char** args) {

   //run_tests();
   //return;
   char* str;
   if (true) { //(nargs != 2)
      str = read_file("life2.asm");//return -1;
      printf("succ");
   }
   else {
      str = read_file(args[1]);
      printf("\n%s read successfully!\n", args[1]);
   }


   run_asm(str);

   //printf("%i", is_alpha('a'));

}





