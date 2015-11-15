/** @file
	Implementacja vectora

	@ingroup dictionary
	@author Filip Czaplicki <fc359081@students.mimuw.edu.pl>
	@copyright Uniwerstet Warszawski
	@date 2015-05-21
 */

#include "vector.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

///Struktura reprezentująca vector tj. dynamiczną tablicę
struct vector
{
	int size; ///<liczba elementów w vectorze
	int reserved; ///<liczba zarezerwowanych komórek pamięci
	char size_of_element; ///<rozmiar pojedynczego elementu
	void *content; ///<zawartość vectora
};


vector * vector_new(const int size_of_element, const int start_size)
{
	vector *res = malloc(sizeof(vector));
	res->size = 0;
	res->reserved = start_size > 0 ? start_size : 1;
	res->size_of_element = size_of_element;
    //printf("%d\n", res->reserved * res->size_of_element);
	res->content = malloc(res->reserved * res->size_of_element);
	return res;
}

void * vector_content(const vector *vec)
{
	assert(vec != NULL);
	return vec->content;
}

void vector_done(vector *vec)
{
	assert(vec != NULL);
	free(vec->content);
	free(vec);
}

int vector_size(const vector *vec)
{
	assert(vec != NULL);
	return vec->size;
}

/**
Zmienia wielkość vectora
Jeżeli potrzeba to powiększa ilość zarezerwowanej pamięci
@param[in,out] vec Interesujący nas vector.
@param[in] size Nowa wielkość vectora
*/
static void vector_resize(vector *vec, const int size)
{
	assert(vec != NULL);
	vec->size = size;
	if(vec->size > vec->reserved)
	{
		vec->reserved = vec->size > vec->reserved * 2 ? vec->size : vec->reserved * 2;
		vec->content = realloc(vec->content, vec->reserved * vec->size_of_element);
	}
	assert(vec->size <= vec->reserved);
}

void vector_insert(vector *vec, const void *element, const int index)
{
	assert(vec != NULL);
	assert(0 <= index && index <= vec->size);
	vector_resize(vec, vec->size + 1);
	for(int i = vec->size - 1; i > index; i--)
		memcpy((char *)vec->content + (i * vec->size_of_element), (char *)vec->content + ((i - 1) * vec->size_of_element), vec->size_of_element);
	memcpy((char *)vec->content + index * vec->size_of_element, element, vec->size_of_element);
}

void vector_push_back(vector *vec, const void *element)
{
	assert(vec != NULL);
	vector_insert(vec, element, vec->size);
}

void vector_remove(vector *vec, const int index)
{
	assert(vec != NULL);
	assert(0 <= index && index < vec->size);
	vector_resize(vec, vec->size - 1);
	for(int i = index; i < vec->size; i++)
		memcpy((char *)vec->content + (i * vec->size_of_element), (char *)vec->content + ((i + 1) * vec->size_of_element), vec->size_of_element);
}

int vector_sorted_find_wchar_t(vector *vec, const wchar_t c)
{
	assert(vec != NULL);
	assert(vec->size_of_element == sizeof(wchar_t));
	int size = vector_size(vec);
	wchar_t *content = ((wchar_t *)vec->content);
	if(size == 0)
		return -1;
	int begin = 0;
	int end = size - 1;
	while(begin <= end)
	{
		int med = (begin + end) / 2;
		wchar_t check = content[med];
		if(check == c)
			return med;
		if(check < c)
			begin = med + 1;
		else
			end = med - 1;
	}
	return -1;
}

void vector_clear(vector *vec)
{
	vector_resize(vec, 0);
}
