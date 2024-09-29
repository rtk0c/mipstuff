#include "emit.h"
#include "parse.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char* source_file;
	char* output_file;
} CliArgs;

char** find_cli_arg_slot(CliArgs* cli, const char* arg, const char* arg_end) {
	size_t arg_len = arg_end - arg;
#define SHORT_STYLE(c) (arg_len == 1 && arg[0] == c)
#define LONG_STYLE(name) (strncmp("-" name, arg, arg_len) == 0)

	if (SHORT_STYLE('i') || LONG_STYLE("input"))
		return &cli->source_file;
	if (SHORT_STYLE('o') || LONG_STYLE("output"))
		return &cli->output_file;

	fprintf(stderr, "Invalid argument %.*s\n", (int)(arg_end - arg), arg);
	exit(-1);

#undef SHORT_STYLE
#undef LONG_STYLE
}

void parse_cli_args(CliArgs* cli, int argc, char** argv) {
	char** named_value_slot = NULL;

	for (int argi = 0; argi < argc; ++argi) {
		char* arg = argv[argi];
		size_t arg_len = strlen(arg);

		if (named_value_slot) {
			*named_value_slot = arg;
			continue;
		}
		if (arg_len > 1 && arg[0] == '-') {
			// Named argument like --name
			char* key_part = arg + 1;
			char* equal_sign = strchr(arg, '=');
			if (equal_sign) {
				// Write directly into the slot
				char* value_part = equal_sign + 1;
				*find_cli_arg_slot(cli, key_part, equal_sign) = value_part;
			} else {
				// No value_part here, next argument gets put into named_value_slot
				named_value_slot = find_cli_arg_slot(cli, arg + 1, arg + arg_len);
			}
			continue;
		}

		printf("Warning: unused argument %s\n", arg);
	}
}

int main(int argc, char** argv) {
	// We don't do any cleanup here, because when main() exits the OS takes care of the rest

	CliArgs cli = {0};
	parse_cli_args(&cli, argc, argv);

	char* source_code;
	long source_code_len;
	{
		FILE* f = fopen(argv[1], "r");

		fseek(f, 0, SEEK_END);
		source_code_len = ftell(f);

		source_code = malloc(source_code_len + 1);
		rewind(f);
		fread(source_code, sizeof(char), source_code_len, f);
		source_code[source_code_len] = '\0';

		fclose(f);
	}

	MSparsestate ps;
	ps.src = source_code;
	ps.src_len = source_code_len;
	MS_init_compile(&ps);

	// TODO

	MS_finish_compile(&ps);

	return 0;
}
