#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "forth.h"
#include "util.h"
#include "stack.h"
#include "builtins.h"
#include "eval.h"

int forth_eval(Forth *f) {
	size_t i = 0;

	while (i < f->tokens.count) {
		Token *tok = &f->tokens.items[i];
		switch (tok->type) {

		case TOK_WORD: {
			char matched = 0;
			for (size_t n = 0; n < ARRAY_LEN(builtins); n++) {
				if (strcmp(builtins[n].name, tok->as.string) == 0) {
					if (builtins[n].fn(f)) return 1;
					matched = 1;
					i++;
					break;
				}
			}
			if (!matched) {
				for (size_t n = 0; n < f->words.count; n++) {
					if (strcmp(f->words.items[n].name, tok->as.string) == 0) {
						i++;
						cstack_push(f, (cell)i);
						i = f->words.items[n].start + 2;
						matched = 1;
						break;
					}
				}
			}
			if (!matched) {
				for (size_t n = 0; n < f->variables.count; n++) {
					if (strcmp(f->variables.items[n].name, tok->as.string) == 0) {
						if (f->variables.items[n].type == VAR_CONSTANT)
							stack_push(f, *f->variables.items[n].address);
						else {
							stack_push(f, (f->variables.items[n].address - f->varstack));
						}
						i++;
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
			stack_push(f, tok->as.number);
			i++;
		} break;

		case TOK_STRING: {
			printf("%s", tok->as.string);
			i++;
		} break;

		case TOK_COLON: {
			i++;
			Word word = {
				.name = strdup(f->tokens.items[i].as.string),
				.start = tok->as.scope.start,
				.end = tok->as.scope.end,
			};
			da_append(&f->words, word);
			cstack_push(f, word.end + 1);
			i = word.end;
		} break;

		case TOK_IF: {
			if (stack_pop(f)) {
				i++;
			} else {
				if (tok->as.scope.middle) {
					i = tok->as.scope.middle + 1;
				} else {
					i = tok->as.scope.end + 1;
					continue;
				}
			}
			cstack_push(f, tok->as.scope.end + 1);
		} break;

		case TOK_SEMICOLON:
		case TOK_ELSE:
		case TOK_THEN: {
			i = cstack_pop(f);
			if (i + 1>= f->tokens.count) break;
		} break;

		case TOK_DO: {
			tok->as.scope.start = stack_pop(f);
			tok->as.scope.middle = stack_pop(f);
			if (tok->as.scope.start > tok->as.scope.middle) {
				i = tok->as.scope.end + 1;
				continue;
			}
			i++;
		} break;

		case TOK_ITERATOR: {
			stack_push(f, (cell)f->tokens.items[tok->as.scope.start].as.scope.start);
			i++;
		} break;

		case TOK_LOOP: {
			Token *if_token = &f->tokens.items[tok->as.scope.start];
			if_token->as.scope.start++;
			if (if_token->as.scope.start < if_token->as.scope.middle) {
				i = tok->as.scope.start + 1;
				continue;
			}
			i++;
		} break;

		case TOK_BEGIN: {
			i++;
		} break;

		case TOK_UNTIL: {
			stack_pop(f);
			cell val = stack_pop(f);
			stack_push(f, val);
			if (TRUTHY(val) == TRUE) {
				i = tok->as.scope.start;
				continue;
			}
			i++;
		} break;

		case TOK_CONSTANT: {
			cell val = stack_pop(f);
			Token *name = &f->tokens.items[i + 1];
			Variable var = {
				.type = VAR_CONSTANT,
				.name = strdup(name->as.string),
				.address = &val,
			};
			da_append(&f->variables, var);
			i += 2;
		} break;

		case TOK_VARIABLE: {
			Token *name = &f->tokens.items[i + 1];
			Variable var = {
				.type = VAR_VARIABLE,
				.name = strdup(name->as.string),
				.address = f->vp++,
			};
			da_append(&f->variables, var);
			i += 2;
		} break;

		case TOK_READ: {
			cell addr = stack_pop(f);
			stack_push(f, f->varstack[addr]);
			i++;
		} break;

		case TOK_WRITE: {
			cell addr = stack_pop(f);
			cell val = stack_pop(f);
			f->varstack[addr] = val;
			i++;
		} break;

		case TOK_ADD: {
			cell addr = stack_pop(f);
			cell val = stack_pop(f);
			f->varstack[addr] += val;
			i++;
		} break;

		case TOK_CELLS: {
			// ignored
			i++;
		} break;

		case TOK_ALLOT: {
			cell a = stack_pop(f);
			f->vp += a;
			i++;
		} break;

		default: {
			assert(0 && "Unreachable");
		} break;

		}

	}
	return 0;
}

