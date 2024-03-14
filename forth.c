#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "forth.h"
#include "stack.h"
#include "lexer.h"
#include "parser.h"
#include "eval.h"

void forth_init(Forth *f, const char *file_name, char *source_code) {
	f->stack = (cell*)(&f->memory[DATA_OFFSET + CELL_SIZE]);
	f->cstack = (cell*)(&f->memory[CALLSTACK_OFFSET + CELL_SIZE]);
	f->varstack = (cell*)(&f->memory[VARIABLES_OFFSET]);
	f->sp = (cell*)(&f->memory[DATA_OFFSET]);
	f->csp = (cell*)(&f->memory[CALLSTACK_OFFSET]);
	f->vp = f->varstack;
	f->file_name = file_name;
	f->source_code = strdup(source_code);
}

void forth_free(Forth *f) {
	for (size_t i = 0; i < f->tokens.count; i++) {
		if (f->tokens.items[i].type == TOK_STRING) {
			free(f->tokens.items[i].as.string);
		}
	}
	for (size_t i = 0; i < f->words.count; i++) {
		free(f->words.items[i].name);
	}
	for (size_t i = 0; i < f->variables.count; i++) {
		free(f->variables.items[i].name);
	}
	if (f->words.count) {
		da_free(f->words);
	}
	if (f->tokens.count) {
		da_free(f->tokens);
	}
	if (f->variables.count) {
		da_free(f->variables);
	}
	free(f->source_code);
}

int forth_run(Forth *f) {
	if (forth_lex(f)) goto error;
	if (forth_parse(f)) goto error;
	if (forth_eval(f)) goto error;
	return 0;
error:
	return 1;
}

int main(int argc, char *argv[]) {
	Forth forth = {0};

	const char *program = shift_args(&argc, &argv);
	if (argc < 1) {
		fprintf(stderr, "usage: %s [-v] <file>\n", program);
		exit(1);
	}
	const char *file_name = shift_args(&argc, &argv);

	char *source_code = read_file(file_name);
	if (source_code == NULL) {
		fprintf(stderr, "ERROR: could not read file: '%s'\n", file_name);
		goto error;
	}
	forth_init(&forth, file_name, source_code);

	if(forth_run(&forth)) {
		fprintf(stderr, "ERROR: Execution failed\n");
		goto error;
	}

error:
	forth_free(&forth);
	free(source_code);

	return 0;
}
