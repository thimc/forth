#include <stdio.h>

#include "forth.h"
#include "lexer.h"
#include "stack.h"

int forth_parse(Forth *f) {
	f->i = 0;
	char in_function = 0;
	char in_begin_loop = 0;
	size_t i = 0;
	while (f->i < f->tokens.count) {
		switch (f->tokens.items[f->i].type) {

		case TOK_COLON: {
			i = f->i;
			if (in_function) {
				forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i], "cannot define words recursively");
				return 1;
			}
			if (forth_peek_token(f)->type != TOK_WORD) {
				forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i], "expected '%s' after '%s'",
					token_string[TOK_WORD], token_string[forth_peek_token(f)->type]);
				return 1;
			}
			in_function = 1;
			while (f->tokens.items[f->i].type != TOK_SEMICOLON) {
				if (forth_parser_eof(f)) {
					forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i], "expected '%s' after '%s'",
						token_string[TOK_SEMICOLON], token_string[TOK_COLON]);
					return 1;
				}
				f->i++;
			}
			f->tokens.items[i].as.scope.start = i;
			cstack_push(f, i);
			f->i = i + 2;
		} break;

		case TOK_SEMICOLON: {
			i = f->i;
			if (cstack_empty(f)) {
				forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i], "expected '%s' before '%s'",
					token_string[TOK_COLON], token_string[TOK_SEMICOLON]);
				return 1;
			}
			size_t addr = cstack_pop(f);
			f->tokens.items[f->i].as.scope.start = f->tokens.items[addr].as.scope.start;
			f->tokens.items[addr].as.scope.end = f->i;
			in_function = 0;
			f->i++;
		}  break;

		case TOK_DO: {
			i = f->i;
			if (!in_function) {
				forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i],
					"loops are only permitted inside functions");
				return 1;
			}
			while (f->tokens.items[f->i].type != TOK_LOOP &&
					f->tokens.items[f->i].type != TOK_PLUS_LOOP) {
				if (forth_parser_eof(f)) {
					forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i], "expected '%s' or '%s' after '%s'",
						token_string[TOK_LOOP], token_string[TOK_PLUS_LOOP], token_string[TOK_DO]);
					return 1;
				}
				f->i++;
			}
			f->tokens.items[i].as.scope.start = 0;
			cstack_push(f, i);
			f->i = i + 1;
		} break;

		case TOK_ITERATOR: {
			i = f->i;
			if (cstack_empty(f)) {
				forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i], "expected '%s' before '%s'",
					token_string[TOK_DO], token_string[TOK_ITERATOR]);
				return 1;
			}
			size_t addr = cstack_pop(f);
			f->tokens.items[f->i].as.scope.start = addr;
			cstack_push(f, addr);
			f->i = i + 1;
		} break;

		case TOK_PLUS_LOOP:
		case TOK_LOOP: {
			i = f->i;
			if (cstack_empty(f)) {
				forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i], "expected '%s' before '%s'",
					token_string[TOK_DO], token_string[f->tokens.items[f->i].type]);
				return 1;
			}
			size_t addr = cstack_pop(f);
			f->tokens.items[addr].as.scope.end = f->i;
			f->tokens.items[f->i].as.scope.start = addr;
			f->i++;
		} break;

		case TOK_IF: {
			i = f->i;
			if (!in_function) {
				forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i],
					"conditionals are only permitted inside functions");
				return 1;
			}
			while (f->tokens.items[f->i].type != TOK_THEN) {
				if (forth_parser_eof(f)) {
					forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i], "expected '%s' after '%s'",
						token_string[TOK_THEN], token_string[TOK_IF]);
					return 1;
				}
				f->i++;
			}

			f->tokens.items[i].as.scope.start = i;
			cstack_push(f, i);
			f->i = i + 1;
		} break;

		case TOK_ELSE: {
			i = f->i;
			if (!in_function) {
				forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i],
					"conditionals are only permitted inside functions");
				return 1;
			}
			if (cstack_empty(f)) {
				forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i], "expected '%s' before '%s'",
					token_string[TOK_IF], token_string[TOK_ELSE]);
				return 1;
			}
			while (f->tokens.items[f->i].type != TOK_THEN) {
				if (forth_parser_eof(f)) {
					forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i], "expected '%s' after '%s'",
						token_string[TOK_THEN], token_string[TOK_ELSE]);
					return 1;
				}
				f->i++;
			}

			size_t addr = cstack_pop(f);
			cstack_push(f, addr);
			f->tokens.items[addr].as.scope.middle = i;
			f->tokens.items[i].as.scope.start = addr;
			f->i = i + 1;
		} break;

		case TOK_THEN: {
			i = f->i;
			if (!in_function) {
				forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i],
					"conditionals are only permitted inside functions");
				return 1;
			}
			if (cstack_empty(f)) {
				forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i], "expected '%s' before '%s'",
					token_string[TOK_IF], token_string[TOK_THEN]);
				return 1;
			}

			size_t addr = cstack_pop(f);
			f->tokens.items[addr].as.scope.end = f->i;
			f->tokens.items[f->i].as.scope.start = addr;
			f->tokens.items[f->i].as.scope.end = f->i;
			if (f->tokens.items[addr].as.scope.middle) {
				size_t else_addr = f->tokens.items[addr].as.scope.middle;
				f->tokens.items[else_addr].as.scope.end = f->i;
			}
			f->i++;
		} break;

		case TOK_BEGIN: {
			i = f->i;
			if (!in_function) {
				forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i],
					"loops are only permitted inside functions");
				return 1;
			}
			in_begin_loop = 1;
			while (f->tokens.items[f->i].type != TOK_UNTIL) {
				if (forth_parser_eof(f)) {
					forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i], "expected '%s' after '%s'",
						token_string[TOK_UNTIL], token_string[TOK_BEGIN]);
					return 1;
				}
				f->i++;
			}
			f->tokens.items[i].as.scope.start = 0;
			cstack_push(f, i);
			f->i = i + 1;
		} break;

		case TOK_UNTIL: {
			i = f->i;
			if (!in_begin_loop) {
				forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i], "expected '%s' before '%s'",
					token_string[TOK_BEGIN], token_string[f->tokens.items[f->i].type]);
				return 1;
			}
			in_begin_loop = 0;
			if (cstack_empty(f)) {
				forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[i], "expected '%s' before '%s'",
					token_string[TOK_BEGIN], token_string[f->tokens.items[f->i].type]);
				return 1;
			}
			size_t addr = cstack_pop(f);
			f->tokens.items[addr].as.scope.end = f->i;
			f->tokens.items[f->i].as.scope.start = addr;
			f->i++;
		} break;

		default: {
			f->i++;
		} break;

		}
	}
	return 0;
}
