#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "forth.h"
#include "util.h"
#include "lexer.h"

void forth_lexer_error(char *src, const char *fname, Token *token, const char *fmt, ...) {
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

    /* FIXME: To get the "^near  here" marker point some what correctly
     * we need to replace the tabs so that the width of all symbols
     * are the same. */
	for (size_t n = 0; n < strlen(src_line); n++)
		if (src_line[n] == '\t') src_line[n] = ' ';

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

int forth_lex(Forth *f) {
	size_t source_code_len = strlen(f->source_code);
	size_t i = 0;

	while (i < source_code_len) {
		while (i < source_code_len && iswhitespace(f->source_code[i])) i++;
		if (i >= source_code_len) break;
		switch (f->source_code[i]) {

		case '(': {
			size_t start = i;
			while (f->source_code[i++] != ')') {
				if (i >= source_code_len) {
					printf("ERROR: expected end of comment block that starts at index %zu, got EOF\n", start);
					return 1;
				}
			}
			i++;
		} break;

		case '\\': {
			size_t start = i;
			while (f->source_code[i] != '\n') {
				if (i >= source_code_len) {
					printf("ERROR: expected end of comment that starts at index %zu, got EOF\n", start);
					return 1;
				}
				i++;
			}
		} break;

		case ':': {
			Token tok = {
				.type = TOK_COLON,
				.location = i,
			};
			da_append(&f->tokens, tok);
			i++;
		} break;

		case ';': {
			Token tok = {
				.type = TOK_SEMICOLON,
				.location = i,
			};
			da_append(&f->tokens, tok);
			i++;
		} break;

		case '@': {
			Token tok = {
				.type = TOK_READ,
				.location = i,
			};
			da_append(&f->tokens, tok);
			i++;
		} break;

		case '!': {
			Token tok = {
				.type = TOK_WRITE,
				.location = i,
			};
			da_append(&f->tokens, tok);
			i++;
		} break;

		case '.': {
			if (f->source_code[i+1] == '\"') {
				i++; // consume .
				i++; // consume "
				i++; // consume <space>
				size_t start = i;
				while (f->source_code[i] != '\"') {
					if (i >= source_code_len) {
						/* forth_lexer_error(f->source_code, f->file_name, &f->tokens.items[start], "Expected end of string literal"); */
						printf("ERROR: expected literal string, got EOF at index %zu\n", i);
						return 1;
					}
					i++;
				}
				size_t end = i - 1;
				i++; // consume "
				Token tok = {
					.type = TOK_STRING,
					.location = i,
				};
				tok.as.string = malloc(sizeof(tok.as.string) * end - start + 1);
				assert(tok.as.string != NULL);
				memcpy(tok.as.string, f->source_code+start, end - start + 1);
				tok.as.string[end - start + 1] = '\0';
				da_append(&f->tokens, tok);
				continue;
			}
		};
		// fall through

		default:
			if (f->source_code[i] == 'i' && iswhitespace(f->source_code[i + 1])) {
				Token tok = {
					.type = TOK_ITERATOR,
					.location = i,
				};
				da_append(&f->tokens, tok);
				i++;
				break;
			}

			if ((f->source_code[i] == '-' && isnumber(f->source_code[i+1])) || isnumber(f->source_code[i])) {
				char negate = (f->source_code[i] == '-');
				if (negate) i++;
				size_t start = i;
				while (isnumber(f->source_code[i])) {
					if (i >= source_code_len) break;
					i++;
				}
				size_t end = i;
				char buffer[end - start + 1];
				assert(end > start);
				memcpy(buffer, f->source_code + start, end - start);
				buffer[end-start] = '\0';
				Token tok = {
					.type = TOK_NUMBER,
					.location = i,
					.as.number = atoi(buffer),
				};
				da_append(&f->tokens, tok);
				break;
			}

			Token tok = {
				.type = TOK_WORD,
				.location = i,
			};

			size_t start = i;
			while (!iswhitespace(f->source_code[i])) {
				if (i >= source_code_len) break;
				i++;
			}
			size_t end = i;
			if (end-start < 1) break;
			tok.as.string = malloc(sizeof(tok.as.string) * end - start + 1);
			assert(tok.as.string != NULL);
			memcpy(tok.as.string, f->source_code + start, end - start);
			tok.as.string[end - start] = '\0';
			to_lower(tok.as.string);

			for (size_t kw = KEYWORD_START; kw < ARRAY_LEN(token_string); kw++) {
				if (strcmp(token_string[kw], tok.as.string) == 0) {
					tok.type = (TokenType)kw;
					break;
				}
			}
			if (strcmp(tok.as.string, "+!") == 0) tok.type = TOK_ADD;
			da_append(&f->tokens, tok);
			break;
		}

	}
	return 0;
}

int forth_parser_eof(Forth *f) {
	return f->i >= f->tokens.count;
}

Token *forth_current_token(Forth *f) {
	return &f->tokens.items[f->i];
}

Token *forth_peek_token(Forth *f) {
	if (forth_parser_eof(f)) return NULL;
	return &f->tokens.items[f->i + 1];
}
