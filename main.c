#include <unistd.h>

#include "utils.h"
#include "tests.h"
#include "main.h"


//***ELF

typedef struct elf32_header_t {
   char magic[4];

} PACKED elf32_header_t;


void init_header(elf32_header_t* header) {
   char* elf_str = "ELF";


   header->magic[0] = 0x7f;
   memcpy(&header->magic[1], elf_str, 3);
}


//***END ELF





//typedef enum instr_arg_types {} instr_arg_types;
TYPE(instr_arg_type, enum) {
   ArgReg8,
   ArgReg16,
   ArgReg32,
   ArgConst,
   ArgAddr
} END_TYPE(instr_arg_type);

//typedef struct instr {} instr;
TYPE(instr, struct) {
   char* name;
   instr_arg_type* arg1;
   instr_arg_type* arg2;
} END_TYPE(instr);


/*
enum instr_args {
   InstrNone,
   InstrConst, //0x80 43
   InstrAddr,
   InstrReg,
   InstrRegConst2, //mov eax, 0x04
   Instr

}

enum opcodes {
   mov_r16_const
};
*/



int main() {
   //run_tests();
   printf("%i", is_alpha('a'));

}





