CFLAGS = -Werror -Wextra -Wpedantic -Wall
CC = clang -fno-color-diagnostics

.PHONY: clean cleano covidval


covid: covid.o parser.o county.o state.o us.o
	$(CC) C_code/covid.o C_code/parser.o C_code/county.o  C_code/state.o C_code/us.o -lm -o covid

covid.o: C_code/covid.c  
	cd C_code && $(CC) $(CFLAGS) -c covid.c

parser.o: C_code/parser.c C_code/parser.h
	cd C_code && $(CC) $(CFLAGS) -c parser.c

county.o: C_code/county.c C_code/county.h
	cd C_code && $(CC) $(CFLAGS) -c county.c

state.o: C_code/state.c C_code/state.h
	cd C_code && $(CC) $(CFLAGS) -c state.c

us.o: C_code/us.c C_code/us.h
	cd C_code && $(CC) $(CFLAGS) -c us.c


test: test.o county.o state.o us.o
	$(CC) C_code/test.o C_code/county.o C_code/state.o C_code/us.o -o test

test.o: C_code/test.c
	cd C_code && $(CC) $(CFLAGS) -c test.c


clean: 
	rm -f -r covid test
	make cleano

cleano:
	rm -f  C_code/covid.o C_code/parser.o C_code/county.o C_code/state.o C_code/us.o C_code/test.o 

cleandata:
	rm -f counties/* states/* united_states/*

covidval: 
	make clean
	make covid
	valgrind --leak-check=full --show-leak-kinds=all ./covid
