CFLAGS = -Werror -Wextra -Wpedantic -Wall
CC = clang -fno-color-diagnostics

.PHONY: clean covidval


covid: covid.o parser.o county.o
	$(CC) covid.o parser.o county.o -lm -o covid

covid.o: C_code/covid.c  
	$(CC) $(CFLAGS) -c C_code/covid.c

parser.o: C_code/parser.c C_code/parser.h
	$(CC) $(CFLAGS) -c C_code/parser.c

county.o: C_code/county.c C_code/county.h
	$(CC) $(CFLAGS) -c C_code/county.c

clean: 
	rm -f -r covid covid.o parser.o county.o counties/*

covidval: 
	make clean
	make covid
	valgrind --leak-check=full --show-leak-kinds=all ./covid
