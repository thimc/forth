// vim: ft=c
#ifndef STACK_H_
#define STACK_H_

#define TRUTHY(x) (((x) == 1) ? -1 : 0)

#define STACK_SIZE      2048
#define STACK_PUSH(val) (STACK[SP] = (val), SP++)
#define STACK_POP()     (STACK[--SP])
#define STACK_EMPTY()   (SP == 0)
#define STACK_FULL()   	(SP >= STACK_SIZE)

#define CALL_STACK_SIZE      64
#define CALL_STACK_PUSH(val) (CALL_STACK[CSP++] = (val))
#define CALL_STACK_POP()     (CALL_STACK[--CSP])
#define CALL_STACK_EMPTY()   (CSP == 0)
#define CALL_STACK_FULL()    (CSP >= CALL_STACK_SIZE)

#define ITERATOR_STACK_SIZE      64
#define ITERATOR_STACK_PUSH(val) (ITERATOR_STACK[ISP++] = (val))
#define ITERATOR_STACK_POP()     (ITERATOR_STACK[--ISP])
#define ITERATOR_STACK_EMPTY()   (ISP == 0)
#define ITERATOR_STACK_FULL()    (ISP >= ITERATOR_STACK_SIZE)

extern int SP;
extern int STACK[STACK_SIZE];

extern int CSP;
extern int CALL_STACK[CALL_STACK_SIZE];

extern int ISP;
extern int ITERATOR_STACK[ITERATOR_STACK_SIZE];

#endif // STACK_H_
