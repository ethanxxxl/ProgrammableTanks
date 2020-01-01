#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <tank.h>
#include <time.h>

#define WIN_SIZE_X 400
#define WIN_SIZE_Y 350

bool run_game = true;

void handle_input()
{
	SDL_Event event;
	while ( SDL_PollEvent(&event) )
	{
		switch ( event.type )
		{
			case SDL_KEYDOWN:
				if ( event.key.keysym.scancode == SDL_SCANCODE_Q )
					run_game = false;
				break;

			default:
				break;
		}
	}
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
	struct Tank t1;
	t1.bounds = (struct Rect){ 200, 200, 100, 150 };

	while ( run_game )
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);


		t1.rot = 3*(float)clock()/CLOCKS_PER_SEC;

		printf("%lf\n", t1.rot);
		draw_tank(renderer, &t1);

		SDL_RenderPresent(renderer);
		handle_input();
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
