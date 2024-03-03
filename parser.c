#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "stack.h"
#include "util.h"

Token *current_token(Parser *parser) {
	return &parser->tokens->items[parser->i];
}

Token *peek_token(Parser *parser) {
	if (parser_eof(parser)) return NULL;
	return &parser->tokens->items[parser->i + 1];
}

void error(char *src, const char *fname, Token *token, const char *fmt, ...) {
    va_list ap;
	char location[64] = {0};

	char *src_line = NULL;
	int line = 0, column = 0;
	size_t i = 0;
	size_t src_len = strlen(src);
	size_t line_start = 0;

	while (i < src_len) {
		column++;
		if (src[i] == '\n') {
			line_start = i + 1;
			line++;
			column = 1;
		}
		if (i == token->location) {
			while (src[i] != '\0' && src[i] != '\n') i++;
			break;
		}
		i++;
	}
	assert(line_start < i);

	src_line = (char*)malloc(i - line_start + 1);
	assert(src_line != NULL);
	memcpy(src_line, src + line_start, i - line_start);
	src_line[i - line_start] = '\0';

    va_start(ap, fmt);
	snprintf(location, ARRAY_LEN(location), "%s:%d:%d: ERROR: ", fname, line, column);
	fputs(location, stderr);
	vfprintf(stderr, fmt, ap);
	printf("\n%s\n", src_line);
	int offset = strlen(src_line) - 1;
    printf("%*s^ near here\n", offset, " ");
    va_end(ap);
    free(src_line);
}

int parser_eof(Parser *parser) {
	return parser->i >= parser->tokens->count;
}

int parse(Parser *parser) {
	parser->i = 0;
	char in_function = 0;
	char in_do_loop = 0;
	char in_begin_loop = 0;
	Token *tok = NULL;

	while (parser-> i < parser->tokens->count) {
		switch (parser->tokens->items[parser->i].type) {

		case TOK_COLON: {
			if (in_function) {
				error(parser->src, parser->fname, tok, "cannot define functions recursively");
				return 1;
			}
			size_t i = parser->i;
			if (peek_token(parser)->type != TOK_WORD) {
				error(parser->src, parser->fname, tok, "expected '%s' after '%s'",
					token_string[TOK_WORD], token_string[peek_token(parser)->type]);
				return 1;
			}
			in_function = 1;
			while (parser->tokens->items[parser->i].type != TOK_SEMICOLON) {
				if (parser_eof(parser)) {
					error(parser->src, parser->fname, tok, "expected '%s' after '%s'",
						token_string[TOK_SEMICOLON], token_string[TOK_COLON]);
					return 1;
				}
				parser->i++;
			}
			parser->tokens->items[i].as.scope.start = i;
			CALL_STACK_PUSH(i);
			parser->i = i + 2;
		} break;

		case TOK_SEMICOLON: {
			if (CALL_STACK_EMPTY()) {
				error(parser->src, parser->fname, tok, "expected '%s' before '%s'",
					token_string[TOK_COLON], token_string[TOK_SEMICOLON]);
				return 1;
			}
			size_t addr = CALL_STACK_POP();
			parser->tokens->items[parser->i].as.scope.start = parser->tokens->items[addr].as.scope.start;
			parser->tokens->items[addr].as.scope.end = parser->i;
			in_function = 0;
			parser->i++;
		}  break;

		case TOK_DO: {
			if (!in_function) {
				error(parser->src, parser->fname, tok,
					"loops are only permitted inside functions");
				return 1;
			}
			in_do_loop = 1;
			size_t i = parser->i;
			while (parser->tokens->items[parser->i].type != TOK_LOOP &&
					parser->tokens->items[parser->i].type != TOK_PLUS_LOOP) {
				if (parser_eof(parser)) {
					error(parser->src, parser->fname, tok, "expected '%s' or '%s' after '%s'",
						token_string[TOK_LOOP], token_string[TOK_PLUS_LOOP], token_string[TOK_DO]);
					return 1;
				}
				parser->i++;
			}
			parser->tokens->items[i].as.scope.start = 0;
			CALL_STACK_PUSH(i);
			parser->i = i + 1;
		} break;

		case TOK_ITERATOR: {
			if (!in_do_loop) {
				error(parser->src, parser->fname, tok,
					"iterators are only permitted inside loops");
				return 1;
			}
			size_t i = parser->i;
			if (CALL_STACK_EMPTY()) {
				error(parser->src, parser->fname, tok, "expected '%s' before '%s'",
					token_string[TOK_DO], token_string[TOK_ITERATOR]);
				return 1;
			}
			size_t addr = CALL_STACK_POP();
			parser->tokens->items[parser->i].as.scope.start = addr;
			CALL_STACK_PUSH(addr);
			parser->i = i + 1;
		} break;

		case TOK_PLUS_LOOP:
		case TOK_LOOP: {
			if (!in_do_loop) {
			 	error(parser->src, parser->fname, tok, "expected '%s' before '%s'",
					token_string[TOK_DO], token_string[parser->tokens->items[parser->i].type]);
			 	return 1;
			}
			in_do_loop = 0;
			if (CALL_STACK_EMPTY()) {
				error(parser->src, parser->fname, tok, "expected '%s' before '%s'",
					token_string[TOK_DO], token_string[parser->tokens->items[parser->i].type]);
				return 1;
			}
			size_t addr = CALL_STACK_POP();
			parser->tokens->items[addr].as.scope.end = parser->i;
			parser->tokens->items[parser->i].as.scope.start = addr;
			parser->i++;
		} break;

		case TOK_IF: {
			if (!in_function) {
				error(parser->src, parser->fname, tok,
					"conditionals are only permitted inside functions");
				return 1;
			}
			size_t i = parser->i;
			while (parser->tokens->items[parser->i].type != TOK_THEN) {
				if (parser_eof(parser)) {
					error(parser->src, parser->fname, tok, "expected '%s' after '%s'",
						token_string[TOK_THEN], token_string[TOK_IF]);
					return 1;
				}
				parser->i++;
			}

			parser->tokens->items[i].as.scope.start = i;
			CALL_STACK_PUSH(i);
			parser->i = i + 1;
		} break;

		case TOK_ELSE: {
			if (!in_function) {
				error(parser->src, parser->fname, tok,
					"conditionals are only permitted inside functions");
				return 1;
			}
			if (CALL_STACK_EMPTY()) {
				error(parser->src, parser->fname, tok, "expected '%s' before '%s'",
					token_string[TOK_IF], token_string[TOK_ELSE]);
				return 1;
			}
			size_t i = parser->i;
			while (parser->tokens->items[parser->i].type != TOK_THEN) {
				if (parser_eof(parser)) {
					error(parser->src, parser->fname, tok, "expected '%s' after '%s'",
						token_string[TOK_THEN], token_string[TOK_ELSE]);
					return 1;
				}
				parser->i++;
			}

			size_t addr = CALL_STACK_POP();
			CALL_STACK_PUSH(addr);
			parser->tokens->items[addr].as.scope.middle = i;
			parser->tokens->items[i].as.scope.start = addr;
			parser->i = i + 1;
		} break;

		case TOK_THEN: {
			if (!in_function) {
				error(parser->src, parser->fname, tok,
					"conditionals are only permitted inside functions");
				return 1;
			}
			if (CALL_STACK_EMPTY()) {
				error(parser->src, parser->fname, tok, "expected '%s' before '%s'",
					token_string[TOK_IF], token_string[TOK_THEN]);
				return 1;
			}

			size_t addr = CALL_STACK_POP();
			parser->tokens->items[addr].as.scope.end = parser->i;
			parser->tokens->items[parser->i].as.scope.start = addr;
			parser->tokens->items[parser->i].as.scope.end = parser->i;
			if (parser->tokens->items[addr].as.scope.middle) {
				size_t else_addr = parser->tokens->items[addr].as.scope.middle;
				parser->tokens->items[else_addr].as.scope.end = parser->i;
			}
			parser->i++;
		} break;

		case TOK_BEGIN: {
			if (!in_function) {
				error(parser->src, parser->fname, tok,
					"loops are only permitted inside functions");
				return 1;
			}
			in_begin_loop = 1;
			size_t i = parser->i;
			while (parser->tokens->items[parser->i].type != TOK_UNTIL) {
				if (parser_eof(parser)) {
					error(parser->src, parser->fname, tok, "expected '%s' after '%s'",
						token_string[TOK_UNTIL], token_string[TOK_BEGIN]);
					return 1;
				}
				parser->i++;
			}
			parser->tokens->items[i].as.scope.start = 0;
			CALL_STACK_PUSH(i);
			parser->i = i + 1;
		} break;

		case TOK_UNTIL: {
			if (!in_begin_loop) {
				error(parser->src, parser->fname, tok, "expected '%s' before '%s'",
					token_string[TOK_BEGIN], token_string[parser->tokens->items[parser->i].type]);
				return 1;
			}
			in_begin_loop = 0;
			if (CALL_STACK_EMPTY()) {
				error(parser->src, parser->fname, tok, "expected '%s' before '%s'",
					token_string[TOK_BEGIN], token_string[parser->tokens->items[parser->i].type]);
				return 1;
			}
			size_t addr = CALL_STACK_POP();
			parser->tokens->items[addr].as.scope.end = parser->i;
			parser->tokens->items[parser->i].as.scope.start = addr;
			parser->i++;
		} break;
		default: {
			parser->i++;
		} break;
		}
	}
	return 0;
}
