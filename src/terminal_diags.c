#include <sys/types.h>
#include <terminal_diags.h>

#include <curses.h>
#include <string.h>
#include <array.h>

void init_terminal_diags()
{
	initscr();
	diagnostic_tables = array_init(2, sizeof(struct Diagnostic_Table*));
}

void stop_terminal_diags()
{
	endwin();
	array_kill(diagnostic_tables);
}

void update_terminal_diags()
{
	//TODO make this function show some stuff in the terminal.
}

struct Diagnostic_Table* diag_table_init()
{
	// allocate space for the diagnostic table
	struct Diagnostic_Table* t = malloc(sizeof(struct Diagnostic_Table));

	// add tis diagnostic table to the master list
	array_add(diagnostic_tables, t);

	// create the list of elements, which are arrays of chars.
	t->elements = array_init(1, sizeof(Array*));

	// set the current element to zero, so that it is not garbage.
	t->current_element = 0;

	return t;
}

void diag_table_kill(struct Diagnostic_Table* table)
{
	// loop through the diagnostic_tables array, until you find
	//  the table that matches the pointer
	for ( int i = 0; i < array_get_size(diagnostic_tables); i++ )
	{
		if ( array_get(diagnostic_tables, i) == table )
		{
			array_remove(diagnostic_tables, i);
			break;
		}
	}

	// free the memory that the table was stored in
	free(table);
}

Array* diag_table_add_element(struct Diagnostic_Table *table)
{
	// create the new char array
	Array* a = array_init(0, sizeof(char));

	// adds the array to the table element list.
	// because the table array is of type Array*, and the array_add()
	// function wants a pointer to the data, I am giving the function the address of
	// the newley created element variable, so it can copy the data at that location,
	// which will be the Array*
	array_add(table->elements, &a);

	// give the user their pointer, so they can update the text
	return a;
}

void diag_element_update(Array* element, char* text)
{
	// make sure the there is enough room for the text in the array.
	array_resize(element, strlen(text));

	// this will set the entire array to the string.
	strcpy(array_get(element, 0), text);
}
