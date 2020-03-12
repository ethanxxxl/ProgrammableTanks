#ifndef TERMINAL_DIAGS_H
#define TERMINAL_DIAGS_H

#include <array.h>

struct Diagnostic_Table
{
	// title of the table
	char title[50];
	
	// array of char* to c_strs
	Array *elements;

	int width;
	int height;

	int current_element;
};

// master list of all diagnostic tables
static Array *diagnostic_tables;

// starts and stops ncurses and such
void init_terminal_diags();
void stop_terminal_diags();

// updates the display (call this every frame)
void update_terminal_diags();

/* creates/adds a diagnostic table, that has multiple diagnostic elements.
 * returns a pointer to the new diagnostic table
 */
struct Diagnostic_Table* diag_table_init();

/* Removes the diagnostic table from diagnostic_tables, and also
 * frees the memory that the table was stored in. therefore, that pointer will
 * no longer be usable
 */
void diag_table_kill(struct Diagnostic_Table *table);

/* table should be the table that you want to add an element to.
 * element should be the text of the element that will be added.
 *
 * returns an array (of type char) which is to be used as a fancy string.
 * keep this. it is important.
 */
Array* diag_table_add_element(struct Diagnostic_Table *table);

/* a little helper function so that you don't have to use strcpy on the array. :)
 * this does NOT update the display. use update_terminal_diags() for that.
 */
void diag_element_update(Array* element, char* text);
#endif
