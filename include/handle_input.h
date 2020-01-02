#ifndef HANDLE_INPUT_H
#define HANDLE_INPUT_H

#include <SDL2/SDL.h>

enum key_state { UP, DOWN, DOWN_TO_UP, UP_TO_DOWN };
typedef enum key_state key_state;

// associates a scancode with a function pointer.
struct Key_Callback
{
	SDL_Scancode scancode;
	void (*callback)(key_state toggle);
	key_state state;
};

void handle_input(struct Key_Callback* k, int size);

#endif
