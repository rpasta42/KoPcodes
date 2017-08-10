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
                         uint32_t flags,
                         uint16_t str_section_index)
{
   header->entry_addr = entry_addr;
   header->flags = flags;
   header->sect_head_section_name = str_section_index;
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




#define STR_INDEX_TEXT 47
#define STR_INDEX_SHSTRLAB 16
#define STR_INDEX_DATA (STR_INDEX_TEXT+5)
byte_t* get_string_table(int* string_table_len)
{
   //TODO: table[] on stack or bss or waddup?
   char table[] = ".symtab\0.strtab\0.shstrtab\0.note\0.gnu\0.build-id\0.text\0.data";
   *string_table_len = sizeof(table);
   printf("\nstring table size: %i\n", *string_table_len);
   char* ret = malloc(*string_table_len);
   memcpy(ret, table, *string_table_len);
   return ret;
}

void elf_init_prog_headers(elf_file_t* f)
{

   uint32_t seg_1_mem_file_size = 0x00131;
   uint32_t seg_2_mem_file_size = 0xd;
   uint32_t seg2_offset = ELF_DATA_OFF;

   //prog text seg
   elf_init_prog_header(&f->prog_header_entries[0],
                        ELF_SEG_TYPE_LOAD, //type
                        0, //offset
                        ELF_LOAD_1_ADDR, //virt address
                        ELF_LOAD_1_ADDR, //physical address
                        seg_1_mem_file_size, //file_size
                        seg_1_mem_file_size, //mem_sizes
                        ELF_PROG_FLAG_R | ELF_PROG_FLAG_X,
                        0x1000);

   //figure out data segment offset

   //data segment
   elf_init_prog_header(&f->prog_header_entries[1],
                        ELF_SEG_TYPE_LOAD, //type
                        ELF_SEG_2_OFFSET, //offset
                        ELF_DATA_ADDR, //virt
                        ELF_DATA_ADDR, //phys
                        seg_2_mem_file_size, //file_size
                        seg_2_mem_file_size, //mem_size
                        ELF_PROG_FLAG_R | ELF_PROG_FLAG_W,
                        0x4);


}


void init_sect_prog_tables(elf_file_t* f,
                            byte_t* text_data, uint32_t text_size)
{
   /* first calculate and store sizes and indices,
    * then set pointers to (indices + mem)
    */

   /* we figure out sizes for tables */
   int sect_table_size =
         (f->num_sect_header_entries * ELF32_SECTION_HEADER_ENTRY_SIZE);

   int prog_table_size =
         (f->num_prog_header_entries * ELF32_PROGRAM_HEADER_ENTRY_SIZE);

   int string_table_size = 0;
   byte_t* string_table_data = get_string_table(&string_table_size);

   int data_table_size = 0;


   f->prog_header_offset = ELF32_HEADER_SIZE;
   f->segments_offset = f->prog_header_offset + prog_table_size;

   int total_segments_size = data_table_size + text_size + string_table_size;

   f->sect_header_offset = f->segments_offset + total_segments_size;

   f->mem_len = f->sect_header_offset + sect_table_size;
   f->mem = malloc(f->mem_len);

   f->header = (elf32_header_t*)f->mem;
   f->prog_header_entries = (elf32_program_header_t*)(f->mem + f->prog_header_offset);
   f->sect_header_entries = (elf32_section_header_t*)(f->mem + f->sect_header_offset);


   //TODO: need to update mem_len first
   //int sect_header_i = mem_len - sect_table_size;
   elf_init_section_header(&f->sect_header_entries[0],
                           0, ELF_SECT_TYPE_NULL,  //name, type
                           0, 0, 0, 0, //flags, address, file offset, size
                           ELF_SECT_IN_UNDEF, //link
                           0, 0, 0); //info, addr_align, ent_size

   elf_init_section_header(&f->sect_header_entries[1],
                           STR_INDEX_TEXT, ELF_SECT_TYPE_PROGBITS,
                           ELF_SECT_FLAG_ALLOC | ELF_SECT_FLAG_EXECINSTR,
                           ELF_ENTRY_ADDR, //address
                           f->segments_offset, text_size, //file offset, size
                           ELF_SECT_IN_UNDEF, //link
                           0, 16, 0); //info, addr_align, ent_size

   elf_init_section_header(&f->sect_header_entries[ELF_STR_SECTION_INDEX],
                           STR_INDEX_SHSTRLAB, ELF_SECT_TYPE_STRTAB,
                           0, 0, //flag, address
                           f->segments_offset + text_size, //file offset
                           string_table_size, //size
                           ELF_SECT_IN_UNDEF, //link
                           0, 1, 0); //info, addr_align, ent_size


   elf_init_section_header(&f->sect_header_entries[ELF_DATA_SECT_INDEX],
                           STR_INDEX_DATA, ELF_SECT_TYPE_PROGBITS,
                           ELF_SECT_FLAG_ALLOC | ELF_SECT_FLAG_WRITE, //flag
                           ELF_DATA_ADDR, //address
                           //file offset
                           f->segments_offset + text_size + string_table_size,
                           0, //size
                           0,  //link
                           0, 4, 0); //info, addr_align, ent_size

   elf_init_prog_headers(f);

   memcpy(f->mem + f->segments_offset, text_data, text_size);
   memcpy(f->mem + f->segments_offset + text_size,
          string_table_data, string_table_size);

   free(string_table_data);

}



elf_file_t* gen_elf(byte_t* opcodes, int len_opcodes)
{
   elf_file_t* f = malloc(sizeof(elf_file_t));
   f->num_prog_header_entries = 2;
   f->num_sect_header_entries = 4;

   init_sect_prog_tables(f, opcodes, len_opcodes);

   /* *****************************/
   /* start initializing elf data */
   elf_init_header(f->header, ELF_TYPE_EXEC);
   elf_set_header_prog_table(f->header,
                             f->prog_header_offset,
                             ELF32_PROGRAM_HEADER_ENTRY_SIZE,
                             f->num_prog_header_entries);

   elf_set_header_sect_table(f->header,
                             f->sect_header_offset,
                             ELF32_SECTION_HEADER_ENTRY_SIZE,
                             f->num_sect_header_entries);


   //set 0 section here
   //elf_init_section_header(&sect_header_entries[0],

   //elf_init_prog_header(&prog_header_entries[0]);



   elf_set_header_misc(f->header, ELF_ENTRY_ADDR, 0,
                       ELF_STR_SECTION_INDEX);

   //elf_set_header_misc

   //memcpy(elf_mem, &header, ELF32_HEADER_SIZE);
   //memcpy(mem + ELF32_HEADER_SIZE, opcodes, len_opcodes);


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
                                       (mem + elf_file->header->prog_head_off);

   elf_file->sect_header_entries = (elf32_section_header_t*)
                                       (mem + elf_file->header->sect_head_off);


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

void elf_file_free(elf_file_t* f) {

}


