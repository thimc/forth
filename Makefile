NAME = forth
CC = cc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -pedantic-errors -ggdb -I.
LDFLAGS +=
SRC = main.c util.c lexer.c parser.c eval.c builtins.c stack.c

forth: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME) $(LDFLAGS)

clean:
	rm $(NAME)

.PHONY: $(NAME) clean
