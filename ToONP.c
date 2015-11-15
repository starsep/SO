//Filip Czaplicki 359081

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "vector.h"

#define INF 1000000000
#define SPLIT '|'
#define STDIN 0
#define STDOUT 1
#define STDERR 2

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
		fprintf(stderr, "Usage: %s input\n", argv[0]);
		fprintf(stderr, "Examples:\n");
		fprintf(stderr, "  %s \"2 + 3 * x + 15 + 42 * ( 1 + 32 ) / 3\"\n", argv[0]);
		fprintf(stderr, "  %s \"e + 42 * ( p - i )\"\n", argv[0]);
		exit(1);
	}
}

void clean_up(vector *input, vector *stack, vector *output) {
	vector_done(input);
	vector_done(stack);
	vector_done(output);
}

void get_data(vector **input, vector **stack, vector **output, int from) {
	*input = vector_new();
	*stack = vector_new();
	*output = vector_new();
	vector *vectors[3] = { *input, *stack, *output };
	size_t size = 100;
	char *buffer = malloc(size);
	int read_bytes;
	int splits = 0;
	while(splits < 3 && (read_bytes = read(from, buffer, size)) > 0) {
		fprintf(stderr, "Read %d bytes\n", read_bytes);
		buffer[read_bytes] = '\0';
		fprintf(stderr, "%s\n", buffer);
		for(int i = 0; i < read_bytes; i++) {
			if(buffer[i] == SPLIT) {
				splits++;
			}
			else {
				vector_push_back(vectors[splits], buffer + i);
			}
		}
	}
	free(buffer);
}

void w(int id, int in, int out) {
	fprintf(stderr, "Process %d started\n", id);
	vector *input, *stack, *output;
	get_data(&input, &stack, &output, in);
	clean_up(input, stack, output);
}


void stream(char *input, char *stack, char *output, int out) {
	char split[] = "x";
	split[0] = SPLIT;
	write(out, input, strlen(input));
	write(out, split, 1);
	write(out, stack, strlen(stack));
	write(out, split, 1);
	write(out, output, strlen(output));
	write(out, split, 1);
}

void ToONP(char *input) {
	fprintf(stderr, "ToONP started\n");
	int n = strlen(input);
	if(n == 0) {
		fprintf(stderr, "ToONP: Empty input\n");
		return;
	}
	int fds[2];
	pipe(fds);
	pid_t id = fork();
	if(id == 0) { //child
		w(1, fds[0], fds[1]);
	}
	else {
		stream(input, "", "", fds[1]);
		waitpid(id, NULL, 0);	
	}
	//stream(input, input, input, out);
}

int main(int argc, char **argv) {
	check_arguments(argc, argv);
	ToONP(argv[1]);
	return 0;
}