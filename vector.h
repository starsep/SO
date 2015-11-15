//Filip Czaplicki 359081

#ifndef VECTOR_H
#define VECTOR_H

///Struktura reprezentująca vector tj. dynamiczną tablicę
struct vector;

///Typedef, aby pisać vector zamiast struct vector
typedef struct vector vector;

/**
 Tworzy nowy vector, potem należy zwolnić vector za pomocą vector_done.
 @return Nowy vector
*/
vector * vector_new(void);

///Zamienia vector charów na c-string
char * vector_to_string(vector *vec);

/**
 Usuwa vector
 @param[in] vec Vector.
*/
void vector_done(vector *vec);

/**
 Zwraca wielkość vectora.
 @param[in] vec Vector.
 @return Wielkość vectora vec.
*/
int vector_size(const vector *vec);

/**
 Dodaje element na koniec vectora.
 @param[in] vec Vector.
 @param[in] element Element, który chcemy wstawić.
*/
void vector_push_back(vector *vec, const void * element);

///Usuwa element z początku
void vector_pop_front(vector *vec);

///Usuwa element z końca
void vector_pop_back(vector *vec);

/**
 Dodaje element do vectora na danym indeksie.
 Wszystkie elementy od miejsca, w którym wstawiamy do końca przesuwamy w prawo.
 @param[in] vec Vector.
 @param[in] element Element, który chcemy wstawić.
 @param[in] index Indeks, w ktorym chcemy wstawić.
*/
void vector_insert(vector *vec, const void * element, int index);

/**
 Usuwa element vectora.
 @param[in] vec Vector.
 @param[in] index Miejsce, z którego chcemy usunąć.
*/
void vector_remove(vector *vec, const int index);


///Wypisuje vector (charów) na stderr
void vector_debug(vector *vec);

/**
 Czyści vector (tj. usuwa wszystkie elementy, bez niszczenia vectora)
 @param vec Vector.
 */
void vector_clear(vector *vec);

///zwraca char z vector o indeksie = index
char vector_get(vector *vec, int index);

///czy vector pusty?
int vector_empty(vector *vec);

///char z początku vectora
char vector_front(vector *vec);

///char z końca vectora
char vector_back(vector *vec);

#endif /* VECTOR_H */
