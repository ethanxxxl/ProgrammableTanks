#ifndef TANK_H
#define TANK_H

#include <SDL2/SDL.h>
#include <stdbool.h>

// the rotation speed of the turret in degrees/sec
#define TURRET_ROT_SPEED 10

// the move speed of the tank in points/sec
#define TANK_MOV_SPEED 10
#define TANK_ACCELERATION 1

struct Point
{
	float x;
	float y;
};

struct Rect
{
	float x;
	float y;
	float h;
	float w;
};

struct Tank
{
	// possition and orientation
	struct Rect bounds;
	float rot;
	float turret_angle;

	bool destroyed;
};


void draw_tank(struct SDL_Renderer* renderer, struct Tank* tank);

// TODO fill in these functions (they will need to take time into account somehow)
void rotate_tank();
void move_tank();

struct Point rotate_point(struct Point center, struct Point arm, float angle);

#endif
