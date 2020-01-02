#include <handle_input.h>
#include <SDL2/SDL.h>

void handle_input(struct Key_Callback* k, int size)
{
	SDL_Event event;
	while ( SDL_PollEvent(&event) )
	{
		switch ( event.type )
		{
			case SDL_KEYDOWN:
				for ( int i = 0; i < size; i++ )
				{
					// calls the function associated with the key, if that key is pressed.
					if ( event.key.keysym.scancode == k[i].scancode )
					{
						switch ( k[i].state )
						{
							case DOWN:
								// the key has already been pressed
								break;
							case UP:
								k[i].state = UP_TO_DOWN;
								break;
							case DOWN_TO_UP:
								k[i].state = UP_TO_DOWN;
								break;
							case UP_TO_DOWN:
								// this is probably the second time a keydown event has been
								// recieved
								k[i].state = DOWN;
								break;
						}

						// since the key that was pressed was found, there is no sense continuing the loop
						break;
					}
				}
				break;

			case SDL_KEYUP:
				for ( int i = 0; i < size; i++ )
				{
					// calls the function associated with the key, if that key is pressed.
					if ( event.key.keysym.scancode == k[i].scancode )
					{
						switch ( k[i].state )
						{
							case DOWN:
								k[i].state = DOWN_TO_UP;
								break;
							case UP:
								// nothing needs to happen, but something has probably gone wrong
								break;
							case DOWN_TO_UP:
								k[i].state = UP;
								break;
							case UP_TO_DOWN:
								k[i].state = DOWN_TO_UP;
								break;
						}

						// since the key that was pressed was found, there is no sense continuing the loop
						break;
					}
				}
				break;

			default:
				break;
		}
	}

	// TODO check and make sure that this is the most efficient way to switch between UP_TO_DOWN -> DOWN,
	// and DOWN_TO_UP -> UP
	for ( int i = 0; i < size; i++ )
	{
		// calls the callback function with the current button state
		k[i].callback(k[i].state);

		// if this is the first or last calling of the callback function, then the states will need to be
		// adjusted to reflect that.
		switch ( k[i].state )
		{
			case UP_TO_DOWN:
				k[i].state = DOWN;
				break;
			case DOWN_TO_UP:
				k[i].state = UP;
				break;
			default:
				break;
		}
	}
}
