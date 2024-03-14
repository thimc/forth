#ifndef STACK_H
#define STACK_H

cell stack_pop(Forth *f);
void stack_push(Forth *f, cell x);
cell stack_empty(Forth *f);
cell cstack_pop(Forth *f);
void cstack_push(Forth *f, cell x);
cell cstack_empty(Forth *f);

#endif // !STACK_H
