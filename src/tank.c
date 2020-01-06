#include <tank.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <physics.h>

// TODO add logic to draw the tank differently, depending on the team,
// and whether or not is is destroyed
void tank_draw(SDL_Renderer* renderer, struct Tank* t)
{
	Vec2 points[5] =
	{
		{ t->rb->bounds.r.w / 2 , t->rb->bounds.r.h / 2 },
		{ -t->rb->bounds.r.w / 2 , t->rb->bounds.r.h / 2 },
		{ -t->rb->bounds.r.w / 2 , -t->rb->bounds.r.h / 2 },
		{ t->rb->bounds.r.w / 2 , -t->rb->bounds.r.h / 2 },
		{ t->rb->bounds.r.w / 2 , t->rb->bounds.r.h / 2 }
	};

	// tank body points (in sdl format(integers))
	SDL_Point sdl_points[5];

	// rotating and filling in the tank body points
	for ( int i = 0; i < 5; i++ )
	{
		Vec2 p;
		vec2_rotate(points+i, t->rb->rot, &p);
		p.x += t->rb->pos.x;
		p.y += t->rb->pos.y;

		sdl_points[i].x = p.x;
		sdl_points[i].y = p.y;
	}

	Vec2 turret = {0, t->rb->bounds.r.h/2};
	vec2_rotate(&turret, t->turret_angle, &turret);
	turret.x += t->rb->pos.x;
	turret.y += t->rb->pos.y;

	// draw body
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLines(renderer, sdl_points, 5);

	// draw gun
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, t->rb->pos.x, t->rb->pos.y, turret.x, turret.y);
}

void tank_move(struct Tank* t, float distance)
{
	t->rb->pos.x -= sin(t->rb->rot)*distance;
	t->rb->pos.y += cos(t->rb->rot)*distance;
}

void tank_rotate(struct Tank* t, float angle)
{
	t->rb->rot += angle;
	t->turret_angle += angle;
}

void tank_rotate_turret(struct Tank* t, float angle)
{
	t->turret_angle += angle;
}
