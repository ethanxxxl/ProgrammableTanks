#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <tank.h>
#include <time.h>

#define WIN_SIZE_X 400
#define WIN_SIZE_Y 350

bool run_game = true;

// associates a scancode with a function pointer.
struct Key_Callback
{
	SDL_Scancode scancode;
	void (*callback)(void);
	bool enabled;
};

void handle_input(struct Key_Callback* k, int size);

struct Tank t1;

void t1_w(void)
{
	// pt / s 
	static clock_t last_time = 0;
	clock_t current_time = clock();
	tank_move(&t1, 10/((float)(current_time - last_time)/CLOCKS_PER_SEC));

	printf("%f\n", 10/((float)(current_time - last_time)/CLOCKS_PER_SEC));
	last_time = current_time;
}

void t1_s(void)
{
	tank_move(&t1, -5);
}

void t1_a(void)
{
	t1.rot -= 0.1;
}

void t1_d(void)
{
	t1.rot += 0.1;
}

void t1_j(void)
{
	t1.turret_angle += 0.1;
}

void t1_k(void)
{
	t1.turret_angle -= 0.1;
}

void quit(void)
{
	run_game = false;
}

int main()
{
	int err;

	err = SDL_Init(SDL_INIT_VIDEO);
	if ( err != 0 )
	{
		printf("There was an error during initialization: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	/* Window Creation
	 */
	SDL_Window *window;
	window = SDL_CreateWindow(
			"Programmable Tanks",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			WIN_SIZE_X,
			WIN_SIZE_Y,
			SDL_WINDOW_OPENGL
			);
	if ( window == NULL )
	{
		printf("Could not create window: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	/* Renderer Stuff
	 */
	SDL_Renderer* renderer;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


	/* tank stuff
	 */
	t1.bounds = (struct Rect){ 200, 200, 50, 35 };
	t1.turret_angle = M_PI/4;



	// this is the list of keys and functions associated with them.
	#define NUM_KEYS 7
	struct Key_Callback keys[NUM_KEYS] =
	{
		{ SDL_SCANCODE_Q, &quit, false },
		{ SDL_SCANCODE_W, &t1_w, false },
		{ SDL_SCANCODE_A, &t1_a, false },
		{ SDL_SCANCODE_S, &t1_s, false },
		{ SDL_SCANCODE_D, &t1_d, false },
		{ SDL_SCANCODE_J, &t1_j, false },
		{ SDL_SCANCODE_K, &t1_k, false }
	};

	t1.rot = 0;
	t1.turret_angle = 0;

	while ( run_game )
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		//t1.rot = 3*(float)clock()/CLOCKS_PER_SEC;
		//t1.turret_angle = 4*(float)clock()/CLOCKS_PER_SEC;
		

		tank_draw(renderer, &t1);

		SDL_RenderPresent(renderer);
		handle_input(keys, NUM_KEYS);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}


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
						k[i].enabled = true;
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
						k[i].enabled = false;
						break;
					}
				}
				break;

			default:
				break;
		}
	}

	for ( int i = 0; i < size; i++ )
	{
		if ( k[i].enabled )
			k[i].callback();
	}
}
