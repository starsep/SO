#Filip Czaplicki 359081

CFLAGS=-std=gnu99 -pedantic -Wall -Wshadow -Wextra
COMPILER=gcc

all: ToONP

ToONP: ToONP.o
	$(COMPILER) -o ToONP ToONP.o
	
ToONP.o: ToONP.c
	$(COMPILER) -c $(CFLAGS) ToONP.c
	
clean:
	rm -f ToONP *.o