#include <unistd.h>

#include "utils.h"
#include "tests.h"
#include "main.h"
#include "elf.h"

int main(int nargs, char** args) {
   //run_tests();
   //return 0;

   //elf_file_t* elf_file = read_elf("a.out");
   elf_file_t* elf_file = read_elf("opcodes.ex");

   elf_print_header(elf_file->header);
   elf_print_sect_table(elf_file);
   elf_print_prog_table(elf_file);
   elf_print_string_table(elf_file);
   return 0;

   ///

   char* str;
   if (true) { //(nargs != 2)
      str = read_file("test.asm");//read_file("life2.asm");//return -1;
      printf("succ");
   }
   else {
      str = read_file(args[1]);
      printf("\n%s read successfully!\n", args[1]);
   }


   int opcodes_len = NULL;
   char* opcodes = run_asm(str, &opcodes_len);


   //elf_conf_t conf;
   //conf.

   int elf_mem_len = NULL;
   byte_t* elf_mem = gen_elf(/*&elf_conf,*/ &elf_mem_len,
                             opcodes, opcodes_len);


   write_file("kopcode.elf.test", elf_mem, elf_mem_len);

   return;


   //printf("%i", is_alpha('a'));

}





