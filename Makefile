CFLAGS = -Werror -Wextra -Wpedantic -Wall
CC = clang -fno-color-diagnostics

.PHONY: clean cleano covidval


covid: covid.o parser.o county.o
	$(CC) C_code/covid.o C_code/parser.o C_code/county.o -lm -o covid

covid.o: C_code/covid.c  
	cd C_code && $(CC) $(CFLAGS) -c covid.c

parser.o: C_code/parser.c C_code/parser.h
	cd C_code && $(CC) $(CFLAGS) -c parser.c

county.o: C_code/county.c C_code/county.h
	cd C_code && $(CC) $(CFLAGS) -c county.c


test: test.o county.o
	$(CC) C_code/test.o county.o -o test

test.o: C_code/test.c
	cd C_code && $(CC) $(CFLAGS) -c test.c


clean: 
	rm -f -r covid test counties/*
	make cleano

cleano:
	rm -f  C_code/covid.o C_code/parser.o C_code/county.o C_code/test.o

covidval: 
	make clean
	make covid
	valgrind --leak-check=full --show-leak-kinds=all ./covid
