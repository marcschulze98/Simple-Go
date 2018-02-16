all:
	gcc -L./ -lcutils -O0 -g test.c simple-go.c simple-gtp.c -o test -std=c11 \
	-Wall -Wextra -Wpedantic -Wnull-dereference -Wshadow -Wconversion -Wstrict-prototypes -Wmissing-prototypes -Wcast-qual -Wstrict-overflow=5 -Wunreachable-code -Wno-unused-parameter -Wno-missing-prototypes \
