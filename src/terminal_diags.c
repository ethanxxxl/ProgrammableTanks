#include "physics.h"
#include <SDL2/SDL_audio.h>
#include <sys/types.h>
#include <terminal_diags.h>

#include <curses.h>
#include <string.h>
#include <array.h>

void terminal_diags_init()
{
	initscr();
	noecho();
	cbreak();

	diagnostic_tables = array_init(2, sizeof(struct Diagnostic_Table*));
}

void terminal_diags_stop()
{
	endwin();
	array_kill(diagnostic_tables);
}

// TODO add height restriction, colors, borders
void terminal_diags_update()
{
	// loop through every diagnostic table
	for ( int i = 0; i < array_get_size(diagnostic_tables); i++ )
	{
		int line = 1;
		// t is the diagnostic table.
		struct Diagnostic_Table* t = *(struct Diagnostic_Table**)array_get(diagnostic_tables, i);

		// clear the window
		werase(t->win);
	
		// add the title, also increment line so that the next operation is on the next line
		wattron(t->win, A_BOLD);
		mvwaddnstr(t->win, line++, 1, t->title, t->width);
		wattroff(t->win, A_BOLD);

		// make sure the selected element is valid, just a double check, if the cycle functions
		// are used, there should be no problems
		if ( (t->current_element < 0) || (t->current_element >= array_get_size(t->elements)) )
		{
			// t->current_element was out of range
			t->current_element = 0;
		}
		// add the selected element
		/* tokenizes the element, using the newline as a delimiter. this way, the
		 * line length can be controlled easily.
		 */
		char* data = strdup(*(char**)array_get(t->elements, t->current_element));
		char* tk = strtok(data, "\n");
		while ( tk != NULL )
		{
			mvwaddnstr(t->win, line, 1, tk, t->width);
			tk = strtok(NULL, "\n");
			line++;
		}
		free(data); // make sure to free that unused memory
		
		// add border
		if ( i == active_table )
		{
			wattron(t->win, A_BLINK);
			box(t->win, 0, 0);
			wattroff(t->win, A_BLINK);
		}
		else
		{
			box(t->win, 0, 0);
		}

		wrefresh(t->win);
	}
}

struct Diagnostic_Table* diag_table_init(const char* title, int width, int height)
{
	// allocate space for the diagnostic table
	struct Diagnostic_Table* t = malloc(sizeof(struct Diagnostic_Table));

	// create the list of elements, each element is just a c-str.
	t->elements = array_init(1, sizeof(char*));

	// set the current element to zero, so that it is not garbage.
	t->current_element = 0;

	// set the title
	t->title = strdup(title);

	// set the width and height
	t->width = width;
	t->height = height;
	
	// create the window, and give a spot on the screen
	// find the starting x position for the new window
	t->start_x = 1;
	t->start_y = 0;
	for ( int i = 0; i < array_get_size(diagnostic_tables); i++ )
	{
		// TODO do a check to make sure that it will fit horizontally, and if it doesn't
		// then move it underneath
		t->start_x += (**(struct Diagnostic_Table**)array_get(diagnostic_tables, i)).width;
		t->start_x += PADDING;
		//mvaddstr(10, 1+i, "?");
	}

	
	// create the new window.
	t->win = newwin(height, width, t->start_y, t->start_x);

	//mvprintw(11, 1, "x: %d\ny: %d\nh: %d\nw: %d\nwin: %p", t->start_x, t->start_y, height, width, t->win);

	// add this diagnostic table to the master list
	array_add(diagnostic_tables, &t);

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

	// go through all of the elements and free the memory allocated for
	// their strings
	for ( int i = 0; i < array_get_size(table->elements); i++ )
	{
		free(*(char**)array_get(table->elements, i));
	}

	// free the title
	free(table->title);

	// delete the window
	delwin(table->win);

	// TODO after this, the rest of the tables will need rearanged

	// free the memory that the table was stored in
	free(table);
}

char* diag_table_add_element(struct Diagnostic_Table *table, const char* text)
{
	// create the new c-string.
	char* a = strdup(text);
	
	// adds the c-string to the table's list of elements
	array_add(table->elements, &a);

	// give the user their pointer, so they can update the text
	return a;
}

void diag_table_update_element(struct Diagnostic_Table* table, char** element, const char* text)
{
	// loop through all of the elements in the table
	for ( int i = 0; i < array_get_size(table->elements); i++ )
	{
		// look for an element that matches element
		if ( *element == *(char**)array_get(table->elements, i) )
		{
			// free the old string
			free(*element);

			// allocate space for the new text and update the user
			// provided variable
			*element = strdup(text);

			// update the pointer in element list
			array_set(table->elements, element, i);

			// no need to continue the search
		}
	}
}

void diag_table_remove_element(struct Diagnostic_Table* table, char* element)
{
	// loop through all of the elements in the table
	for ( int i = 0; i < array_get_size(table->elements); i++ )
	{
		// look for an element that matches element
		if ( element == *(char**)array_get(table->elements, i) )
		{
			// free the old string
			free(element);
			// remove the element
			array_remove(table->elements, i);
			// no need to continue the search
			break;
		}
	}
}

void diag_table_cycle(int direction)
{
	if ( direction >= 1 )
	{
		// we are going forwards:
		if ( active_table >= array_get_size(diagnostic_tables)-1 )
		{
			// the last table is active, so it needs to wrap around to hte beginning
			active_table = 0;
		}
		else
		{
			active_table++;
		}
	}
	else
	{
		// we are in reverse:
		if ( active_table <= 0 )
		{
			// the active table is the first one, so it needs to wrap around
			active_table = array_get_size(diagnostic_tables)-1;
		}
		else
		{
			active_table--;
		}
	}
}

void diag_table_cycle_element(int direction)
{
	// t is a pointer to the active table
	struct Diagnostic_Table* t = *(struct Diagnostic_Table**)array_get(diagnostic_tables, active_table);

	if ( direction >= 1 )
	{
		// we are going forwards:
		if ( t->current_element >= array_get_size(t->elements)-1)
		{
			// the last element is active, so it needs to wrap around to hte beginning
			t->current_element = 0;
		}
		else
		{
			t->current_element++;
		}
	}
	else
	{
		// we are in reverse:
		if ( t->current_element <= 0 )
		{
			// the active table is the first one, so it needs to wrap around
			t->current_element = array_get_size(t->elements)-1;
		}
		else
		{
			t->current_element--;
		}
	}
}
