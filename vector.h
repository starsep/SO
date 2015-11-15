#ifndef VECTOR_H
#define VECTOR_H

///Struktura reprezentująca vector tj. dynamiczną tablicę
struct vector;

///Typedef, aby pisać vector zamiast struct vector
typedef struct vector vector;

/**
 Tworzy nowy vector, potem należy zwolnić vector za pomocą vector_done.
 @param[in] size_of_element Liczba bajtów na element.
 @param[in] start_size Wielkość wektora na początku.
 @return Nowy vector
*/
vector * vector_new(const int size_of_element, const int start_size);

/**
 Zwraca zawartość vectora.
 @param[in] vec Vector.
 @return Zawartość vectora vec
*/
void * vector_content(const vector *vec);

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
void vector_push_back(vector *vec, const void *element);

/**
 Dodaje element do vectora na danym indeksie.
 Wszystkie elementy od miejsca, w którym wstawiamy do końca przesuwamy w prawo.
 @param[in] vec Vector.
 @param[in] element Element, który chcemy wstawić.
 @param[in] index Indeks, w ktorym chcemy wstawić.
*/
void vector_insert(vector *vec, const void *element, int index);

/**
 Usuwa element vectora.
 @param[in] vec Vector.
 @param[in] index Miejsce, z którego chcemy usunąć.
*/
void vector_remove(vector *vec, const int index);

/**
 Czyści vector (tj. usuwa wszystkie elementy, bez niszczenia vectora)
 @param vec Vector.
 */
void vector_clear(vector *vec);

#endif /* VECTOR_H */
