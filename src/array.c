#include <array.h>
#include <string.h>
#include <stdio.h>

typedef struct Array
{
	size_t _unit_size;
    int _capacity;
	int _num_elements;

	char* buff;
} Array;

Array* array_init(int n, size_t unit_size)
{
	// allocate space for the array struct
	Array* arr = malloc(sizeof(Array));
	if ( arr == NULL ) // make sure malloc didn't fail
		return NULL;

	// allocate space for the buffer.
	arr->buff = calloc(n*2, unit_size);
	if ( arr->buff == NULL ) // make sure malloc didn't fail
		return NULL;

	// update the buffer size and unit size trackers.
	arr->_unit_size = unit_size;
    arr->_capacity = n;

	arr->_num_elements = 0;

	return arr;
}

void array_kill(Array* arr)
{
	free(arr->buff);
	free(arr);
}

Array* array_resize(Array* arr, int n)
{
	// resize the array
	arr->buff = realloc(arr->buff, n*arr->_unit_size);
	if ( arr->buff == NULL ) // make sure that realloc was successful
		return NULL;
	
	// set the capacity tracker
	arr->_capacity = n;

	// update the elements tracker.
	if ( arr->_num_elements > n )
		arr->_num_elements = n;

	return arr;
}

int array_get_size(const Array* arr)
{
	return arr->_num_elements;
}

int array_add(Array* arr, void* data)
{
	// check if there is enough space to add the new element
	if ( arr->_num_elements >= arr->_capacity )
	{
		// the buffer was to small. double the buffer size.
		arr->buff = realloc(arr->buff, arr->_capacity*arr->_unit_size*2);
		arr->_capacity *= 2; // update the capacity tracker.
	}

	// pointer to the location that data should be written.
	void* p = arr->buff + (arr->_unit_size * (arr->_num_elements))/sizeof(char);
	arr->_num_elements++;

	// copy data into that location
	memcpy(p, data, arr->_unit_size);

	return arr->_num_elements - 1;
}

void array_remove(Array* arr, int index)
{
	if ( index > arr->_num_elements )
		return; // nothing to remove

	if ( index == arr->_num_elements )
	{
		memset(arr->buff + arr->_unit_size*index, 0, arr->_unit_size);
		arr->_num_elements--;
		return;
	}

	memmove(
			arr->buff + (index * arr->_unit_size),
			arr->buff + ((index+1) * arr->_unit_size),
			(arr->_num_elements - index) * arr->_unit_size
			);
	arr->_num_elements--;
}

void* array_get(const Array* arr, int index)
{
	return arr->buff + arr->_unit_size * index / sizeof(char);
}

void* array_get_into(const Array* arr, void* dest, int index)
{
	void* p = arr->buff + arr->_unit_size * index / sizeof(char);
	memcpy(dest, p, arr->_unit_size);
	return p;
}

void array_set(Array* arr, void* src, int index)
{
	if ( index >= arr->_capacity )
	{
		// if program control reaches here, that means that the index is bigger than the buffer
		arr->buff = realloc(arr->buff, index*arr->_unit_size*2);

		// update the buffer and element trackers.
		arr->_capacity = index*2;
		arr->_num_elements = index + 1;
	}
		
	// copy the value at src into the position in the buffer
	void* p = arr->buff + arr->_unit_size * index / sizeof(char);
	memcpy(p, src, arr->_unit_size);
}

void array_reset(Array* arr)
{
	arr->_num_elements = 0;
}

void array_clear(Array* arr)
{
	memset(arr->buff, 0, arr->_unit_size * arr->_num_elements);
	arr->_num_elements = 0;
}
