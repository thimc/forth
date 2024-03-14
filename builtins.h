#ifndef BUILTINS_H
#define BUILTINS_H

int builtin_dot(Forth *f);
int builtin_cr(Forth *f);
int builtin_dup(Forth *f);
int builtin_swap(Forth *f);
int builtin_over(Forth *f);
int builtin_rot(Forth *f);
int builtin_emit(Forth *f);
int builtin_mod(Forth *f);
int builtin_and(Forth *f);
int builtin_or(Forth *f);
int builtin_invert(Forth *f);
int builtin_drop(Forth *f);
int builtin_add(Forth *f);
int builtin_sub(Forth *f);
int builtin_mul(Forth *f);
int builtin_div(Forth *f);
int builtin_equals(Forth *f);
int builtin_less(Forth *f);
int builtin_greater(Forth *f);
int builtin_stacktrace(Forth *f);

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

	// Debug words
	{ .name = ".s",     .fn = builtin_stacktrace },
};


#endif // !BUILTINS_H
