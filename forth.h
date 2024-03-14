#ifndef FORTH_H
#define FORTH_H

typedef int  cell;
typedef long dcell;
typedef char byte;

#define CELL_SIZE (sizeof(cell))

#define DATA_OFFSET      0     // 40k values
#define VARIABLES_OFFSET 40000 // 4882 variables
#define CALLSTACK_OFFSET 59535 // 1500 possible jumps
#define MEMORY_SIZE      65535

typedef enum {
	TOK_WORD,		// dup, swap, over ..
	TOK_STRING,		// ." this"
	TOK_NUMBER,		// 1, 2, 3, 4 ...
	TOK_COLON,		// : foo 100 + ;
	TOK_SEMICOLON,
	TOK_IF,			// 0 = if ." Yes!" else ." No!" then ;
	TOK_ELSE,
	TOK_THEN,
	TOK_DO,			// 10 0 do i . `loop` ;
	TOK_ITERATOR,
	TOK_PLUS_LOOP,	// 128 do i . i `+loop` ;
	TOK_LOOP,
	TOK_BEGIN,		// `begin` key 32 = until ;
	TOK_UNTIL,		//
	TOK_CONSTANT,	// 42 `constant` answer
	TOK_VARIABLE,	// `variable` numbers
	TOK_READ,	    // numbers `@`
	TOK_WRITE,	    // 123 variable `!`
	TOK_ADD,	    // 123 variable `+!`
	TOK_CELLS,      // 5 `cells` allot
	TOK_ALLOT,		// variable x 3 cells `allot`
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
static char *token_string[] = {
	"word", "literate", "number", ":", ";",
	"if", "else", "then", "do", "i", "+loop", "loop", "begin", "until",
	"constant", "variable", "get", "write", "add", "cells", "allot",
};

// static const char *variable_type[]  = {
// 	"variable", "constant", "array",
// };

typedef struct {
	char *name;
	size_t start, end;
} Word;

typedef struct {
	Word *items;
	size_t count, capacity;
} Words;

typedef enum {
	VAR_VARIABLE,
	VAR_CONSTANT,
	VAR_ARRAY,
} VariableType;

typedef struct {
	VariableType type;
	char *name;
	cell *address;
} Variable;

typedef struct {
	Variable *items;
	size_t count, capacity;
} Variables;

typedef struct {
	byte memory[MEMORY_SIZE];
	cell *cstack, *stack, *varstack;
	cell *sp, *csp, *vp;
	Tokens tokens;
	Words words;
	Variables variables;

	const char *file_name;
	char *source_code;
	size_t i;
} Forth;

typedef struct {
	const char *name;
	int (*fn)(Forth *f);
} Builtin;

void forth_init(Forth *f, const char *file_name, char *source_code);
void forth_free(Forth *f);

#endif // !FORTH_H
