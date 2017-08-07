#ifndef ELF_H_INCLUDE
#define ELF_H_INCLUDE

#include "utils.h"



/***********elf32_header_t*******/

TYPE(elf_version_t, enum) {
   ELF_VERSION_NONE,    //0 = INVALID
   ELF_VERSION_CURRENT, //1 = CURRENT
} END_TYPE(elf_version_t);

//enum elf_file_type (defines)
#define ELF_TYPE_NONE 0 //no file type
#define ELF_TYPE_REL 1 //relocatable file
#define ELF_TYPE_EXEC 2 //executable file
#define ELF_TYPE_DYN 3 //shared object file
#define ELF_TYPE_CORE 4 //core file
#define ELF_TYPE_LOPROC 0xff00 //processor-specific
#define ELF_TYPE_HIPROC 0xffff //processor-specific
//end elf_file_type

TYPE(elf_machine_t, enum) {
   ELF_MACHINE_NONE,          // 0 = No Machine
   ELF_MACHINE_M32,           // 1 = At&T WE 32100
   ELF_MACHINE_SPARC,         // 2 = SPARC
   ELF_MACHINE_386,           // 3 = Intel 80386
   ELF_MACHINE_68K,           // 4 = Motorola 68000
   ELF_MACHINE_88K,           // 5 = Motorola 88000
   ELF_MACHINE_UNKNOWN,       // 6 = ???
   ELF_MACHINE_860,           // 7 = Intel 80860
   ELF_MACHINE_MIPS           // 8 = MIPS RS3000
} END_TYPE(elf_machine_t);

/*********elf32_ident_t*********/

TYPE(elf_class_t, enum) {
   ELF_CLASS_NONE, //0 = INVALID
   ELF_CLASS_32,   //1 = 32-bit objects
   ELF_CLASS_64    //2 = 64-bit objects
} END_TYPE(elf_class_t);


TYPE(elf_data_t, enum) {
   ELF_DATA_NONE, //0 = INVALID
   ELF_DATA2_LSB, //1 = 32-bit objects
   ELF_DATA2_MSB  //2 = 64-bit objects
} END_TYPE(elf_data_t);


#define NUM_IDENT 16


TYPE(elf32_ident_t, struct) {
   byte_t magic[4];

   elf_class_t elf_class : 8;
   elf_data_t elf_data : 8;
   elf_version_t elf_version : 8;

   byte_t elf_pad : 8;

   uint16_t elf_num_ident : 16;

} PACKED END_TYPE(elf32_ident_t);

/*****END*elf32_ident_t*********/


TYPE(elf32_header_t, struct) {

   union ident { //16 bytes
      byte_t ident_bytes[NUM_IDENT];
      elf32_ident_t ident_struct;
   } END_TYPE(ident);

   //uint16_t elf_file_type;
   uint16_t file_type : 16;
   elf_machine_t machine_type : 16;
   elf_version_t version : 32;

   //contains virtual address to which system first transfers control
   uint32_t entry_addr : 32; //elf32_addr_t
   uint32_t prog_head_off : 32; //program header table file offset from start of file
   uint32_t sect_head_off : 32; //section header table file offset

   uint32_t flags : 32;

   uint16_t header_size : 16; //sizeof(elf32_header_t)

   uint16_t prog_head_size : 16; //size of one entry in program header table (elf32_section_header_t)
   uint16_t prog_head_num : 16; //num entries in program header table

   uint16_t sect_head_size : 16; //sizeof(elf32_section_header_t)
   uint16_t sect_head_num : 16; //num entries in section header table

   //This member holds the section header table index of the entry associated
   //with the section name string table.  If the file has no section
   //name string table, this member holds the value SHN_UNDEF
   uint16_t sect_head_section_name : 16;


} PACKED END_TYPE(elf32_header_t);


/*******END*elf32_header_t*******/



/********elf32_section_header_t**********/

//TYPE(elf_sect_head_type_t, enum)
#define ELF_SECT_TYPE_NULL                0
#define ELF_SECT_TYPE_PROGBITS            1
#define ELF_SECT_TYPE_SYMTAB              2
#define ELF_SECT_TYPE_STRTAB              3
#define ELF_SECT_TYPE_RELA                4
#define ELF_SECT_TYPE_HASH                5
#define ELF_SECT_TYPE_DYNAMIC             6
#define ELF_SECT_TYPE_NOTE                7
#define ELF_SECT_TYPE_NOBITS              8
#define ELF_SECT_TYPE_REL                 9
#define ELF_SECT_TYPE_SHLIB              10
#define ELF_SECT_TYPE_DYNSYM             11
#define ELF_SECT_TYPE_LOPROC     0x70000000
#define ELF_SECT_TYPE_HIPROC     0x7fffffff
#define ELF_SECT_TYPE_LOUSER     0x80000000
#define ELF_SECT_TYPE_HIUSER     0xffffffff
//ENDTYPE(elf_sect_head_type_t)


//TYPE(elf_sect_head_flags_t, enum)
#define ELF_SECT_FLAG_WRITE              0x1
#define ELF_SECT_FLAG_ALLOC              0x2
#define ELF_SECT_FLAG_EXECINSTR          0x4
#define ELF_SECT_FLAG_MASKPROC    0xf0000000
//END_TYPE(elf_sect_head_flags_t);

TYPE(elf32_section_header_t, struct) {
   uint32_t sect_name;
   uint32_t sect_type; //elf_sect_head_type_t

   uint32_t sect_flags; //elf_sect_head_flags_t
   uint32_t sect_addr;
   uint32_t sect_offset;
   uint32_t sect_size;

   uint32_t sect_link;

   uint32_t sect_info;
   uint32_t sect_addr_align;
   uint32_t sect_ent_size;

} PACKED END_TYPE(elf32_section_header_t);

/*******END elf32_section_header_t*******/



/********elf32_program_header_t**********/


TYPE(elf32_program_header_t, struct) {

   uint32_t seg_type : 32;

   //offset from beginning of the file to where segment resides
   uint32_t seg_file_offset : 32;

   uint32_t seg_virt_addr;
   uint32_t seg_phys_addr;

   uint32_t file_size; //number of bytes in file image of segment
   uint32_t mem_sizes; //# of bytes in image of the segment

   uint32_t seg_flags;
   uint32_t seg_align;


} PACKED END_TYPE(elf32_program_header_t);

/*******END elf32_program_header_t*******/



/*TYPE(elf_conf_t, struct) {
   elf_machine_t machine_type;
   uint16_t elf_file_type;
} END_TYPE(elf_conf_t);*/



void elf_init_header(elf32_header_t* header,
                     uint16_t elf_file_type);

void elf_set_header_misc(elf32_header_t* header,
                         uint32_t entry_addr,
                         uint32_t flags);

void elf_set_header_prog_table(elf32_header_t* header,
                               uint32_t offset,
                               uint16_t entry_size,
                               uint16_t num_entries);

void elf_set_header_sect_table(elf32_header_t* header,
                               uint32_t offset,
                               uint16_t entry_size,
                               uint16_t num_entries);


/*
linking view:
   ELF header
   Program header table (optional)
   Section 1
   Section 2
   Section header table

execution view:
   ELF header
   Program Header Table
   Segment 1
   Segment 2
   Section header table (optional)
*/


#endif //ELF_H_INCLUDE


