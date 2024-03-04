#include <stdbool.h>
#include <stdio.h>

#include "lexer.h"
#include "stack.h"
#include "util.h"
#include "stack.h"
#include "builtins.h"

int builtin_dot(void) {
	printf("%d", STACK_POP());
	return 0;
}

int builtin_cr(void) {
	putchar('\n');
	return 0;
}

int builtin_dup(void) {
	int a = STACK_POP();
	STACK_PUSH(a);
	STACK_PUSH(a);
	return 0;
}

int builtin_swap(void) {
	int a = STACK_POP();
	int b = STACK_POP();
	STACK_PUSH(a);
	STACK_PUSH(b);
	return 0;
}

int builtin_over(void) {
	int a = STACK_POP();
	int b = STACK_POP();
	STACK_PUSH(b);
	STACK_PUSH(a);
	STACK_PUSH(b);
	return 0;
}

int builtin_rot(void) {
	int a = STACK_POP();
	int b = STACK_POP();
	int c = STACK_POP();
	STACK_PUSH(b);
	STACK_PUSH(a);
	STACK_PUSH(c);
	return 0;
}

int builtin_emit(void) {
	int a = STACK_POP();
	printf("%c", a);
	return 0;
}

int builtin_mod(void) {
	int a = STACK_POP();
	int b = STACK_POP();
	STACK_PUSH(b % a);
	return 0;
}

int builtin_and(void) {
	int a = STACK_POP();
	int b = STACK_POP();
	STACK_PUSH(b & a);
	return 0;
}

int builtin_or(void) {
	int a = STACK_POP();
	int b = STACK_POP();
	STACK_PUSH(b | a);
	return 0;
}

int builtin_invert(void) {
	int a = STACK_POP();
	STACK_PUSH(~a);
	return 0;
}

int builtin_drop(void) {
	STACK[SP--] = 0;
	return 0;
}

int builtin_add(void) {
	int a = STACK_POP();
	int b = STACK_POP();
	STACK_PUSH(a + b);
	return 0;
}

int builtin_sub(void) {
	int a = STACK_POP();
	int b = STACK_POP();
	STACK_PUSH(b - a);
	return 0;
}

int builtin_mul(void) {
	int a = STACK_POP();
	int b = STACK_POP();
	STACK_PUSH(b * a);
	return 0;
}

int builtin_div(void) {
	int a = STACK_POP();
	int b = STACK_POP();
	STACK_PUSH(b / a);
	return 0;
}

int builtin_equals(void) {
	int a = STACK_POP();
	int b = STACK_POP();
	STACK_PUSH(TRUTHY(b == a));
	return 0;
}

int builtin_less(void) {
	int a = STACK_POP();
	int b = STACK_POP();
	STACK_PUSH(TRUTHY(b < a));
	return 0;
}

int builtin_greater(void) {
	int a = STACK_POP();
	int b = STACK_POP();
	STACK_PUSH(TRUTHY(b > a));
	return 0;
}

int builtin_stacktrace(void) {
	putchar('\t');
	for (int s = 0; s < SP; s++) {
		printf("%d ", STACK[s]);
	}
	printf("<- Top");
	return 0;
}
