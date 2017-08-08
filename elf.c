#include "elf.h"



void elf_init_ident(elf32_ident_t* ident) {
   char* elf_str = "ELF";

   ident->magic[0] = 0x7f;
   memcpy(&ident->magic[1], elf_str, 3);

   ident->elf_class = ELF_CLASS_32;
   ident->elf_data = ELF_DATA2_LSB; //little endian x86
   ident->elf_version = ELF_VERSION_CURRENT;

   ident->elf_pad = 0;

   ident->elf_num_ident = 16;

}



/***********elf32_header_t*******/

void elf_init_header(elf32_header_t* header,
                     uint16_t file_type)
{
   elf_init_ident(&header->ident.ident_struct);

   header->file_type = file_type;
   header->machine_type = ELF_MACHINE_386;
   header->version = ELF_VERSION_CURRENT;

   header->flags = NULL;

   header->header_size = sizeof(elf32_header_t);

}

void elf_set_header_misc(elf32_header_t* header,
                         uint32_t entry_addr,
                         uint32_t flags)
{
   header->entry_addr = entry_addr;
   header->flags = flags;
}


void elf_set_header_prog_table(elf32_header_t* header,
                               uint32_t offset,
                               uint16_t entry_size,
                               uint16_t num_entries)
{
   header->prog_head_off = offset;
   header->prog_head_size = entry_size;
   header->prog_head_num = num_entries;
}


void elf_set_header_sect_table(elf32_header_t* header,
                               uint32_t offset,
                               uint16_t entry_size,
                               uint16_t num_entries)
{
   header->sect_head_off = offset;
   header->sect_head_size = entry_size;
   header->sect_head_num = num_entries;
}


/*******END*elf32_header_t*******/



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




byte_t* gen_elf(int* ret_len,
                byte_t* opcodes, int len_opcodes)
{
   //
   int num_prog_header_entries = 1;
   int num_sect_header_entries = 2;

   /* mem contains future file */
   byte_t* mem;

   /* all of this point will point to a location in mem */
   elf32_header_t* header;
   byte_t* new_opcodes_p;
   elf32_program_header_t* prog_header_entries;
   elf32_section_header_t* sect_header_entries;


   /* we figure out sizes for tables */
   int sect_table_size =
         (num_sect_header_entries * ELF32_SECTION_HEADER_ENTRY_SIZE);

   int prog_table_size =
         (num_prog_header_entries * ELF32_PROGRAM_HEADER_ENTRY_SIZE);

   /* TODO: section sizes should be calculated here */

   /* figure out the total size of our file */
   int mem_len = ELF32_HEADER_SIZE + len_opcodes
                 + sect_table_size
                 + prog_table_size;


   mem = malloc(mem_len);

   /* TODO: first calculate and store sizes and indices,
    * then set pointers to (indices + mem)
    */
   int prog_header_i = ELF32_HEADER_SIZE;
   int opcodes_i = prog_header_i + ELF32_HEADER_SIZE;
   int sect_header_i = mem_len - sect_table_size;



   /* figure out the pointers based on mem */
   //TODO: ??? where does the program start?? rn at end of prog table
   new_opcodes_p = mem + opcodes_i;
   header = mem;
   prog_header_entries = mem + prog_header_i;
   sect_header_entries = mem + sect_header_i;

   /* *****************************/
   /* start initializing elf data */
   elf_init_header(header, ELF_TYPE_EXEC);
   elf_set_header_prog_table(header,
                             prog_header_i,
                             ELF32_PROGRAM_HEADER_ENTRY_SIZE,
                             num_prog_header_entries);

   elf_set_header_sect_table(header,
                             sect_header_i,
                             ELF32_SECTION_HEADER_ENTRY_SIZE,
                             num_sect_header_entries);



   elf_init_section_header(&sect_header_entries[0],
                           0, ELF_SECT_TYPE_NULL,  //mae, type
                           0, 0, 0, 0, //flags, address, file offset, size
                           ELF_SECT_IN_UNDEF, //link
                           0, 0, 0); //info, addr_align, ent_size

   //elf_init_prog_header(&prog_header_entries[0]);




   //elf_set_header_misc

   //memcpy(elf_mem, &header, ELF32_HEADER_SIZE);
   memcpy(mem + ELF32_HEADER_SIZE, opcodes, len_opcodes);

   *ret_len = mem_len;
   return mem;
}


elf_file_t* read_elf(char* fname)
{

   uint32_t mem_len = 0;
   byte_t* mem = read_file_bin(fname, &mem_len);

   elf_file_t* elf_file = malloc(sizeof(elf_file_t));


   elf_file->mem = mem;
   elf_file->mem_len = mem_len;

   elf_file->header = mem;

   elf_file->prog_header_entries = mem + elf_file->header.prog_head_off;

   elf_file->sect_header_entries = mem + elf_file->header.sect_head_off;



   return elf_file;
}


void print_elf_prog_table(elf_file_t* elf_file) {

   elf32_program_header_t* prog_header = elf_file->prog_header_entries;

   uint32_t prog_entry_num = elf_file->header->prog_head_num;
}

void print_elf_sect_table(elf_file_t* elf_file) {
   elf32_section_header_t* sect_header = elf_file->sect_header_entries;

   uint32_t prog_entry_num = elf_file->header->sect_head_num;

}


void elf_print_header(elf32_header_t* header) {
   bool is_elf = header->ident.ident_bytes[0] == 0x7f;
   if (!(0 == strncmp("ELF", &header->ident.ident_bytes[1], 3)))
      is_elf = false;

   print_mem(header, 3);

   if (is_elf)
      printf("\nmagic number match");
   else {
      printf("\nbad magic number");
      goto elf_header_err;
   }

   printf("\nelf class: ");
   elf32_ident_t elf_ident = header->ident.ident_struct;
   switch (elf_ident.elf_class) {
      case ELF_CLASS_NONE:
         printf("ELF_CLASS_NONE (invalid)");
         goto elf_header_err;
         break;
      case ELF_CLASS_32:
         printf("ELF_CLASS_32");
         break;
      case ELF_CLASS_64:
         printf("ELF_CLASS_64");
         break;
   }

   printf("\nelf data representation: ");
   switch (elf_ident.elf_data) {
      case ELF_DATA_NONE:
         printf("ELF_DATA_NONE (invalid)");
         goto elf_header_err;
         break;
      case ELF_CLASS_32:
         printf("ELF_DATA2_LSB");
         break;
      case ELF_CLASS_64:
         printf("ELF_DATA2_MSB");
         break;
   }

   printf("\nelf version: %i", elf_ident.elf_version);
   if (elf_ident.elf_version == 0) {
      printf("invalid version");
      goto elf_header_err;
   }

   printf("\nelf num ident: %i", elf_ident.elf_num_ident);


   printf("\nfile type: ");
   switch (header->file_type) {
      case ELF_TYPE_NONE:
         printf("ELF_TYPE_NONE");
         break;
      case ELF_TYPE_REL:
         printf("ELF_TYPE_REL (relocatable executable)");
         break;
      case ELF_TYPE_EXEC:
         printf("ELF_TYPE_EXEC");
         break;
      case ELF_TYPE_DYN:
         printf("ELF_TYPE_DIN");
         break;
      case ELF_TYPE_CORE:
         printf("ELF_TYPE_CORE");
         break;
      case ELF_TYPE_LOPROC:
         printf("ELF_TYPE_LOPROC");
         break;
      case ELF_TYPE_HIPROC:
         printf("ELF_TYPE_HIPROC");
         break;
      default:
         printf("unknown (%i)", header->file_type);
         break;
   }


   printf("\nmachine type: ");
   switch (header->machine_type) {
      case ELF_MACHINE_NONE:
         printf("ELF_MACHINE_NONE");
         break;
      case ELF_MACHINE_M32:
         printf("ELF_MACHINE_M32");
         break;
      case ELF_MACHINE_SPARC:
         printf("ELF_MACHINE_SPARC");
         break;
      case ELF_MACHINE_386:
         printf("ELF_MACHINE_386");
         break;
      case ELF_MACHINE_68K:
         printf("ELF_MACHINE_68K");
         break;
      case ELF_MACHINE_88K:
         printf("ELF_MACHINE_88K");
         break;
      case ELF_MACHINE_UNKNOWN:
         printf("ELF_MACHINE_UNKNOWN");
         break;
      case ELF_MACHINE_860:
         printf("ELF_MACHINE_860");
         break;
      case ELF_MACHINE_MIPS:
         printf("ELF_MACHINE_MIPS");
         break;
      case ELF_MACHINE_x86_64:
         printf("ELF_MACHINE_X86_64");
         break;

      default:
         printf("unknown (%i)", header->machine_type);
         break;
   }

   printf("\nversion: %i", header->version);


   printf("\nentry_addr (bytes):");
   print_mem(&header->entry_addr, 4);
   printf("\nentry_addr (hex): 0x%04X", header->entry_addr);
   printf("\nentry_addr decimal: %i", header->entry_addr);


   printf("\nprog_head_off: %i", header->prog_head_off);
   printf("\nprog_head_num: %i", header->prog_head_num);
   printf("\nprog_head_size: %i", header->prog_head_size);


   printf("\nsect_head_off: %i", header->sect_head_off);
   printf("\nsect_head_num: %i", header->sect_head_num);
   printf("\nsect_head_size: %i", header->sect_head_size);

   printf("\nsect_head_section_name: %i", header->sect_head_section_name);

   printf("\nflags: %i", header->flags);
   printf("\nheader_size: %i", header->header_size);

   //switch (header->file_type)
   printf("\n");

   return;

   elf_header_err:
   printf("\nerror\n");

}




