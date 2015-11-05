#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define INF 1000000000

int operator_priority(char op) {
	switch(op) {
		case '(':
			return 0;
		case '+':
			return 1;
		case '-':
			return 1;
		case ')':
			return 1;
		case '*':
			return 2;
		case '/':
			return 2;
		case '%':
			return 2;
		case '^':
			return 3;
		default:
			return INF;
	}
}

void check_arguments(int argc, char **argv) {
	if(argc != 2) {
		fprintf(stderr, "Usage: ./ToONP input\n");
		fprintf(stderr, "Examples:\n");
		fprintf(stderr, "  ./ToONP \"2 + 3 * x + 15 + 42 * ( 1 + 32 ) / 3\"\n");
		fprintf(stderr, "  ./ToONP \"e + 42 * ( p - i )\"\n");
		exit(1);
	}
	fprintf(stderr, "%lu %s\n", strlen(argv[1]), argv[1]);
}

int main(int argc, char **argv) {
	check_arguments(argc, argv);
	pid_t id = fork();
	switch(id) {
		case -1: //error
			perror("fork");
			exit(1);
		case 0: //child
			printf("LEL from child\n");
			exit(1);
		default: { //parent 
			int x;
			wait(&x);
			printf("LEL from parent %d\n", x);
			exit(x);
		}
	}
}