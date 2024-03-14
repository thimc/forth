#ifndef LEXER_H
#define LEXER_H

void forth_lexer_error(char *src, const char *fname, Token *token, const char *fmt, ...);
int forth_lex(Forth *f);
int forth_parser_eof(Forth *f);
Token *forth_current_token(Forth *f);
Token *forth_peek_token(Forth *f);

#endif // !LEXER_H

