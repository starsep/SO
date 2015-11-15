#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

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
		fprintf(stderr, "Usage: ./ToONP input\n");
		fprintf(stderr, "Examples:\n");
		fprintf(stderr, "  ./ToONP \"2 + 3 * x + 15 + 42 * ( 1 + 32 ) / 3\"\n");
		fprintf(stderr, "  ./ToONP \"e + 42 * ( p - i )\"\n");
		exit(1);
	}
	//fprintf(stderr, "%lu %s\n", strlen(argv[1]), argv[1]);
}

void w(int id, int *in, int *out) {
	int fds[2];
	pipe(fds);
	*in = fds[0];
	*out = fds[1];
	pid_t pid = fork();
	if(pid == 0) { //child
		write(STDERR, "Child\n", 6);
		write(STDERR, "Child2\n", 7);
		w(id + 1, in, out);
		_exit(0);
	}
	else { //parent
		for(int i = 0; i < INF/39500; i++);
		write(STDERR, "Parent\n", 7);
		write(STDERR, "Parent2\n", 8);
	}
}


void stream(char *input, char *stack, char *output, int out) {
	int input_length = strlen(input);
	int stack_length = strlen(stack);
	int output_length = strlen(output);
	int length = input_length + stack_length + output_length + 3;
	char *merged = (char *) malloc(length * sizeof(char));
	for(int i = 0; i < input_length; i++) {
		merged[i] = input[i];
	}
	merged[input_length] = SPLIT;
	for(int i = 0; i < stack_length; i++) {
		merged[i + input_length + 1] = stack[i];
	}
	merged[input_length + 1 + stack_length] = SPLIT;
	for(int i = 0; i < output_length; i++) {
		merged[i + input_length + stack_length + 2] = output[i];
	}
	merged[length - 1] = SPLIT;
	write(out, merged, length);
	free(merged);
}

void ToONP(char *input) {
	int n = strlen(input);
	//fprintf(stderr, "%d\n", n);
	if(n == 0) {
		fprintf(stderr, "ToONP: Empty input\n");
		return;
	}
	int in, out;
	w(0, &in, &out);
	stream(input, input, input, out);
	wait(NULL);
	printf("%d %d\n", in, out);
}

int main(int argc, char **argv) {
	check_arguments(argc, argv);
	ToONP(argv[1]);
	return 0;
}