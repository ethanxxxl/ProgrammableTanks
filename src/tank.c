#include <tank.h>
#include <SDL2/SDL.h>
#include <math.h>

void tank_draw(SDL_Renderer* renderer, struct Tank* t)
{
	// tank body points
	struct Point points[5] =
	{
		{ t->bounds.w / 2 + t->bounds.x , t->bounds.h / 2 + t->bounds.y },
		{ -t->bounds.w / 2 + t->bounds.x , t->bounds.h / 2 + t->bounds.y },
		{ -t->bounds.w / 2 + t->bounds.x , -t->bounds.h / 2 + t->bounds.y },
		{ t->bounds.w / 2 + t->bounds.x , -t->bounds.h / 2 + t->bounds.y },
		{ t->bounds.w / 2 + t->bounds.x , t->bounds.h / 2 + t->bounds.y }
	};

	// tank body points (in sdl format(integers))
	SDL_Point sdl_points[5];

	// rotating and filling in the tank body points
	for ( int i = 0; i < 5; i++ )
	{
		struct Point p = rotate_point((struct Point){t->bounds.x, t->bounds.y}, points[i], t->rot);

		sdl_points[i].x = p.x;
		sdl_points[i].y = p.y;
	}

	struct Point gun = rotate_point(
			(struct Point){t->bounds.x, t->bounds.y},
			(struct Point){t->bounds.x, t->bounds.y+t->bounds.h/2}, t->turret_angle
			);

	// draw body
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLines(renderer, sdl_points, 5);

	// draw gun
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, t->bounds.x, t->bounds.y, gun.x, gun.y);
}

void tank_move(struct Tank* t, float distance)
{
	t->bounds.x -= sin(t->rot)*distance;
	t->bounds.y += cos(t->rot)*distance;
}

struct Point rotate_point(struct Point center, struct Point arm, float angle)
{
	return
	(struct Point){
		cos(angle)*(arm.x-center.x) - sin(angle)*(arm.y-center.y) + center.x,
		sin(angle)*(arm.x-center.x) + cos(angle)*(arm.y-center.y) + center.y
	};
}
