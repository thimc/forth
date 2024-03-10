#ifndef EVAL_H_
#define EVAL_H_

typedef struct {
	char *name;
	size_t start, end;
} Word;

typedef struct {
	Word *items;
	size_t count, capacity;
} Words;

typedef struct {
	VariableType type;
	char *name;
	union {
		int value;
	} as;
} Variable;

typedef struct {
	Variable *items;
	size_t count, capacity;
} Variables;

int eval(Tokens *tokens, Words *words, Variables *variables);

#endif // EVAL_H_


