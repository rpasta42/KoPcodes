#include <unistd.h>
#include "utils.h"
#include "tests.h"
#include "main.h"
#include "elf.h"
#include "x86gen.h"


void usage(int nargs, char** args) {
   printf("\n%s test\n\trun unit tests", args[0]);
   printf("\n%s readelf <elf_file_path>\n\tprint info on elf file", args[0]);
   printf("\n%s asm <in_file_path.asm> [<out_file_path>]\n\tassemble x86 assembly file",
	    args[0]);
   printf("\n\tout_file_path is optional\n");
}




void main_asm(char* path_in, char* path_out) {
   char* str;
   str = read_file(path_in);
   //char* fname = "test.asm";
   //read_file("life2.asm");

   printf("\n%s read successfully!\n", path_in);


   int opcodes_len = 0;
   char* opcodes = run_asm(str, &opcodes_len);


   //elf_conf_t conf;
   //conf.
   //elf_mem = gen_elf(/*&elf_conf,*/ &elf_mem_len,
   //                          opcodes, opcodes_len);

   elf_file_t* elf = gen_elf(opcodes, opcodes_len);

   char* path_out2 = "kopcodes.elf.test.ex";
   if (path_out == NULL)
	path_out = path_out2;

   write_file(path_out, elf->mem, elf->mem_len);
}

//arguments similar to GNU readelf
//if arg is 0, elf_file_print_debug()
void main_readelf(char* elf_file_path, char* arg) {
   elf_file_t* elf_file = read_elf(elf_file_path);

   if (arg == NULL)
	elf_file_print_debug(elf_file);

   elf_file_free(elf_file);
}

int main(int nargs, char** args)
{

   test_sexps();
   return;

   if (nargs == 1)
	goto args_format_err;

   bool is_test = 0 == strcmp(args[1], "test");
   bool is_elf_info = 0 == strcmp(args[1], "readelf");
   bool is_asm = 0 == strcmp(args[1], "asm");

   if (is_test) {
	if (nargs == 2)
	   printf("running tests\n");
	else
	   goto args_format_err;
	run_tests();
   }
   else if (is_elf_info) {
	//TODO: later on add more cmd args here and
	//main_readelf()
	if (nargs != 3)
	   goto args_format_err;
	printf("readelf()\n");


	//elf_file_t* elf_file = read_elf("a.out");
	//"kopcode.elf.test"; //"opcodes.ex";
	char* path = args[2];
	main_readelf(path, NULL);
   }
   else if (is_asm) {
	if (nargs != 3 && nargs != 4)
	   goto args_format_err;

	char* path_in = args[2];
	char* path_out = (nargs == 4) ? args[3] : NULL;
	main_asm(path_in, path_out);

   }

   return 0;

args_format_err:;

   printf("\ninvalid args\n");
   usage(nargs, args);
   return -1;

}


