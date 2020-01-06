#ifndef TANK_H
#define TANK_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <physics.h>

// the rotation speed of the turret in degrees/sec
#define TURRET_ROT_SPEED 10

// the move speed of the tank in points/sec
#define TANK_MOV_SPEED 10
#define TANK_ACCELERATION 1

struct Tank
{
	// managed rigidbody used by the physics engine.
	RigidBody* rb;

	float turret_angle;

	// this is the max speed. the max rotation speed is a function
	// of this and the width of the tank.
	float max_speed;
	float max_turret_speed;

	// fire rate measured in rounds per second
	float fire_rate;
	
	// the number of rounds left to shoot. this value should be -1 if infinity.
	int rounds;

	bool destroyed;
};

// function that draws the tank.
void tank_draw(struct SDL_Renderer* renderer, struct Tank* tank);

// rotates the tanks body.
void tank_rotate(struct Tank* t, float angle);

// rotates the tanks turret.
void tank_rotate_turret(struct Tank* t, float angle);

// moves a tank in a line, based off of it orientation
void tank_move(struct Tank* t, float distance);

#endif
