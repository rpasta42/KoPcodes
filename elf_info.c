#include "elf.h"


void elf_file_print_debug(elf_file_t* elf_file) {
   elf_print_header(elf_file->header);
   elf_print_sect_table(elf_file);
   elf_print_prog_table(elf_file);
   elf_print_string_table(elf_file);
}


void elf_print_prog_table(elf_file_t* elf_file) {

   elf32_program_header_t* prog_header = elf_file->prog_header_entries;

   uint32_t prog_entry_num = elf_file->header->prog_head_num;

   for (int i = 0; i < prog_entry_num; i++) {
      printf("\nprog entry");
   }

}



char* print_sect_type(uint32_t sect_type) {
   switch (sect_type) {
      case ELF_SECT_TYPE_NULL:
         return ("ELF_SECT_TYPE_NULL");
         break;
      case ELF_SECT_TYPE_PROGBITS:
         return ("ELF_SECT_TYPE_PROGBITS");
         break;
      case ELF_SECT_TYPE_SYMTAB:
         return ("ELF_SECT_TYPE_SYMTAB");
         break;
      case ELF_SECT_TYPE_STRTAB:
         return ("ELF_SECT_TYPE_STRTAB");
         break;
      case ELF_SECT_TYPE_RELA:
         return ("ELF_SECT_TYPE_RELA");
         break;
      case ELF_SECT_TYPE_HASH:
         return ("ELF_SECT_TYPE_HASH");
         break;
      case ELF_SECT_TYPE_DYNAMIC:
         return ("ELF_SECT_TYPE_DYNAMIC");
         break;
      case ELF_SECT_TYPE_NOTE:
         return ("ELF_SECT_TYPE_NOTE");
         break;
      case ELF_SECT_TYPE_NOBITS:
         return ("ELF_SECT_TYPE_NOBITS");
         break;
      case ELF_SECT_TYPE_REL:
         return ("ELF_SECT_TYPE_REL");
         break;
      case ELF_SECT_TYPE_SHLIB:
         return ("ELF_SECT_TYPE_SHLIB");
         break;
      case ELF_SECT_TYPE_DYNSYM:
         return ("ELF_SECT_TYPE_DYNSYM");
         break;
      case ELF_SECT_TYPE_LOPROC:
         return ("ELF_SECT_TYPE_LOPROC");
         break;
      case ELF_SECT_TYPE_HIPROC:
         return ("ELF_SECT_TYPE_HIPROC");
         break;
      case ELF_SECT_TYPE_LOUSER:
         return ("ELF_SECT_TYPE_LOUSER");
         break;
      case ELF_SECT_TYPE_HIUSER:
         return ("ELF_SECT_TYPE_HIUSER");
         break;
      default:
         printf("\tunknown sect type: %i", sect_type);
         return false; //goto elf_print_sect_err;
         break;
   }

   return true;
}


void elf_print_sect_table(elf_file_t* elf_file) {
   elf32_header_t* header = elf_file->header;

   elf32_section_header_t* sect_header = elf_file->sect_header_entries;

   //number of entries
   uint32_t sect_entry_num = elf_file->header->sect_head_num;

   printf("\nSection Headers:\n");
         //#,  Name,   Type,    Addr,     Offset,  Size,   Flags
   printf("#  %-20.20s %-25.25s %-10.10s %-10.10s %-10.10s wax",
          "Name", "Type", "Addr", "Offset", "Size");
   printf("\t%-7.7s %-7.7s %-10.10s %-7.7s",
          "Link", "Info", "AdrAlign", "EntSize");

   for (int i = 0; i < sect_entry_num; i++) {
      printf("\n%i  ", i);

      elf32_section_header_t* sect = &sect_header[i];

      uint32_t sect_name = sect->sect_name;
      if (sect_name == 0)
         printf("%-20.20s", "NO_SECT_NAME");
      else
         printf("%-20.20s", &elf_file->str_sect_strings_p[sect_name]);

      char* sect_type_ok = print_sect_type(sect->sect_type);
      if (!sect_type_ok)
         goto elf_print_sect_err;
      printf(" %-25.25s", sect_type_ok);


      printf(" %08X   ", sect->sect_addr);
      printf("%08X   ", sect->sect_offset);

      printf("%08X   ", sect->sect_size);


      printf("%i%i%i",
               ((sect->sect_flags & ELF_SECT_FLAG_WRITE) > 0),
               ((sect->sect_flags & ELF_SECT_FLAG_ALLOC) > 0),
               ((sect->sect_flags & ELF_SECT_FLAG_EXECINSTR) > 0));



      printf("\t%i\t%i\t%i\t    %i",
             sect->sect_link, sect->sect_info,
             sect->sect_addr_align, sect->sect_ent_size,
             sect->sect_ent_size);

   }



   elf_print_sect_err:
   printf("\nerror!\n");
   return;
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


void elf_print_string_table(elf_file_t* f) {
   elf32_header_t* header = f->header;
   int strings_len = f->str_sect_strings_len;

   printf("\n\nstring table len: %i", strings_len);

   printf("\nstring table id: %i\nstrings table data: ",
          header->sect_head_section_name);
   char* strings = f->str_sect_strings_p;
   for (int i = 0; i < strings_len; i++) {
      printf("%c", strings[i]);
   }
   printf("\n");

}


