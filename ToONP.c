//Filip Czaplicki 359081

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include "vector.h"

#define INF 1000000000
#define SPLIT '|'

///dla operatora zwraca jego priorytet
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

///sprawdza poprawność argumentów
void check_arguments(int argc, char **argv) {
	if(argc != 2) {
		fprintf(stderr, "Usage: %s input\n", argv[0]);
		fprintf(stderr, "Examples:\n");
		fprintf(stderr, "  %s \"2 + 3 * x + 15 + 42 * ( 1 + 32 ) / 3\"\n", argv[0]);
		fprintf(stderr, "  %s \"e + 42 * ( p - i )\"\n", argv[0]);
		exit(1);
	}
}

///wysyła c-string what do pipe'a out
void stream(char *what, int out) {
	static char split[] = "x";
	split[0] = SPLIT;
	write(out, what, strlen(what));
	write(out, split, 1);
}

///robi stream vectora what
void stream_vector(vector *what, int out) {
	char *string = vector_to_string(what);
	stream(string, out);
	free(string);
}

///usuwa whitespace z początku vectora input
void remove_whitespace(vector *input) {
	while(!vector_empty(input) && isspace(vector_front(input))) {
		vector_pop_front(input);
	}
}

///pobiera dane z pipe'a from, zapisuje do vectorów input, stack i output
void get_data(vector **input, vector **stack, vector **output, int from) {
	*input = vector_new();
	*stack = vector_new();
	*output = vector_new();
	
	vector *vectors[3] = { *input, *stack, *output };
	size_t size = 100;
	char *buffer = malloc(size);
	int bytes_read;
	int splits = 0;
	while(splits < 3 && (bytes_read = read(from, buffer, size)) > 0) {
		for(int i = 0; i < bytes_read; i++) {
			if(buffer[i] == SPLIT) {
				splits++;
			}
			else {
				vector_push_back(vectors[splits], buffer + i);
			}
		}
	}
	remove_whitespace(*input);
	free(buffer);
}

///przesyła cały stos na wyjście
void stack_to_output(vector *stack, vector *output) {
	static char tmp[] = "x ";
	while(!vector_empty(stack)) {
		tmp[0] = vector_back(stack);
		vector_pop_back(stack);
		vector_push_back(output, tmp + 1);
		vector_push_back(output, tmp);
	}
}

///jeden krok algorytmu
int do_your_job(vector *input, vector *stack, vector *output) {
	if(vector_empty(input)) {
		stack_to_output(stack, output);
		return 0;
	}
	static char tmp[] = "x ";
	if(isdigit(vector_front(input)) || isalpha(vector_front(input))) {
		vector_push_back(output, tmp + 1);
		while(!vector_empty(input) && !isspace(vector_front(input))) {
			tmp[0] = vector_front(input);
			vector_pop_front(input);
			vector_push_back(output, tmp);
		}
	}
	else { //operator
		tmp[0] = vector_front(input);
		vector_pop_front(input);
		switch(tmp[0]) {
			case '(':
				vector_push_back(stack, tmp);
				break;
			case ')':
				while(!vector_empty(stack)) {
					tmp[0] = vector_back(stack);
					vector_pop_back(stack);
					if(tmp[0] == '(') {
						break;
					}
					vector_push_back(output, tmp + 1);
					vector_push_back(output, tmp);
				}
				break;
			default:
				if(vector_empty(stack)) {
					vector_push_back(stack, tmp);
				}
				else {
					static char top[] = "x ";
					top[0] = vector_back(stack);
					if(operator_priority(tmp[0]) > operator_priority(top[0])) {
						vector_push_back(stack, tmp);
					}
					else {
						while(!vector_empty(stack)) {
							top[0] = vector_back(stack);
							if(operator_priority(tmp[0]) <= operator_priority(top[0])) {
								vector_push_back(output, top + 1);
								vector_push_back(output, top);
								vector_pop_back(stack);
							}
							else {
								break;
							}
						}
						vector_push_back(stack, tmp);
					}
				}
				break;
		}
	}
	remove_whitespace(input);
	if(vector_empty(input)) {
		stack_to_output(stack, output);
	}
	return !vector_empty(input);
}

///zapisuje w output wynik konwersji z pipe'a from
void get_output(vector **output, int from) {
	const size_t size = 100;
	char *buffer = malloc(size);
	int bytes_read;
	vector *good_output = vector_new();
	int split = 0;
	while(!split && (bytes_read = read(from, buffer, size)) > 0) {
		for(int i = 0; i < bytes_read; i++) {
			if(buffer[i] == SPLIT) {
				split = 1;
				break;
			}
			else {
				vector_push_back(good_output, buffer + i);
			}
		}
	}
	free(buffer);
	*output = good_output;
}

///funkcja sterująca tworzenie wątków
void w(int id, int in, int out) {
	vector *input, *stack, *output;
	get_data(&input, &stack, &output, in);
	if(!do_your_job(input, stack, output)) {
		stream_vector(output, out);
	}
	else {
		int fds[2];
		pipe(fds);
		pid_t pid = fork();
		if(pid == 0) { //child
			w(id + 1, fds[0], fds[1]);
		}
		else { //parent
			stream_vector(input, fds[1]);
			stream_vector(stack, fds[1]);
			stream_vector(output, fds[1]);
			
			waitpid(pid, NULL, 0);
			
			vector *good_output;
			get_output(&good_output, fds[0]);
			stream_vector(good_output, out);
			vector_done(good_output);
		}
		close(fds[0]);
		close(fds[1]);
	}
	vector_done(input);
	vector_done(stack);
	vector_done(output);
}

///główna funkcja ToONP
void ToONP(char *input) {
	int n = strlen(input);
	if(n == 0) {
		fprintf(stderr, "ToONP: Empty input\n");
		return;
	}
	int fds[2];
	pipe(fds);
	pid_t pid = fork();
	if(pid == 0) { //child
		w(1, fds[0], fds[1]);
	}
	else { //parent
		stream(input, fds[1]);
		stream("", fds[1]);
		stream("", fds[1]);
		
		waitpid(pid, NULL, 0);	
		
		vector *good_output;
		get_output(&good_output, fds[0]);
		char *final_output = vector_to_string(good_output);
		printf("%s\n", final_output + 1);
		vector_done(good_output);
		free(final_output);
	}
	close(fds[0]);
	close(fds[1]);
}

int main(int argc, char **argv) {
	check_arguments(argc, argv);
	ToONP(argv[1]);
	return 0;
}