#ifndef LEXER_H_
#define LEXER_H_

typedef enum {
	TOK_WORD,		// dup, swap, over ..
	TOK_STRING,		// ." this"
	TOK_NUMBER,		// 1, 2, 3, 4 ...
	TOK_COLON,		// : foo 100 + ;
	TOK_SEMICOLON,
	TOK_IF,			// 0 = if ." Yes!" else ." No!" then ;
	TOK_ELSE,
	TOK_THEN,
	TOK_DO,			// 10 0 do i . loop ;
	TOK_ITERATOR,
	TOK_PLUS_LOOP,	// 128 do i . i +loop ;
	TOK_LOOP,
	TOK_BEGIN,		//begin key 32 = until ;
	TOK_UNTIL,		//
	TOK_CONSTANT,	// 42 constant answer
	TOK_VARIABLE,	// variable numbers
	TOK_VAR_GET,	// variable @
	TOK_VAR_WRITE,	// 123 variable !
	TOK_VAR_ADD,	// 123 variable +!
} TokenType;

typedef struct {
    TokenType type;
    size_t location;
    union {
        struct {
            size_t start, middle, end;
        } scope;
        int number;
        char *string;
    } as;
} Token;

typedef struct {
	Token *items;
	size_t count, capacity;
} Tokens;

#define KEYWORD_START ((int)TOK_IF)
static const char *token_string[] = {
	"word", "literate", "number", ":", ";",
	"if", "else", "then", "do", "i", "+loop", "loop", "begin", "until",
	"constant", "variable", "get", "write", "add"
};

int lex(char *src, Tokens *tokens);

#endif // LEXER_H_
