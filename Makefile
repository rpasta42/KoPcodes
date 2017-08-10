CC=gcc
CFLAGS=-g -O2 -Wall -Wextra
OUTF=kopcodes.ex
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)


default: build

run: build
	./$(OUTF) readelf $(OUTF)


test: build
	./$(OUTF) test

gdb: build
	gdb --args $(OUTF) test

valgrind: build
	valgrind ./$(OUTF) test


build: $(OBJS)
	$(CC) -o $(OUTF) *.o $(CFLAGS)

%.o : %.c
	$(CC) -c -o $@ $< $(CFLAGS)


clean:
	rm -f *.o $(OUTF)


