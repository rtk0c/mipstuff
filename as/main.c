#include "parse.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	// We don't do any cleanup here, because when main() exits the OS takes care of the rest

	if (argc < 1) {
		fprintf(stderr, "Expected input filename argument at 1.\n");
		return -1;
	}

	FILE* f = fopen(argv[1], "r");
	fseek(f, 0, SEEK_END);
	long src_len = ftell(f);
	rewind(f);
	char* src_buf = malloc(src_len + 1);
	fread(src_buf, sizeof(char), src_len, f);
	src_buf[src_len] = '\0';
	fclose(f);

	MSparsestate ps;
	ps.src = src_buf;
	ps.src_len = src_len;
	MS_init_compile(&ps);

	// TODO

	MS_finish_compile(&ps);

	return 0;
}
