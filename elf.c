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




byte_t* gen_elf(/*elf_conf_t conf,*/ int* ret_len,
                byte_t* opcodes, int opcodes_num)
{
   elf32_header_t header;
   int header_size = sizeof(elf32_header_t);
   uint32_t entry_addr = header_size;//0x100

   elf_init_header(&header, ELF_TYPE_EXEC);
   //elf_set_header_offsets()

   elf


   int elf_mem_len = header_size + opcodes_num;
   byte_t* elf_mem = malloc(elf_mem_len);

   memcpy(elf_mem, &header, header_size);
   memcpy(elf_mem + header_size, opcodes, opcodes_num);

   *ret_len = elf_mem_len;
   return elf_mem;
}



