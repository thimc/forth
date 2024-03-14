#include <stdio.h>

#include "forth.h"
#include "util.h"
#include "stack.h"

int builtin_dot(Forth *f) {
	cell a = stack_pop(f);
	printf("%d", a);
	return 0;
}

int builtin_cr(Forth *f) {
	(void)f;
	putchar('\n');
	return 0;
}

int builtin_dup(Forth *f) {
	cell a = stack_pop(f);
	stack_push(f, a);
	stack_push(f, a);
	return 0;
}

int builtin_swap(Forth *f) {
	cell a = stack_pop(f);
	cell b = stack_pop(f);
	stack_push(f, a);
	stack_push(f, b);
	return 0;
}

int builtin_over(Forth *f) {
	cell a = stack_pop(f);
	cell b = stack_pop(f);
	stack_push(f, b);
	stack_push(f, a);
	stack_push(f, b);
	return 0;
}

int builtin_rot(Forth *f) {
	cell a = stack_pop(f);
	cell b = stack_pop(f);
	cell c = stack_pop(f);
	stack_push(f, b);
	stack_push(f, a);
	stack_push(f, c);
	return 0;
}

int builtin_emit(Forth *f) {
	cell a = stack_pop(f);
	printf("%c", a);
	return 0;
}

int builtin_mod(Forth *f) {
	cell a = stack_pop(f);
	cell b = stack_pop(f);
	stack_push(f, b % a);
	return 0;
}

int builtin_and(Forth *f) {
	cell a = stack_pop(f);
	cell b = stack_pop(f);
	stack_push(f, b & a);
	return 0;
}

int builtin_or(Forth *f) {
	cell a = stack_pop(f);
	cell b = stack_pop(f);
	stack_push(f, b | a);
	return 0;
}

int builtin_invert(Forth *f) {
	cell a = stack_pop(f);
	stack_push(f, ~a);
	return 0;
}

int builtin_drop(Forth *f) {
	(void)stack_pop(f);
	return 0;
}

int builtin_add(Forth *f) {
	cell a = stack_pop(f);
	cell b = stack_pop(f);
	stack_push(f, a + b);
	return 0;
}

int builtin_sub(Forth *f) {
	cell a = stack_pop(f);
	cell b = stack_pop(f);
	stack_push(f, b - a);
	return 0;
}

int builtin_mul(Forth *f) {
	cell a = stack_pop(f);
	cell b = stack_pop(f);
	stack_push(f, b * a);
	return 0;
}

int builtin_div(Forth *f) {
	cell a = stack_pop(f);
	cell b = stack_pop(f);
	stack_push(f, b / a);
	return 0;
}

int builtin_equals(Forth *f) {
	cell a = stack_pop(f);
	cell b = stack_pop(f);
	stack_push(f, TRUTHY(b == a));
	return 0;
}

int builtin_less(Forth *f) {
	cell a = stack_pop(f);
	cell b = stack_pop(f);
	stack_push(f, TRUTHY(b < a));
	return 0;
}

int builtin_greater(Forth *f) {
	cell a = stack_pop(f);
	cell b = stack_pop(f);
	stack_push(f, TRUTHY(b > a));
	return 0;
}

int builtin_stacktrace(Forth *f) {
	printf("  ");
	for (cell s = 0; s < (*f->sp); s++) {
		printf("%d ", (cell)f->stack[s]);
	}
	printf("<- Top");
	return 0;
}
