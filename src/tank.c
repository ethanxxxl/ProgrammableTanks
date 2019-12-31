#include <tank.h>
#include <SDL2/SDL.h>

// TODO ADD ROTATIONS
void draw_tank(SDL_Renderer* renderer, struct Tank* tank)
{
	// draw the bounding box
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &tank->bounding_box);

	// find the center of the tank
	int x1 = tank->bounding_box.w / 2 + tank->bounding_box.x;
	int y1 = tank->bounding_box.h / 2 + tank->bounding_box.y;

	// draw the turret
	SDL_SetRenderDrawColor(renderer, 255, 10, 10, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, x1, y1, x1, y1+10);
}

