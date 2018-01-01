SRC = lib.c
HEAD = lib.h
OBJS = lib.o
CFLAGS += -Wall -Wextra -Wno-unused-function -I cake_libs/ -I ./ -std=c99
CC ?= clang

.PHONY: all tests clean

all: $(OBJS)

lib.o: build/lib.o

build/lib.o: lib.c lib.h
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ -c lib.c

tests: gcstring_test

gcstring_test: test/gcstring_test

test/gcstring_test: test/gcstring_test.o build/lib.o
	$(CC) -o $@ $^ -lcunit

test/gcstring_test.o: test/gcstring_test.c

clean:
	@if [ -f build/lib.o ] ; then rm build/lib.o ; fi
	@if [ -f test/cstring_test.o ] ; then rm test/cstring_test.o ; fi
	@if [ -f test/cstring_test ] ; then rm test/cstring_test ; fi
