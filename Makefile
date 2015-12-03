#Filip Czaplicki 359081

CFLAGS=-std=gnu99 -pedantic -Wall -Wshadow -Wextra -Wunused -g -fsanitize=address
COMPILER=clang

all: ToONP

ToONP: ToONP.o vector.o
	$(COMPILER) $(CFLAGS) -o ToONP ToONP.o vector.o
	
ToONP.o: ToONP.c
	$(COMPILER) -c $(CFLAGS) ToONP.c

vector.o: vector.h vector.c
	$(COMPILER) -c $(CFLAGS) vector.c
	
clean:
	rm -f ToONP *.o
