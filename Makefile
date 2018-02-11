all:
	gcc -Og -g simple-go.c \
	-Wall -Werror -Wpedantic -Wnull-dereference -Wshadow -Wconversion -Wstrict-prototypes -Wmissing-prototypes -Wcast-qual -Wstrict-overflow=5 -Wunreachable-code -Wno-unused-parameter
