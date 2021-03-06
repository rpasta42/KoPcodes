#ifndef ELF_H_INCLUDE
#define ELF_H_INCLUDE

#include "utils.h"
#include "types.h"

//#define DEBUG_BIN_ASM "kopcodes.bin"

#define ELF_INIT_ADDR 0x08048000

/* n = num_prog_header_entires */
#define ELF_ENTRY_ADDR(n) \
   ((n * sizeof(elf32_program_header_t)) \
    + ELF32_HEADER_SIZE \
    + ELF_INIT_ADDR)

//BAD/old addresses

#define ELF_ADDR_TEXT 0x080480c0
#define ELF_OFF_TEXT 0xc0

#define ELF_ADDR_DATA 0x08049134
#define ELF_DATA_OFF 0x000134

#define ELF_LOAD_1_ADDR 0x8048000 //both physical and virtual

//#define ELF_ENTRY_ADDR 0x80480C0

#define ELF_SEG_2_OFFSET 0x134
#define ELF_DATA_ADDR 0x08049134 //0x80480C0

#define ELF_STR_SECTION_INDEX 2
#define ELF_DATA_SECT_INDEX 3

/***********elf32_header_t*******/

#define ELF32_HEADER_SIZE sizeof(elf32_header_t)

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
#define ELF_MACHINE_x86_64 62

/*********elf32_ident_t*********/

TYPE(elf_class_t, enum) {
   ELF_CLASS_NONE, //0 = INVALID
   ELF_CLASS_32,   //1 = 32-bit objects
   ELF_CLASS_64    //2 = 64-bit objects
} END_TYPE(elf_class_t);


TYPE(elf_data_t, enum) {
   ELF_DATA_NONE, //0 = INVALID
   ELF_DATA2_LSB, //1 = lsb
   ELF_DATA2_MSB  //2 = msb
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
   uint32_t entry_addr; //elf32_addr_t
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

#define ELF32_SECTION_HEADER_ENTRY_SIZE sizeof(elf32_section_header_t)

//TYPE(elf_sect_head_indices_t, enum)
#define ELF_SECT_IN_UNDEF 0
#define ELF_SECT_IN_LORESERVE   0xff00
#define ELF_SECT_IN_LOPROC      0xff00
#define ELF_SECT_IN_HIPROC      0xff1f
#define ELF_SECT_IN_ABS         0xfff1
#define ELF_SECT_IN_COMMON      0xfff2
#define ELF_SECT_IN_HIRESERVE   0xffff
//END_TYPE(elf_sect_head_indices_t)

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

#define ELF32_PROGRAM_HEADER_ENTRY_SIZE sizeof(elf32_program_header_t)

//TYPE(elf_prog_head_type_t, struct)
#define ELF_SEG_TYPE_NULL               0
#define ELF_SEG_TYPE_LOAD               1
#define ELF_SEG_TYPE_DYNAMIC            2
#define ELF_SEG_TYPE_INTERP             3
#define ELF_SEG_TYPE_NOTE               4
#define ELF_SEG_TYPE_SHLIB              5
#define ELF_SEG_TYPE_PHDR               6
#define ELF_SEG_TYPE_LOPROC    0x70000000
#define ELF_SEG_TYPE_HIPROC    0x7fffffff
//END_TYPE(elf_prog_head_type_t, struct)

//TYPE(elf_prog_head_flags_t, enum)
#define ELF_PROG_FLAG_X 0x1
#define ELF_PROG_FLAG_W 0x2
#define ELF_PROG_FLAG_R 0x4
#define ELF_PROG_FLAG_MASKOS 0x0ff00000 //????
#define ELF_PROG_FLAG_MASKPROC 0xf0000000 //????
//END_TYPE(elf_prog_head_flags_t)

TYPE(elf32_program_header_t, struct) {

   uint32_t seg_type : 32;

   //offset from beginning of the file to where segment resides
   uint32_t seg_file_offset : 32;

   uint32_t seg_virt_addr;
   uint32_t seg_phys_addr;

   uint32_t file_size; //number of bytes in file image of segment
   uint32_t mem_sizes; //# of bytes in image of the segment

   uint32_t seg_flags; //elf_prog_head_flags_t
   uint32_t seg_align;


} PACKED END_TYPE(elf32_program_header_t);

/*******END elf32_program_header_t*******/

/********elf32_sym_table_entry_t**********/

//TYPE(elf_sym_table_bind_t, enum)
#define ELF_SYM_TABLE_BIND_LOCAL 0
#define ELF_SYM_TABLE_BIND_GLOBAL 1
#define ELF_SYM_TABLE_BIND_WEAK 2
#define ELF_SYM_TABLE_BIND_LOPROC 13
#define ELF_SYM_TABLE_BIND_HIPROC 15
//ENDTYPE(elf_sym_table_bind_t)

//TYPE(elf_sym_table_type_t, enum)
#define ELF_SYM_TABLE_TYPE_NOTYPE  	   0
#define ELF_SYM_TABLE_TYPE_OBJECT      1
#define ELF_SYM_TABLE_TYPE_FUNC        2
#define ELF_SYM_TABLE_TYPE_SECTION     3
#define ELF_SYM_TABLE_TYPE_FILE        4
#define ELF_SYM_TABLE_TYPE_LOPROC     13
#define ELF_SYM_TABLE_TYPE_HIPROC     15
//ENDTYPE(elf_sym_table_type_t)

#define ELF_SYM_TABLE_GET_BIND(i) ((i) >> 4)
#define ELF_SYM_TABLE_GET_TYPE(i) ((i) & 0xF)
#define ELF_SYM_TABLE_MAKE_INFO(bind, type) (((bind) << 4) + ((t) & 0xF))


TYPE(elf32_sym_table_entry_t, struct) {
   uint32_t name; //.strtab section stirng index
   uint32_t	value; //can be address, absolute value, etc
   uint32_t	size;
   byte_t info;
   byte_t other;
   byte_t sect_index;
} PACKED END_TYPE(elf32_sym_table_entry_t);

/*******END elf32_sym_table_entry_t*******/



/*TYPE(elf_conf_t, struct) {
   elf_machine_t machine_type;
   uint16_t elf_file_type;
} END_TYPE(elf_conf_t);*/


TYPE(elf_file_t, struct) {
   byte_t* mem;
   uint32_t mem_len;

   uint32_t num_prog_header_entries;
   uint32_t num_sect_header_entries;


   uint32_t prog_header_offset;
   uint32_t sect_header_offset;
   uint32_t segments_offset;

   elf32_header_t* header;
   elf32_program_header_t* prog_header_entries;
   elf32_section_header_t* sect_header_entries;
   byte_t* opcode;

   elf32_section_header_t* str_sect_table_header;
   uint32_t str_sect_offset;
   char* str_sect_strings_p;
   int str_sect_strings_len;

   sym_table_t* sym_table;
} END_TYPE(elf_file_t);


/**********************FUNCTIONS**********************/

elf_file_t* read_elf(char* fname);
elf_file_t* gen_elf(byte_t* opcodes, int len_opcodes,
			  sym_table_t* sym_table);


/* start global header functions */
void elf_init_header(elf32_header_t* header,
                     uint16_t elf_file_type);

void elf_set_header_misc(elf32_header_t* header,
                         uint32_t entry_addr,
                         uint32_t flags,
                         uint16_t str_section_index);

void elf_set_header_prog_table(elf32_header_t* header,
                               uint32_t offset,
                               uint16_t entry_size,
                               uint16_t num_entries);

void elf_set_header_sect_table(elf32_header_t* header,
                               uint32_t offset,
                               uint16_t entry_size,
                               uint16_t num_entries);
/* end global header */


//void elf_init_section_header(elf32_section_header_t*)
static inline
void elf_init_section_header(elf32_section_header_t* sect_head,
                             uint32_t name,
                             uint32_t type,
                             uint32_t flags,
                             uint32_t addr,
                             uint32_t offset,
                             uint32_t size,
                             uint32_t link,
                             uint32_t info,
                             uint32_t addr_align,
                             uint32_t ent_size)
{
   sect_head->sect_name = name;
   sect_head->sect_type = type;
   sect_head->sect_flags = flags;
   sect_head->sect_addr = addr;
   sect_head->sect_offset = offset;
   sect_head->sect_size = size;
   sect_head->sect_link = link;
   sect_head->sect_info = info;
   sect_head->sect_addr_align = addr_align;
   sect_head->sect_ent_size = ent_size;
}


//void elf_init_prog_header(elf32_program_header_t* h)
static inline
void elf_init_prog_header(elf32_program_header_t* h,
                          uint32_t type,
                          uint32_t file_offset,
                          uint32_t virt_addr,
                          uint32_t phys_addr,
                          uint32_t file_size, //
                          uint32_t mem_sizes,
                          uint32_t flags,
                          uint32_t align)
{
   h->seg_type = type;
   h->seg_file_offset = file_offset;
   h->seg_virt_addr = virt_addr;
   h->seg_phys_addr = phys_addr;
   h->file_size = file_size;
   h->mem_sizes = mem_sizes;
   h->seg_flags = flags;
   h->seg_align = align;
}



//elf_info.c
void elf_file_print_debug(elf_file_t* elf_file);


#endif //ELF_H_INCLUDE


