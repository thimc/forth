// vim: ft=c
#ifndef BUILTINS_H_
#define BUILTINS_H_

int builtin_dot(void);
int builtin_cr(void);
int builtin_dup(void);
int builtin_swap(void);
int builtin_over(void);
int builtin_rot(void);
int builtin_emit(void);
int builtin_mod(void);
int builtin_and(void);
int builtin_or(void);
int builtin_invert(void);
int builtin_drop(void);
int builtin_add(void);
int builtin_sub(void);
int builtin_mul(void);
int builtin_div(void);
int builtin_equals(void);
int builtin_less(void);
int builtin_greater(void);
int builtin_dump(void);

typedef struct {
	const char *name;
	int (*fn)(void);
} Builtin;

static const Builtin builtins[] = {
	{ .name = ".",      .fn = builtin_dot },
	{ .name = "cr",     .fn = builtin_cr },
	{ .name = "dup",    .fn = builtin_dup },
	{ .name = "over",   .fn = builtin_over },
	{ .name = "rot",    .fn = builtin_rot },
	{ .name = "emit",   .fn = builtin_emit },
	{ .name = "swap",   .fn = builtin_swap },
	{ .name = "mod",    .fn = builtin_mod },
	{ .name = "and",    .fn = builtin_and },
	{ .name = "or",     .fn = builtin_or },
	{ .name = "invert", .fn = builtin_invert },
	{ .name = "drop",   .fn = builtin_drop },
	{ .name = "+",      .fn = builtin_add },
	{ .name = "-",      .fn = builtin_sub },
	{ .name = "*",      .fn = builtin_mul },
	{ .name = "/",      .fn = builtin_div },
	{ .name = "%",      .fn = builtin_mod },
	{ .name = "=",      .fn = builtin_equals },
	{ .name = "<",      .fn = builtin_less },
	{ .name = ">",      .fn = builtin_greater },

	{ .name = "dump",   .fn = builtin_dump },
};

#endif // !BUILTINS_H_
