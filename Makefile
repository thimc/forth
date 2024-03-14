CFLAGS = -std=c99 -Wall -Wextra -pedantic -ggdb -I. -Wshadow

SRCS = util.c builtins.c stack.c lexer.c parser.c eval.c forth.c
OBJS = $(SRCS:.c=.o)
TARGET = forth

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: $(TARGET)
