// vim: ft=c
#ifndef PARSER_H_
#define PARSER_H_

typedef struct {
	const char *fname;
	char *src;
	Tokens *tokens;
	size_t i;
} Parser;

Token *current_token(Parser *parser);
Token *peek_token(Parser *parser);
int parser_eof(Parser *parser);
void error(char *src, const char *fname, Token *token, const char *fmt, ...);
int parse(Parser *parser);

#endif // PARSER_H_
