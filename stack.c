#include <stdio.h>
#include <stdlib.h>

#include "forth.h"
#include "stack.h"

cell stack_pop(Forth *f) {
	if (*f->sp == 0) {
		fprintf(stderr, "Stack underflow\n");
		exit(1);
	}
	return f->stack[--(*f->sp)];
}

void stack_push(Forth *f, cell x) {
	if (*f->sp > (cell)(VARIABLES_OFFSET / CELL_SIZE)) {
		fprintf(stderr, "Stack overflow\n");
		exit(1);
	}
	f->stack[(*f->sp)++] = x;
}

cell stack_empty(Forth *f) {
	return (*f->sp) < 1;
}

cell cstack_pop(Forth *f) {
	if (*f->csp == 0) {
		fprintf(stderr, "Call stack underflow\n");
		exit(1);
	}
	return f->cstack[--(*f->csp)];
}

void cstack_push(Forth *f, cell x) {
	if (*f->csp + 1 > MEMORY_SIZE) {
		fprintf(stderr, "Call stack overflow\n");
		exit(1);
	}
	f->cstack[(*f->csp)++] = x;
}

cell cstack_empty(Forth *f) {
	return (*f->csp) < 1;
}
