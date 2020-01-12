#include <tank.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <physics.h>
#include <array.h>
#include <time.h>

static Array* tank_buffer; /* <Tank*> */

// TODO try to consolidate all of these last_time variables into one that gets updated
// in the main function
// time storage for use in the loop function. it out here so that the init function
// can set it.
static struct timespec last_time;

void tank_manager_init()
{
	// start the tank buffer
	tank_buffer = array_init(10, sizeof(Tank*));

	// set the time;
	clock_gettime(CLOCK_REALTIME, &last_time);
}

void tank_manager_stop()
{
	// kill the tank buffer, and any tanks that are left in it.
	for ( int i = 0; i < array_get_size(tank_buffer); i++ )
	{
		tank_remove(*(Tank**)array_get(tank_buffer, i));
	}
	array_kill(tank_buffer);
}

void tank_update()
{
	for ( int t = 0; t < array_get_size(tank_buffer); t++ )
	{
		// if a tank is signaled to move forward, then there has to be a ramp
		// up process to full power. if the signal is removed, then there
		// has to be a ramp down process to zero power. 
		// do updates on tank.
	}
}

Tank* tank_create()
{
	// create space for the new tank struct
	// it is initialized to 0 so weird things don't happen.
	Tank* new_tank = calloc(1, sizeof(Tank));
	
	// adds the tank to the tank manager array.
	// gives the tank its ID so it can be removed.
	new_tank->_TANK_ID = array_add(tank_buffer, &new_tank);

	// create a RigidBody, managed by the physics engine, for the tank.
	*new_tank =
	(Tank){
		.rb = physics_add_rigidbody(),
		.max_speed = 50,
		.max_turret_speed = 50,
		.fire_rate = 2,
		.rounds = -1,
		.destroyed = false,
	};

	// default RigidBody settings
	new_tank->rb->bounds.r = (struct Rect){ 50, 35 };
	new_tank->rb->bound_type = RECT;
	new_tank->rb->rot = 0;
	new_tank->rb->mass = 100;
	new_tank->rb->pos = (Vec2){ 0, 0 };
	new_tank->rb->velocity = (Vec2){ 0, 0 };
	new_tank->rb->coef_friction_k = 1.0f;
	new_tank->rb->coef_friction_s = 1.0f;
	new_tank->rb->force_external = (Vec2){ 0.0f, 0.0f };

	return new_tank;
}

void tank_remove(Tank* tank)
{
	// remove the tank from the tank buffer
	array_remove(tank_buffer, tank->_TANK_ID);
	// remove its rigidbody from the physics engine.
	physics_remove_rigidbody(tank->rb);
	// free the space that was allocated to it.
	free(tank);
}



// TODO add logic to draw the tank differently, depending on the team,
// and whether or not is is destroyed
void tank_draw(SDL_Renderer* renderer)
{
	// loop through all the tanks.
	for ( int tank = 0; tank < array_get_size(tank_buffer); tank++ )
	{
		// the current tank that will be drawn.
		Tank* t = *(Tank**)array_get(tank_buffer, tank);

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

		// there needs to be a temporary point for the rotation.
		Vec2 tmp;
		Vec2 turret = {0, t->rb->bounds.r.h/2};
		vec2_rotate(&turret, t->turret_angle, &tmp);
		vec2_add(&tmp, &t->rb->pos, &tmp);

		// draw body
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLines(renderer, sdl_points, 5);

		// draw turret
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, t->rb->pos.x, t->rb->pos.y, tmp.x, tmp.y);
	}
}

void tank_move(struct Tank* t, float throttle)
{
	const float power = 981*throttle;
	t->rb->force_external.y = cos(t->rb->rot)*power;
	t->rb->force_external.x = -sin(t->rb->rot)*power;
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
