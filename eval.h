// vim: ft=c
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

int eval(Tokens *tokens, Words *words);

#endif // EVAL_H_


