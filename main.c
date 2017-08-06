#include <unistd.h>

#include "utils.h"
#include "tests.h"
#include "main.h"


int main(int nargs, char** args) {

   //run_tests();
   //return 0;

   char* str;
   if (true) { //(nargs != 2)
      str = read_file("test.asm");//read_file("life2.asm");//return -1;
      printf("succ");
   }
   else {
      str = read_file(args[1]);
      printf("\n%s read successfully!\n", args[1]);
   }


   run_asm(str);

   //printf("%i", is_alpha('a'));

}





