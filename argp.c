#include <stdio.h>
#include <argp.h>
#include <stdlib.h>
#include <argz.h>

const char * argp_program_bug_address = "jxb@example.com";
const char * argp_program_version = "version 1.0";

struct arguments {
   char * argz;
   size_t argz_len;
};

static int parse_opt(int key, char *arg, struct argp_state *state)
{
   struct arguments *a = state->input;
   switch(key) {
      case 'd': 
         unsigned int dots = 1;
         // printf("%s", arg);
         if(arg != NULL) {
            dots = atoi(arg);
         }
         for(int i = 0; i < dots; i++) {
            printf(".");
         }
         printf("\n");
         break;
      case 888:
         printf("-");
         break;
      case 999:
         parse_opt('d', "3", state);
         // printf(" ");
         for(int i = 0; i < 3; i++) {
            parse_opt(888, NULL, state);
         }
         printf("\n");
         parse_opt('d', "3", state);
         printf("\n");
         exit(0);
         break;
      case ARGP_KEY_ARG: 
         argz_add(&a->argz, &a->argz_len, arg);
         break;
      case ARGP_KEY_INIT:
         a->argz = 0;
         a->argz_len = 0;
         break;
      case ARGP_KEY_END:
         {
            size_t count = argz_count(a->argz, a->argz_len);
            if(count > 2) {
               argp_failure(state, 1, 0, "too many arguments");
            }
            else if(count < 1) {
               argp_failure(state, 1, 0, "too few arguments");
            }
         }
         break;
   }

   return 0;
}

int main(int argc, char **argv)
{
   struct argp_option options[] = {
         {0, 0, 0, 0, "Morse Code Options:", 7},
         {"dot", 'd', "NUM", OPTION_ARG_OPTIONAL, "Show a dot on the screen"}, 
         {"dash", 888, 0, 0, "Show a dash on the screen"},
         {0, 0, 0, 0, "Informational Options:", -1},
         {"SOS", 999, 0, 0, "Give some help in morse code"},
         {0}
   };
   struct argp argp = {options, parse_opt, "WORD\nWORD WORD", 
      "Show some dots and dashes on the screen\v"
      "A final newline is also shown regardless of whether any options were given"};

   struct arguments arguments;
   if(argp_parse(&argp, argc, argv, 0, 0, &arguments) == 0) {
      const char *prev = NULL;
      char * word;
      while((word = argz_next(arguments.argz, arguments.argz_len, prev))) {
         printf("%s", word);
         prev = word;
      }
      printf("\n");
      free(arguments.argz);
   }
   
   return 0;
}

