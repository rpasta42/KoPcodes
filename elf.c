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

   header->flags = 0;

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



elf_file_t* gen_elf(byte_t* opcodes, int len_opcodes)
{
   //
   int num_prog_header_entries = 2;
   int num_sect_header_entries = 5;

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

   /* figure out the total size of our file and allocate needed memory */
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
   header = (elf32_header_t*)mem;
   prog_header_entries = (elf32_program_header_t*)mem + prog_header_i;
   sect_header_entries = (elf32_section_header_t*)mem + sect_header_i;

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

   elf_file_t* f = malloc(sizeof(elf_file_t));
   f->mem = mem;
   f->mem_len =  mem_len;

   f->num_prog_header_entries = num_prog_header_entries;
   f->num_sect_header_entries = num_sect_header_entries;

   f->header = header;
   f->prog_header_entries = prog_header_entries;
   f->sect_header_entries = sect_header_entries;

   //TODO: generate text and string section


   return f;
}





//TODO: return NULL if no string table
elf32_section_header_t* get_string_table_header(elf_file_t* elf_file)
{
   elf32_header_t* header = elf_file->header;

   elf32_section_header_t *sect_head =
      &elf_file->sect_header_entries[header->sect_head_section_name];

   return sect_head;
}




elf_file_t* read_elf(char* fname)
{

   uint32_t mem_len = 0;
   byte_t* mem = read_file_bin(fname, &mem_len);

   elf_file_t* elf_file = malloc(sizeof(elf_file_t));


   elf_file->mem = mem;
   elf_file->mem_len = mem_len;

   elf_file->header = (elf32_header_t*)mem;

   elf_file->prog_header_entries = (elf32_program_header_t*)
                                       mem + elf_file->header->prog_head_off;

   elf_file->sect_header_entries = (elf32_section_header_t*)
                                       mem + elf_file->header->sect_head_off;


   elf32_section_header_t* str_sect_header = get_string_table_header(elf_file);
   if (str_sect_header) {
      elf_file->str_sect_table_header = str_sect_header;

      elf_file->str_sect_offset = str_sect_header->sect_offset;
      elf_file->str_sect_strings_p = elf_file->str_sect_offset + mem;
      elf_file->str_sect_strings_len = str_sect_header->sect_size;
   }
   else {
      elf_file->str_sect_table_header = NULL;
      elf_file->str_sect_offset = 0;
      elf_file->str_sect_strings_p =  NULL;
   }

   return elf_file;
}


