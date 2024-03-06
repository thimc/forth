#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "stack.h"
#include "util.h"
#include "eval.h"

int STACK[STACK_SIZE] = {0};
int SP = 0, CSP = 0;

int verbose = 0;

void run(const char *file_name, char *source_code) {
	Tokens tokens = {0};
	Words words = {0};
	if (lex(source_code, &tokens)) goto cleanup;
	Parser parser = {
		.fname = file_name,
		.src = source_code,
		.tokens = &tokens,
	};
	if (parse(&parser)) goto cleanup;
	if (verbose) dump_tokens(&tokens);
	if (eval(&tokens, &words)) goto cleanup;

cleanup:
	for (size_t i = 0; i < tokens.count; i++) {
		if (tokens.items[i].type <= TOK_STRING) {
			free(tokens.items[i].as.string);
		}
	}
	for (size_t i = 0; i < words.count; i++) {
		free(words.items[i].name);
	}
	da_free(words);
	da_free(tokens);
}

char *read_file(const char *path) {
	FILE *fp = fopen(path, "r");
	if (fp == NULL) return NULL;
	if (fseek(fp, 0, SEEK_END) < 0) goto error;
	size_t file_size = ftell(fp);
	if (fseek(fp, 0, SEEK_SET) < 0) goto error;
	char *file_contents = malloc(sizeof(file_contents) * file_size);
	if (file_contents == NULL) goto error;
	fread(file_contents, sizeof(file_contents), file_size, fp);
	file_contents[file_size] = '\0';
	fclose(fp);
	return file_contents;

error:
	fclose(fp);
	return NULL;
}

void usage(const char *program) {
	fprintf(stderr, "usage: %s [-v] <file>\n", program);
	exit(1);
}

int main(int argc, char *argv[]) {
	const char *program = shift_args(&argc, &argv);

	if (argc < 1) usage(program);
	const char *arg = shift_args(&argc, &argv);

	if (strcmp(arg, "-vv") == 0) {
		verbose = 2;
		arg = shift_args(&argc, &argv);
	} else if (strcmp(arg, "-v") == 0) {
		verbose = 1;
		arg = shift_args(&argc, &argv);
	}

	char *source_code = read_file(arg);
	if (source_code == NULL) {
		fprintf(stderr, "ERROR: could not read file: '%s'\n", arg);
		return 1;
	}
	run(arg, source_code);
	free(source_code);

	return 0;
}
