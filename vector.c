//Filip Czaplicki 359081

#include "vector.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

///Struktura reprezentująca vector tj. dynamiczną tablicę
struct vector {
	int size; ///<liczba elementów w vectorze
	int reserved; ///<liczba zarezerwowanych komórek pamięci
	void *content; ///<zawartość vectora
};


vector * vector_new(void) {
	vector *res = malloc(sizeof(vector));
	res->size = 0;
	res->reserved = 1;
	res->content = malloc(res->reserved * sizeof(char));
	return res;
}

void vector_done(vector *vec) {
	assert(vec != NULL);
	free(vec->content);
	free(vec);
}

int vector_size(const vector *vec) {
	assert(vec != NULL);
	return vec->size;
}

/**
Zmienia wielkość vectora
Jeżeli potrzeba to powiększa ilość zarezerwowanej pamięci
@param[in,out] vec Interesujący nas vector.
@param[in] size Nowa wielkość vectora
*/
static void vector_resize(vector *vec, const int size) {
	assert(vec != NULL);
	vec->size = size;
	if(vec->size > vec->reserved) {
		vec->reserved = vec->size > vec->reserved * 2 ? vec->size : vec->reserved * 2;
		vec->content = realloc(vec->content, vec->reserved * sizeof(char));
	}
	assert(vec->size <= vec->reserved);
}

void vector_insert(vector *vec, const void *element, const int index) {
	assert(vec != NULL);
	assert(0 <= index && index <= vec->size);
	vector_resize(vec, vec->size + 1);
	for(int i = vec->size - 1; i > index; i--) {
		memcpy((char *)vec->content + (i * sizeof(char)), (char *)vec->content + ((i - 1) * sizeof(char)), sizeof(char));
	}
	memcpy((char *)vec->content + index * sizeof(char), element, sizeof(char));
}

void vector_push_back(vector *vec, const void * element) {
	assert(vec != NULL);
	vector_insert(vec, element, vec->size);
}

void vector_remove(vector *vec, const int index) {
	assert(vec != NULL);
	assert(0 <= index && index < vec->size);
	vector_resize(vec, vec->size - 1);
	for(int i = index; i < vec->size; i++) {
		memcpy((char *)vec->content + (i * sizeof(char)), (char *)vec->content + ((i + 1) * sizeof(char)), sizeof(char));
	}
}

void vector_pop_back(vector *vec) {
	vector_remove(vec, vector_size(vec) - 1);
}

void vector_pop_front(vector *vec) {
	vector_remove(vec, 0);
}

void vector_clear(vector *vec) {
	vector_resize(vec, 0);
}

void vector_debug(vector *vec) {
	char *s = (char *) vec->content;
	for(int i = 0; i < vec->size; i++) {
		fprintf(stderr, "%c", s[i]);
	}
	fprintf(stderr, "\n");
}

char * vector_to_string(vector *vec) {
	char *result = malloc((vec->size + 1) * sizeof(char));
	for(int i = 0; i < vec->size; i++) {
		result[i] = ((char *)vec->content)[i];
	}
	result[vec->size] = '\0';
	return result;
}

char vector_get(vector *vec, int index) {
	assert(vec != NULL);
	assert(index >= 0 && index < vec->size);
	return ((char *)vec->content)[index];
}

char vector_front(vector *vec) {
	return vector_get(vec, 0);
}

char vector_back(vector *vec) {
	return vector_get(vec, vec->size - 1);
}

int vector_empty(vector *vec) {
	return vec->size == 0;
}