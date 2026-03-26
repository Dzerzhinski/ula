#include <argp.h>

#define MAX_IF_NAME_LEN 512u

const char * argp_program_version = "version 0.5";
const char * argp_program_bug_address = "jxb@bush-tech.net";

struct ula_opts{
   char iface_name[MAX_IF_NAME_LEN];
};

struct argp_options ula_options[] = {
   {"iface", 'i', "INTERFACE", 0, "Network interface with MAC to use"}, 
   {0}
};

struct argp ula_argp {

};

static int ula_parser(int key, char * arg, struct argp_state * state);

int ula_arg_parser(int argc, char ** argv, struct ula_options * opts);


