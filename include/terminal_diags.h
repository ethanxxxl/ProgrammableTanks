#ifndef TERMINAL_DIAGS_H
#define TERMINAL_DIAGS_H

#include <array.h>
#include <curses.h>

struct Diagnostic_Table
{
	// title of the table
	char* title;
	
	// array of char* to c_strs
	Array *elements;

	int start_x;
	int start_y;
	int width;
	int height;

	int current_element;

	WINDOW* win;
};

// master list of all diagnostic tables
static Array *diagnostic_tables;
static int active_table;

// padding between tables
const static int PADDING = 1;

// starts and stops ncurses and such
void terminal_diags_init();
void terminal_diags_stop();

// updates the display (call this every frame)
void terminal_diags_update();

/* creates/adds a diagnostic table, that has multiple diagnostic elements.
 * returns a pointer to the new diagnostic table
 */
struct Diagnostic_Table* diag_table_init(const char* title, int width, int height);

/* Removes the diagnostic table from diagnostic_tables, and also
 * frees the memory that the table was stored in. therefore, that pointer will
 * no longer be usable
 */
void diag_table_kill(struct Diagnostic_Table *table);

/* table should be the table that you want to add an element to.
 * text should be the text of the element that will be added.
 *
 * returns a char* to the text of the element that was just created. don't lose this, you need it later
 */
char* diag_table_add_element(struct Diagnostic_Table *table, const char* text);

/* table is the table that should be modified
 * element the address of a variable containing an address to the element text.
 * text is the new text
 *
 * this will change out the string, however, the address to the elemnt can/will change, so
 * a double pointer is needed so that that address can be updated
 */
void diag_table_update_element(struct Diagnostic_Table* table, char** element, const char* text);

/* table is the table that should be modified
 * element the element that will be removed
 *
 * removes the element from the table. element will no longer be valid after this operation
 */
void diag_table_remove_element(struct Diagnostic_Table* table, char* element);

// cycles through the currently active table
//  any number greater than or equal to 1 will cycle forwards.
//  any number less than 1 cycles backwards
void diag_table_cycle(int direction);

// cycles through the currently active element
//  any number greater than or equal to 1 will cycle forwards.
//  any number less than 1 cycles backwards
void diag_table_cycle_element(int direction);

#endif
