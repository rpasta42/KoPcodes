CC=gcc
CFLAGS=-g -O2 -Wall -Wextra
OUTF=kopcodes.ex
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)


default: build


OUT_FILE_PATH=misc/tests/kopcodes.test_elf.ex

run_gen_elf: build
	./$(OUTF) asm misc/tests/test_elf.asm $(OUT_FILE_PATH);


readelf2: build #print header for example executable
	./$(OUTF) readelf misc/tests/test_elf.ex #$(OUTF)
readelf1: run_gen_elf
	./$(OUTF) readelf $(OUT_FILE_PATH)

run: run_gen_elf
	chmod u+x ./$(OUT_FILE_PATH); ./$(OUT_FILE_PATH)

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


