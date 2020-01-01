#include <tank.h>
#include <SDL2/SDL.h>
#include <math.h>

void draw_tank(SDL_Renderer* renderer, struct Tank* t)
{
	struct Point points[5] =
	{
		{ t->bounds.w / 2 + t->bounds.x , t->bounds.h / 2 + t->bounds.y },
		{ -t->bounds.w / 2 + t->bounds.x , t->bounds.h / 2 + t->bounds.y },
		{ -t->bounds.w / 2 + t->bounds.x , -t->bounds.h / 2 + t->bounds.y },
		{ t->bounds.w / 2 + t->bounds.x , -t->bounds.h / 2 + t->bounds.y },
		{ t->bounds.w / 2 + t->bounds.x , t->bounds.h / 2 + t->bounds.y }
	};

	SDL_Point sdl_points[5];

	for ( int i = 0; i < 5; i++ )
	{
//		sdl_points[i] =
//		(SDL_Point) {
//			(points[i].x - t->bounds.x)*cos(t->rot)*(t->rot) - (points[i].y - t->bounds.y)*sin(t->rot)*(t->rot) + t->bounds.x,
//			(points[i].x - t->bounds.x)*sin(t->rot)*(t->rot) - (points[i].y - t->bounds.y)*cos(t->rot)*(t->rot) + t->bounds.y
//		};
//
		struct Point p = rotate_point((struct Point){t->bounds.x, t->bounds.y}, points[i], t->rot);

		sdl_points[i].x = p.x;
		sdl_points[i].y = p.y;
		
		printf("%d: %d, %d \n", i, sdl_points[i].x, sdl_points[i].y);
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLines(renderer, sdl_points, 5);
	SDL_RenderDrawPoint(renderer, t->bounds.x, t->bounds.y);
	SDL_SetRenderDrawColor(renderer, 255, 0,0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawPoints(renderer, sdl_points, 4);

}

struct Point rotate_point(struct Point center, struct Point arm, float angle)
{
	return
	(struct Point){
		cos(angle)*(arm.x-center.x) - sin(angle)*(arm.y-center.y) + center.x,
		sin(angle)*(arm.x-center.x) + cos(angle)*(arm.y-center.y) + center.y
	};
}
