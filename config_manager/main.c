#include "config_parser.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
    	printf("Error: File path argument is required.\n");
    	return 1;
    }

    char *filepath = argv[1];
    ConfigWrapper *wrapper;
    int err = config_parser_parse_config(filepath, &wrapper);
    config_parser_print_config(err, wrapper);
}
