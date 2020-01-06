#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

/* Requirements
 * - Create and manage dynamic arrays
 * - Init Requirments
 *   - Allocate memory for a stuct array, and enough spaces requested.
 *   - 
 */

typedef struct Array Array;

/* Initializes an array, with the starting capacity of n units
 * Return: on failure, it returns NULL. on success, it returns a pointer
 *  on success, it returns a pointer to an Array structure.
 * Notes: Do not create your own Array struct. Let the system do it for you.
 */
Array* array_init(int n, size_t unit_size);

/* Frees that was allocated to an arrar
 * Return: none
 */
void array_kill(Array* arr);

/* Resizes an array. If the new size is smaller than the current size, then
 *  any data in locations higher than n will be lost.
 * Return: returns a pointer the array on success. returns NULL on error.
 */
Array* array_resize(Array* arr, int n);

/* Simply returns the size of the array
 */
int array_get_size(const Array* arr);


/* Adds data to the array. Data should be the same type that the array was
 * initialized as.
 */
void array_add(Array* arr, void* data);


// TODO implement this
void array_remove(int index);

/* Gets the element at index.
 * Return: returns a void pointer, that must be dereferenced, to the location.
 */
void* array_get(const Array* arr, int index);

/* Gets the element at index and copies it into the position at dest.
 * Return: returns a void pointer, that must be cast, to the location.
 */
void* array_get_into(const Array* arr, void* dest, int index);

void array_set(Array* arr, void* src, int index);
#endif
