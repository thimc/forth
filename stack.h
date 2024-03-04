#ifndef STACK_H_
#define STACK_H_

#define TRUE      -1
#define FALSE     0
#define TRUTHY(x) (((x) > FALSE) ? TRUE : FALSE)

#define VALUE_STACK_MAX        1024
#define VARIABLE_STACK_OFFSET  4096
#define CONST_VAR_STACK_OFFSET 6144
#define CALL_STACK_OFFSET      7168
#define STACK_SIZE             8192

#define STACK_PUSH(val) (STACK[SP++] = (val))
#define STACK_POP()     (STACK[--SP])
#define STACK_EMPTY()   (SP <= 0)
#define STACK_FULL()    (SP >= VALUE_STACK_MAX)

#define CALL_STACK_PUSH(val) (STACK[CSP++ + CALL_STACK_OFFSET] = (val))
#define CALL_STACK_POP()     (STACK[--CSP + CALL_STACK_OFFSET])
#define CALL_STACK_EMPTY()   (CSP <= 0)

extern int SP, CSP, ISP;
extern int STACK[STACK_SIZE];

#endif // STACK_H_
