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

#include <stdlib.h>
#include <stdbool.h>
#include <vmath.h>


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

typedef struct RigidBody
{
	union Bound_Types bounds;
	Vec2 pos;
	float rot;
	float weight;

	float velocity;

	Vec2* forces; // list of forces acting on object in newtons
	int num_forces;
} RigidBody;

void rb_add_force(RigidBody* rb);

// start and stop the physics engine
// returns 0 on success and -1 on error
int physics_init();
void physics_stop();

// update the physics engine
void physics_update();

// creates a rigidbody, and adds it to the physcis engine.
// this allows the addition of structures of types other than RigidBody,
// as long as rigidbody is the first item in the struct, and the size of the
// parent struct is passed to the function.
// if size is null, then the size of struct RigidBody will be used.
RigidBody* physics_add_rigidbody();

// TODO add support for this.
//void physics_remove_rigidbody(struct RigidBody* rb);

#endif
