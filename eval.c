#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "stack.h"
#include "util.h"
#include "eval.h"
#include "builtins.h"

int eval(Tokens *tokens, Words *words) {
	size_t i = 0;

	while (i < tokens->count) {
		Token *tok = &tokens->items[i];

		/* fprintf(stderr, "Evaluating #%zu (%s ", */
		/* 	i, token_string[tokens->items[i].type]); */
		/* if (tokens->items[i].type <= TOK_LSTRING) */
		/* 	fprintf(stderr, "%s)\t", tokens->items[i].as.string); */
		/* else if (tokens->items[i].type == TOK_NUMBER) */
		/* 	fprintf(stderr, "%d)\t", tokens->items[i].as.number); */
		/* else */
		/* 	fprintf(stderr, ")\t"); */

		/* fprintf(stderr, "\t SP = %d , peek %-4d", SP, STACK[SP]); */
		/* fprintf(stderr, "\tCSP = %d , peek %-4d", CSP, CALL_STACK[CSP]); */
		/* fprintf(stderr, "\tISP = %d , peek %-4d\n", ISP, ITERATOR_STACK[ISP]); */
		/* for (int s = 0; s < SP; s++) { */
		/* 	fprintf(stderr, "%d ", STACK[s]); */
		/* } */
		/* fprintf(stderr, "\n\n"); */

		switch (tok->type) {
		case TOK_WORD: {
			char matched = 0;
			for (size_t n = 0; n < ARRAY_LEN(builtins); n++) {
				if (strcmp(builtins[n].name, tok->as.string) == 0) {
					if (builtins[n].fn()) return 1;
					matched = 1;
					i++;
					break;
				}
			}
			if (!matched) {
				for (size_t n = 0; n < words->count; n++) {
					if (strcmp(words->items[n].name, tok->as.string) == 0) {
						CALL_STACK_PUSH(++i);
						i = words->items[n].start + 2;
						matched = 1;
						break;
					}
				}
			}
			if (!matched) {
				printf("Unknown word: '%s' at index %zu\n", tok->as.string, i);
				return 1;
			}
		} break;

		case TOK_NUMBER: {
			STACK_PUSH(tok->as.number);
			i++;
		} break;

		case TOK_LSTRING: {
			printf("%s", tok->as.string);
			i++;
		} break;

		case TOK_COLON: {
			i++;
			Word word = {
				.name = strdup(tokens->items[i].as.string),
				.start = tok->as.scope.start,
				.end = tok->as.scope.end,
			};
			da_append(words, word);
			CALL_STACK_PUSH(word.end + 1);
			i = word.end;
		} break;

		case TOK_IF: {
			if (STACK_POP()) {
				i++;
			} else {
				if (tok->as.scope.middle) {
					i = tok->as.scope.middle + 1;
				} else {
					i = tok->as.scope.end + 1;
					continue;
				}
			}
			CALL_STACK_PUSH(tok->as.scope.end + 1);
		} break;

		case TOK_SEMICOLON:
		case TOK_ELSE:
		case TOK_THEN: {
			i = CALL_STACK_POP();
			if (i >= tokens->count) break;
		} break;

		case TOK_DO: {
			int start = STACK_POP();
			int end = STACK_POP();
			if (start + 1 >= end) {
				i = tok->as.scope.end + 1;
				continue;
			}
			ITERATOR_STACK_PUSH(end);
			ITERATOR_STACK_PUSH(start);
			i++;
		} break;

		case TOK_ITERATOR: {
			int start = ITERATOR_STACK_POP();
			int end = ITERATOR_STACK_POP();
			STACK_PUSH(start);
			ITERATOR_STACK_PUSH(end);
			ITERATOR_STACK_PUSH(start);
			i++;
		} break;

		case TOK_LOOP: {
			int start = ITERATOR_STACK_POP() + 1;
			int end = ITERATOR_STACK_POP();
			if (start < end) {
				ITERATOR_STACK_PUSH(end);
				ITERATOR_STACK_PUSH(start);
				i = tok->as.scope.start + 1;
				continue;
			}
			i++;
		} break;

		default: {
			printf("\nTODO: implement %s\n", token_string[tok->type]);
			assert(0);
		} break;

		}
	}

	return 0;
}
