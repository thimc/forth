#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "stack.h"
#include "util.h"

void run(char *src) {
	Tokens tokens = {0};
	if (lex(src, &tokens)) goto cleanup;
	dump_tokens(&tokens);

cleanup:
	for (size_t i = 0; i < tokens.count; i++) {
		Token *tok = &tokens.items[i];
		if (tok->type <= TOK_LSTRING) {
			free(tokens.items[i].as.string);
		}
	}
	if (tokens.items) da_free(tokens);
}

char *read_file(const char *path) {
	FILE *fp = fopen(path, "r");
	if (fp == NULL) goto error;
	if (fseek(fp, 0, SEEK_END) < 0) goto error;
	size_t file_size = ftell(fp);
	if (fseek(fp, 0, SEEK_SET) < 0) goto error;
	char *file_contents = malloc(sizeof(file_contents) * file_size);
	if (file_contents == NULL) goto error;
	fread(file_contents, sizeof(file_contents), file_size, fp);
	file_contents[file_size] = '\0';
	fclose(fp);
	fprintf(stderr, "%s: %zu bytes\n", path, file_size);
	return file_contents;

error:
	return NULL;
}

void usage(const char *program) {
	fprintf(stderr, "usage: %s <file>\n", program);
	exit(1);
}

int main(int argc, char *argv[]) {
	const char *program = shift_args(&argc, &argv);

	if (argc < 1) usage(program);
	const char *arg = shift_args(&argc, &argv);
	char *program_contents = read_file(arg);
	if (program == NULL) {
		fprintf(stderr, "ERROR: could not read file: '%s'\n", arg);
		return 1;
	}
	run(program_contents);
	free(program_contents);

	return 0;
}

