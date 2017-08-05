#include "main.h"
#include "types.h"


void init_lexeme(lexeme_t* lex, char* line, int start, int end) {
   lex->len = end - start;

   lex->str = malloc(lex->len);
   memcpy(lex->str, line+start, lex->len);
}


lexed_line_t* lex(char* file, int *num_lines1) {

   char* stripped = strip(file);

   int num_lines;
   char** lines = split_at_every(stripped, "\n", &num_lines);
   *num_lines1 = num_lines;

   lexed_line_t* ret = malloc(sizeof(lexed_line_t)*num_lines);

   int i; //, j;
   for (i = 0; i < num_lines; i++) {
      lexeme_t* lexemes = NULL; //malloc(sizeof(lexeme_t)*10);
      int num_lexemes = -1;

      char* line = lines[i];
      //char* line_len = strlen(line);

      char* good_line = NULL;

      char** comment_split = split_at(line, ";");
      if (comment_split == NULL)
         good_line = line;
      else {
         good_line = comment_split[0];
         free(comment_split);
         free(comment_split[1]);
         free(line);
      }

      char* bad_line = good_line;
      good_line = strip(bad_line);
      printf("\nbad_line:%s\ngood_line:%s\n", bad_line, good_line);
      free(bad_line);


      //instruction/operands split
      char** instr_op_split = split_at(good_line, " ");
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

         init_lexeme(&lexemes[0], instr, 0, strlen(instr));

         if (op_split == NULL) { //multiple arguments or 1
            init_lexeme(&lexemes[1], ops, 0, strlen(ops));
         }
         else {
            init_lexeme(&lexemes[1], op_split[0], 0, strlen(op_split[0]));
            init_lexeme(&lexemes[2], op_split[1], 0, strlen(op_split[1]));
         }
      }

      ret[i].lexemes = lexemes;
      ret[i].num_lexemes = num_lexemes;

      free(line);
   }

   /*for (i = 0; i < num_lines; i++) {
      free(lines[i];
   }*/
   free(lines);

   return ret;
}


