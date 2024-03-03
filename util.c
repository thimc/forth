#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "util.h"

void to_lower(char *str) {
	for ( ; *str; ++str)
		*str = tolower(*str);
}

char *shift_args(int *argc, char ***argv) {
    assert(*argc > 0);
    char *result = **argv;
    (*argv) += 1;
    (*argc) -= 1;
    return result;
	assert(*argc > 0);
}

void dump_tokens(Tokens *tokens) {
	fprintf(stderr, "%-7s %-15s %-20s %-8s %-8s %-8s\n",
		"Index #", "Token Type", "Value", "Start", "Middle", "End");
	for (size_t i = 0; i < tokens->count; i++) {
		Token *tok = &tokens->items[i];
		fprintf(stderr, "%-7zu %-15s ", i, token_string[tok->type]);
		switch (tok->type) {
		case TOK_NUMBER:
			fprintf(stderr, "%-20d ", tok->as.number);
			break;
		case TOK_COLON:
			fprintf(stderr, "%-20s %-8zu %-8s %-8zu",
				" ", tok->as.scope.start, " ", tok->as.scope.end);
			break;
		case TOK_SEMICOLON:
			fprintf(stderr, "%-20s %-8zu",
				" ", tok->as.scope.start);
			break;
		case TOK_BEGIN:
		case TOK_DO:
			fprintf(stderr, "%-20s %-8s %-8s %-8zu",
				" ", " ", " ", tok->as.scope.end);
			break;
		case TOK_UNTIL:
		case TOK_LOOP:
			fprintf(stderr, "%-20s %-8zu ",
				" ", tok->as.scope.start);
			break;
		case TOK_ITERATOR:
		case TOK_IF:
			fprintf(stderr, "%-20s %-8zu %-8zu %-8zu ",
				" ", tok->as.scope.start, tok->as.scope.middle, tok->as.scope.end);
			break;
		case TOK_ELSE:
			fprintf(stderr, "%-20s %-8zu %-8s %-8zu ",
				" " , tok->as.scope.start, " ", tok->as.scope.end);
			break;
		case TOK_THEN:
			fprintf(stderr, "%-20s %-8zu %-8s %-8s ",
				" ", tok->as.scope.start, " ", " ");
			break;
		case TOK_WORD:
		case TOK_STRING:
			fprintf(stderr, "%-20s ", tok->as.string);
			break;
		default: break;
		}
		fprintf(stderr, "\n");
	}
}
