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

typedef struct Tank
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

	int _TANK_ID;
} Tank;

/* Managed Tanks will be indirectly be controlled by the user,
 * or a script. Tank management system will control the speed, acceleration,
 * and other details about a tank.
 */
void tank_manager_init();
void tank_manager_stop();

/* Updates information about all tanks.
 */
void tank_update();

// Creates a managed tank
Tank* tank_create();

// Removes a tank from management
void tank_remove(Tank* tank);

// function that draws the tank.
void tank_draw(struct SDL_Renderer* renderer);

// rotates the tanks body.
void tank_rotate(struct Tank* t, float angle);

// rotates the tanks turret.
void tank_rotate_turret(struct Tank* t, float angle);

/* Moves the tank forward at throttle percent of its maximum speed
 */
void tank_move(struct Tank* t, float throttle);

#endif
