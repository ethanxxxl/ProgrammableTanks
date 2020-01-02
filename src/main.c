#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <tank.h>
#include <time.h>
#include <handle_input.h>

#define WIN_SIZE_X 400
#define WIN_SIZE_Y 350

bool run_game = true;

struct Tank t1;

// TODO figure out how to do this properly, so that there aren't any jumps or crap like that.
void movement_helper(void (*movement_callback)(struct Tank* t, float x), float x, struct timespec* last_time, key_state toggle)
{
	if ( toggle == DOWN )
	{
		// find the current time.
		struct timespec current_time;
		clock_gettime(CLOCK_REALTIME, &current_time);


		// calculate the delta time between the current frame and the last frame.
		double dt = (double)current_time.tv_nsec/1000000000 - (double)last_time->tv_nsec/1000000000;
		dt += (double)current_time.tv_sec - (double)last_time->tv_sec;

	//	printf("%lf, %ld.%ld\n", dt, last_time.tv_sec, last_time.tv_nsec/1000000000);

		// move the tank.
		movement_callback(&t1, x*dt);

		// update last_time.
		*last_time = current_time;
		
	}
	else if ( toggle == UP_TO_DOWN )
	{
		// if this is the first iteration through the function, then update last_time, so that on the next iteration,
		// it gets the time between frames, and not the time between the the last key press and the current one.
		clock_gettime(CLOCK_REALTIME, last_time);
	}
}

void t1_w(key_state toggle)
{
	static struct timespec last_time;
	movement_helper(&tank_move, 50, &last_time, toggle);
}

void t1_s(key_state toggle)
{
	static struct timespec last_time;
	movement_helper(&tank_move, -50, &last_time, toggle);
}

void t1_a(key_state toggle)
{
	static struct timespec last_time;
	movement_helper(&tank_rotate, -1, &last_time, toggle);
}

void t1_d(key_state toggle)
{
	static struct timespec last_time;
	movement_helper(&tank_rotate, 1, &last_time, toggle);
}

void t1_j(key_state toggle)
{
	static struct timespec last_time;
	movement_helper(&tank_rotate_turret, -1, &last_time, toggle);
}

void t1_k(key_state toggle)
{
	static struct timespec last_time;
	movement_helper(&tank_rotate_turret, 1, &last_time, toggle);
}

void quit(key_state toggle)
{
	if ( toggle == DOWN )
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
		{ SDL_SCANCODE_Q, &quit, UP },
		{ SDL_SCANCODE_W, &t1_w, UP },
		{ SDL_SCANCODE_A, &t1_a, UP },
		{ SDL_SCANCODE_S, &t1_s, UP },
		{ SDL_SCANCODE_D, &t1_d, UP },
		{ SDL_SCANCODE_J, &t1_j, UP },
		{ SDL_SCANCODE_K, &t1_k, UP }
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
