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

// function that draws the tank.
void tank_draw(struct SDL_Renderer* renderer, struct Tank* tank);

// TODO fill in these functions (they will need to take time into account somehow)
void tank_rotate(struct Tank* t, float angle);

// moves a tank in a line, based off of it orientation
void tank_move(struct Tank* t, float distance);

// helper function for rotating a point about another point.
struct Point rotate_point(struct Point center, struct Point arm, float angle);

#endif
