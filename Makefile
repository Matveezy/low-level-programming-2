.PHONY: build

CPPFLAGS = -pedantic-errors -Wall -Werror -g3 -O0 --sanitize=address,undefined,leak

build:
	/opt/homebrew/Cellar/bison/3.8.2/bin/bison -t -d parser.y -o parser.c
	flex -o lexer.c --header-file=lexer.h lexer.l
	gcc $(CFLAGS) lexer.c parser.c constant.c map_entry.c main.c -o main