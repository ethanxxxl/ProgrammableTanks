#ifndef PHYSICS_H
#define PHYSICS_H

/* Requirements
 * - handles collision detection, and correction
 * - capable of raycasting (for when the tanks fire)
 *
 * Implementation Ideas
 * - physics engine has a list of rigid bodies that are managed by the physics engine.
 * - if an object want to be handled by the physics engine, it must be created and inserted
 *   into the physcics engine by the physics engine.
 *
 */

#include <stdbool.h>

struct Point
{
	float x;
	float y;
};


/*
 * Bounding Boxes
 */

/* I chose not to include the positions with the rect, despite it being included
 * in the sdl rect. this is because these rects will have a rotation associated with
 * them. Additionally, Rects are not the only types of bounds that will be supported.
 * it doesn't make sense to include position and rotation data for each bounding box.
 */
struct Rect
{
	float h;
	float w;
};

struct Square
{
	float s;
};

struct Circle
{
	float r;
};

// these should be convex polygons.
struct Polygon
{
	struct Point* p; /* note that these are local points */
	int num_p;
};

union Bound_Types
{
	struct Rect r;
	struct Circle c;
	struct Square s;
	struct Polygon p;
};

struct RigidBody
{
	union Bound_Types bounds;
	struct Point pos;
	float rot;
	float weight;

	bool can_collide;
};

// start and stop the physics engine
// returns 0 on success and -1 on error
int physics_init();
int physics_stop();

// update the physics engine
void physics_update();

// creates a rigidbody, and adds it to the physcis engine.
void physics_add_rigidbody(struct RigidBody** rb);
//void physics_remove_rigidbody(struct RigidBody* rb);

#endif
