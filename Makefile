CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g


a3a: a3a.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f a3a
