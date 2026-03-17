#include <argp.h>

#define MAX_IF_NAME_LEN 512u

const char * argp_program_version = "version 0.5";
const char * argp_program_bug_address = "jxb@bush-tech.net";

struct ula_options {
   char iface_name[MAX_IF_NAME_LEN];
};

struct argp ula_argp {

};

static int parse_opt(int key, char * arg, struct argp_state * state);

int ula_arg_parser(int argc, char ** argv, struct ula_options * opts);


