#ifndef STACK_H_
#define STACK_H_

#define TRUE      -1
#define FALSE     0
#define TRUTHY(x) (((x) > FALSE) ? TRUE : FALSE)

#define VALUE_STACK_MAX        1024 // 1024 values
#define VARIABLE_STACK_OFFSET  5120 // 4096 variables (starting from 1024)
#define CONST_VAR_STACK_OFFSET 6144	// 1024 constant variables (starting from 5120)
#define CALL_STACK_OFFSET      7168 // 1024 calls (starting from 6144)
#define ITER_STACK_OFFSET      8192 // 1024 iterators (starting from 7168)
#define STACK_SIZE             9216

#define STACK_PUSH(val) (STACK[SP++] = (val))
#define STACK_POP()     (STACK[--SP])
#define STACK_EMPTY()   (SP <= 0)
#define STACK_FULL()    (SP >= VALUE_STACK_MAX)

#define CALL_STACK_PUSH(val) (STACK[CSP++ + CALL_STACK_OFFSET] = (val))
#define CALL_STACK_POP()     (STACK[--CSP + CALL_STACK_OFFSET])
#define CALL_STACK_EMPTY()   (CSP <= 0)

#define ITERATOR_STACK_PUSH(val) (STACK[ISP++ + ITER_STACK_OFFSET] = (val))
#define ITERATOR_STACK_POP()     (STACK[--ISP + ITER_STACK_OFFSET])

extern int SP, CSP, ISP;
extern int STACK[STACK_SIZE];

#endif // STACK_H_
