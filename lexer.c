#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "util.h"

int lex(char *src, Tokens *tokens) {
	size_t src_len = strlen(src);
	size_t i = 0;

	while (i < src_len) {
		while (i < src_len && isspace(src[i])) i++;
		switch (src[i]) {

		case '(': {
			size_t start = i;
			while (src[i++] != ')') {
				if (i >= src_len) {
					printf("ERROR: expected end of comment block that starts at index %zu, got EOF\n", start);
					return 1;
				}
			}
			i++;
		} break;

		case '\\': {
			size_t start = i;
			while (src[i] != '\n') {
				if (i >= src_len) {
					printf("ERROR: expected end of comment line that starts at index %zu, got EOF\n", start);
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
			da_append(tokens, tok);
			i++;
		} break;

		case ';': {
			Token tok = {
				.type = TOK_SEMICOLON,
				.location = i,
			};
			da_append(tokens, tok);
			i++;
		} break;

		case '.': {
			if (src[i+1] == '\"') {
				i++; // consume .
				i++; // consume "
				i++; // consume <space>
				if (i >= src_len) {
					printf("ERROR: expected literal string, got EOF at index %zu\n", i);
					return 1;
				}
				size_t start = i;
				while (src[i] != '\"') i++;
				size_t end = i - 1;
				i++; // consume "
				Token tok = {
					.type = TOK_LSTRING,
					.location = i,
				};
				tok.as.string = malloc(sizeof(tok.as.string) * end - start + 1);
				assert(tok.as.string != NULL);
				memcpy(tok.as.string, src+start, end - start + 1);
				tok.as.string[end - start + 1] = '\0';
				da_append(tokens, tok);
				continue;
			}
		};

		default:
			if (src[i] == 'i' && isspace(src[i + 1])) {
				Token tok = {
					.type = TOK_ITERATOR,
					.location = i,
				};
				da_append(tokens, tok);
				i++;
				break;
			}

			if ((src[i] == '-' && isdigit(src[i+1])) || isdigit(src[i])) {
				char negate = (src[i] == '-');
				if (negate) i++;
				size_t start = i;
				while (isdigit(src[i])) {
					if (i >= src_len) break;
					i++;
				}
				size_t end = i;
				char buffer[end - start];
				assert(end > start);
				memcpy(buffer, src + start, end - start);
				buffer[end-start] = '\0';
				Token tok = {
					.type = TOK_NUMBER,
					.location = i,
					.as.number = atoi(buffer),
				};
				da_append(tokens, tok);
				break;
			}

			Token tok = {
				.type = TOK_WORD,
				.location = i,
			};

			size_t start = i;
			while (!isspace(src[i])) {
				if (i >= src_len) break;
				i++;
			}
			size_t end = i;
			if (end-start < 1) break;
			tok.as.string = malloc(sizeof(tok.as.string) * end - start + 1);
			assert(tok.as.string != NULL);
			memcpy(tok.as.string, src + start, end - start);
			tok.as.string[end - start] = '\0';

			for (size_t kw = KEYWORD_START; kw < ARRAY_LEN(token_string); kw++) {
				if (strcmp(token_string[kw], tok.as.string) == 0) {
					tok.type = (TokenType)kw;
					break;
				}
			}

			da_append(tokens, tok);
			break;
		}
	}

	return 0;
}
