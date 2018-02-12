all:
	gcc -O0 -g test.c simple-go.c -o test \
	-Wall -Wpedantic -Wnull-dereference -Wshadow -Wconversion -Wstrict-prototypes -Wmissing-prototypes -Wcast-qual -Wstrict-overflow=5 -Wunreachable-code -Wno-unused-parameter \
	-fsanitize=undefined
