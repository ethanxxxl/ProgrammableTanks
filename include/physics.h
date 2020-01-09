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
#include <array.h>


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
	struct Vec2* p; /* note that these are local points */
	int num_p;
};

union Bound_Data
{
	struct Rect r;
	struct Circle c;
	struct Square s;
	struct Polygon p;
};

enum bound_types { RECT, SQUARE, CIRCLE, POLYGON };

typedef struct RigidBody
{
	union Bound_Data bounds;
	enum bound_types bound_type;
	int ID; // this is just the RigidBodies index in the rb_buff array

	float rot;
	float mass;

	Vec2 pos;
	Vec2 velocity;

	Array* forces; // <Vec2> list of forces acting on object in newtons
} RigidBody;

struct Collision
{
	RigidBody* rb1;
	RigidBody* rb2;
};

/* Physics functions
 */
// start and stop the physics engine
// returns 0 on success and -1 on error
int physics_init();
void physics_stop();

// update the physics engine
void physics_update();


/* Creates a rigidbody in the physics engine.
 * Return: returns a pointer to that RigidBody, returns NULL of failure.
 */
RigidBody* physics_add_rigidbody();


// TODO add support for this.
//void physics_remove_rigidbody(struct RigidBody* rb);

/* RigidBody Functions
 */
float rb_find_radius(const RigidBody* rb);
void rb_add_force(RigidBody* rb);

/* Bounds Functions
 */

/* Transforms the data in the union to a polygon, provided that the union
 * is of type square or rect. The vertex data is placed in the buffer
 * pointed to by buff.
 * Notes: - It will not convert circles.
 *        - poly.p must be large enough to hold at least 4 vertices
 */
void rect_to_polygon(const union Bound_Data* bd, enum bound_types bt, struct Polygon* poly);
#endif
