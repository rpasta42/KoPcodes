#include "main.h"
#include "types.h"


void init_lexeme(lexeme_t* lex, char* line, int start, int len) {

   lex->str = malloc(len+1);
   memcpy(lex->str, line+start, len);

   lex->len = len;
}


lexed_line_t* lex(char* file, int *num_lines1) {

   char* stripped = strip(file);
   printf("\n*****\n%s\n******\n", stripped);

   int num_lines;
   char** lines = split_at_every(stripped, "\n", &num_lines);
   *num_lines1 = num_lines;

   lexed_line_t* ret = malloc(sizeof(lexed_line_t)*num_lines);

   DEBUG

   int i; //, j;
   for (i = 0; i < num_lines; i++) {

      lexeme_t* lexemes = NULL; //malloc(sizeof(lexeme_t)*10);
      int num_lexemes = -1;

      char* line = lines[i];
      //char* line_len = strlen(line);

      char* good_line = NULL;

      bool have_comment = false;
      char** comment_split = split_at(line, ";");
      if (comment_split == NULL)
         good_line = line;
      else {
         have_comment = true;
         good_line = comment_split[0];
         //printf("\ncomment split:%s\n", good_line);
         free(comment_split);
         free(comment_split[1]);
         free(line);
      }

      //printf("\ngood_line: %s\t good line len:%i\n", good_line, strlen(good_line));

      char* bad_line = good_line;
      good_line = strip(bad_line);
      //printf("\nbad_line %i:%s\ngood_line %i:%s\n", strlen(bad_line), bad_line, strlen(good_line), good_line);
      free(bad_line);


      //instruction/operands split
      char** instr_op_split = split_at(good_line, " ");

      //printf("\nhave comment:%i, instr_op_split:%i\n", have_comment, instr_op_split);

      if (instr_op_split == NULL) {
         num_lexemes = 1;
         lexemes = malloc(sizeof(lexeme_t));
         init_lexeme(&lexemes[0], good_line, 0, strlen(good_line));
      }
      else {

         char* instr = instr_op_split[0];
         char* ops = instr_op_split[1];

         char** op_split = split_at(ops, ",");

         if (op_split == NULL)
            num_lexemes = 2;
         else
            num_lexemes = 3;
         lexemes = malloc(sizeof(lexeme_t)*num_lexemes);

         init_lexeme(&lexemes[0], instr, 0, strlen(instr)); //instruction

         if (op_split == NULL) { //multiple arguments or 1
            ops = strip(ops);
            init_lexeme(&lexemes[1], ops, 0, strlen(ops));
         }
         else {
            op_split[0] = strip(op_split[0]);
            op_split[1] = strip(op_split[1]);

            init_lexeme(&lexemes[1], op_split[0], 0, strlen(op_split[0]));
            init_lexeme(&lexemes[2], op_split[1], 0, strlen(op_split[1]));
         }
      }

      ret[i].lexemes = lexemes;
      ret[i].num_lexemes = num_lexemes;

      //free(line);
   }

   DEBUG
   /*for (i = 0; i < num_lines; i++) {
      free(lines[i];
   }*/
   free(lines);

   return ret;
}


char* instr_names_str[] = {
   "add",
   "and",
   "call",
   "cmp",
   "dec",
   "div",
   "inc",
   "int",
   "ja",
   "jae",
   "jb",
   "jbe",
   "jcxz",
   "je",
   "jg",
   "jge",
   "jl",
   "jle",
   "jmp",
   "loop",
   "mov",
   "mul",
   "or",
   "pop",
   "push",
   "sub"
};

char* reg_names_str[] = {
   "eax",
   "ebx",
   "ecx",
   "edx",
};

#define NUM_INSTRUCTS OpSub + 1
#define NUM_REGS RegEdx + 1

instr_t* gen_instructions(lexed_line_t* lines, int num_lines) {

   instr_t* instructs = malloc(sizeof(instr_t)*num_lines);


   int i, j, k;
   for (i = 0; i < num_lines; i++) {
      lexed_line_t* line = &lines[i];

      lexeme_t* lexemes = line->lexemes;
      int num_lexemes = line->num_lexemes;


      bool have_instr_name = false;
      bool have_fst_arg = false;
      bool have_snd_arg = false;
      for (j = 0; j < num_lexemes; j++) {
         lexeme_t* lexeme = &lexemes[j];
         char* str_lex = lexeme->str;

         if (!have_instr_name) {

            for (k = 0; k < NUM_INSTRUCTS; k++) {
               char* test_name = instr_names_str[k];
               int test_name_len = strlen(test_name);
               int cmp_res = strncmp(str_lex, test_name, test_name_len);
               if (cmp_res == 0) {
                  instructs[i].name = k;
                  have_instr_name = true;
                  break;
               }
            }

            if (!have_instr_name) {
               printf("\ncouldn't match instruction to opcode: %s", lexeme->str);
               goto bad;
            }

            continue;
         }
         else if (!have_fst_arg) {

            //first test if it's a register
            for (k = 0; k < NUM_REGS; k++) {
               char* test_name = reg_names_str[k];
               int test_name_len = strlen(test_name);
               int cmp_res = strncmp(str_lex, test_name, test_name_len);
               if (cmp_res == 0) {
                  instructs[i].arg1_t = ArgReg32;
                  instructs[i].arg1_v.reg_index = k;
                  have_fst_arg = true;
               }
            }

            if (have_fst_arg)
               continue;

            if (str_lex[0] == '0' && str_lex[1] == 'x') {
               //..
               instructs[i].arg1_t = ArgConst;

               int hex_num = parse_hex_num(str_lex+2);

               if (hex_num == 0xDEADBAEF) {
                  printf("couldn't parse hex num: %s", str_lex);
                  goto bad;
               }

               instructs[i].arg1_v.const_num = hex_num;

               have_fst_arg = true;
            }

            if (have_fst_arg)
               continue;

            if (is_digit(str_lex[0])) {
               instructs[i].arg1_t = ArgConst;

               int human_num = parse_human_num(str_lex);
               if (human_num == 0xDEADBAEF) {
                  printf("couldn't parse number: %s", str_lex);
                  goto bad;
               }

               instructs[i].arg1_v.const_num = human_num;

               //..
               have_fst_arg = true;
            }

            if (!have_fst_arg) {
               printf("\ncouldn't parse first argument: %s", lexeme->str);
               goto bad;
            }
            continue;

         }
         else if (!have_snd_arg) {


            //first test if it's a register
            for (k = 0; k < NUM_REGS; k++) {
               char* test_name = reg_names_str[k];
               int test_name_len = strlen(test_name);
               int cmp_res = strncmp(str_lex, test_name, test_name_len);
               if (cmp_res == 0) {
                  instructs[i].arg2_t = ArgReg32;
                  instructs[i].arg2_v.reg_index = k;
                  have_fst_arg = true;
               }
            }

            if (have_snd_arg)
               continue;

            if (str_lex[0] == '0' && str_lex[1] == 'x') {
               //..
               instructs[i].arg2_t = ArgConst;

               int hex_num = parse_hex_num(str_lex+2);

               if (hex_num == 0xDEADBAEF) {
                  printf("couldn't parse hex num: %s", str_lex);
                  goto bad;
               }

               instructs[i].arg2_v.const_num = hex_num;

               have_snd_arg = true;
            }

            if (have_snd_arg)
               continue;

            if (is_digit(str_lex[0])) {
               instructs[i].arg2_t = ArgConst;

               int human_num = parse_human_num(str_lex);
               if (human_num == 0xDEADBAEF) {
                  printf("couldn't parse number: %s", str_lex);
                  goto bad;
               }

               instructs[i].arg2_v.const_num = human_num;

               //..
               have_snd_arg = true;
            }
            printf("%s", str_lex);

            if (!have_snd_arg) {
               printf("\ncouldn't parse second argument: %s", str_lex);
               goto bad;
            }
            continue;

         }
         else {
            printf("\ntoo many arguments passed to instruction\n");
            goto bad;
         }


      }
   }


   return instructs;


   bad:;
   free(instructs);
   return NULL;

}

void print_instructs(instr_t* instructs, int num_instructs) {
   int i;
   int j;
   for (i = 0; i < num_instructs; i++) {
      instr_t* instruct = &instructs[i];

      printf("\n%s ", instr_names_str[instruct->name]);

      j = 0;
      while (j++ < 2) {
         instr_arg_type_t arg_t;
         instr_arg_value_t arg_v;
         if (j == 1) {
            arg_t = instruct->arg1_t;
            arg_v = instruct->arg1_v;
         } else if (j == 2) {
            arg_t = instruct->arg2_t;
            arg_v = instruct->arg2_v;
         }

         switch (arg_t) {
            case ArgNone:
               printf("(none) ");
               break;
            case ArgReg32:
               printf("(reg)%s ", reg_names_str[arg_v.reg_index]);
               break;
            case ArgConst:
               printf("#%i ", arg_v.const_num);
               break;
         }
      }
   }
}

