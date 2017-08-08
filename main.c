#include <unistd.h>

#include "utils.h"
#include "tests.h"
#include "main.h"
#include "elf.h"
#include "x86gen.h"

int main(int nargs, char** args) {
   ///***UNIT TESTS
   //run_tests();
   //return 0;

   ///***readelf alternative:

   //elf_file_t* elf_file = read_elf("a.out");
   elf_file_t* elf_file = read_elf("opcodes.ex");
   elf_file_print_debug(elf_file);

   return 0;

   ///***assembler

   char* str;
   if (true) { //(nargs != 2)
      str = read_file("test.asm");//read_file("life2.asm");//return -1;
      printf("succ");
   }
   else {
      str = read_file(args[1]);
      printf("\n%s read successfully!\n", args[1]);
   }


   int opcodes_len = 0;
   char* opcodes = run_asm(str, &opcodes_len);


   //elf_conf_t conf;
   //conf.
   //elf_mem = gen_elf(/*&elf_conf,*/ &elf_mem_len,
   //                          opcodes, opcodes_len);

   elf_file_t* elf = gen_elf(opcodes, opcodes_len);

   write_file("kopcode.elf.test", elf->mem, elf->mem_len);

   return 0;


   //printf("%i", is_alpha('a'));

}





