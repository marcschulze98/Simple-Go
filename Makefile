all:
	gcc -Og -g test.c simple-go.c -o test -std=c11 \
	-Wall -Wextra -Wpedantic -Wnull-dereference -Wshadow -Wconversion -Wstrict-prototypes -Wmissing-prototypes -Wcast-qual -Wstrict-overflow=5 -Wunreachable-code -Wno-unused-parameter \
	-fsanitize=undefined
