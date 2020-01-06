#include <physics.h>
#include <stdlib.h>
#include <array.h>
#include <vmath.h>
#include <time.h>
#include <stdio.h>

// master list of all rigid bodies that are managed by the physics engine.
// each element is a pointer to a rigidbody.
static Array* rb_buff;

// time storage for use in the loop function. it out here so that the init function can set it.
static struct timespec last_time;

int physics_init()
{
	// we will start with space for 10 rigid bodies. because I like the number 10.
	rb_buff = array_init(10, sizeof(RigidBody*));

	// check if malloc was successfull.
	if ( rb_buff == NULL )
		return -1;

	clock_gettime(CLOCK_REALTIME, &last_time);

	return 0;
}

void physics_stop()
{
	for ( int i = 0; i < array_get_size(rb_buff); i++ )
	{
		// free the forces array. (it's complicated, I know)
		Array* a = (**(RigidBody**)array_get(rb_buff, i)).forces;
		array_kill(a);
		// free all the rigidbodies
		free(*(RigidBody**)array_get(rb_buff, i));
	}

	// free the buffer.
	array_kill(rb_buff);
}

RigidBody* physics_add_rigidbody()
{
	// create space for the rigidbody
	RigidBody* rb_new = malloc(sizeof(RigidBody));
	if ( rb_new == NULL ) // make sure malloc was successful
		return NULL;

	// add that space to the master list of RigidBodies
	array_add(rb_buff, &rb_new);

	// initialize the forces array.
	rb_new->forces = array_init(2, sizeof(Vec2));

	return rb_new;
}

void physics_update()
{
	// solve for all forces
	/* every object needs to have their forces summed up.
	 * this is the net force acting on the object.
	 *
	 * using the net force and the weight of the object, we can find the net acceleration
	 * of the object.
	 *
	 * using the acceration, we can update the velocity, and then the position
	 *
	 * ---
	 *
	 *  after updating the positions, we can check for collisions.
	 *  - if an object is colliding with another object, then the following should hapen:
	 *    - their positions should be fixed, so they aren't intersecting
	 *    - use conservation of momentum to find the velocities of each object. ( these will
	 *      be perfectly elastic collisions )
	 *    - (maybe do some rotaional stuff here)
	 *
	 */
	
	
	// find the current time.
	struct timespec current_time;
	clock_gettime(CLOCK_REALTIME, &current_time);

	// calculate the delta time between the current frame and the last frame.
	double dt = (double)current_time.tv_nsec/1000000000 - (double)last_time.tv_nsec/1000000000;
	dt += (double)current_time.tv_sec - (double)last_time.tv_sec;

	/* Update forces, velocity, and position
	 */
	for ( int i = 0; i < array_get_size(rb_buff); i++ )
	{
		RigidBody* rb = *(RigidBody**)array_get(rb_buff, i);
		Vec2 net_force = {0.0f, 0.0f};

		for ( int f = 0; f < array_get_size(rb->forces); f++ )
		{
			vec2_add(&net_force, (Vec2*)array_get(rb->forces, f), &net_force);
		}
		
		// find the net acceleration
		Vec2 net_accel;
		vec2_scale(&net_force, 1/rb->mass, &net_accel);

		// temporary variable for subsequent vector math
		Vec2 temp;

		// update velocity
		/* essentially: rb->velocity += net_accel*dt; */
		vec2_add(&rb->velocity, vec2_scale(&net_accel, dt, &temp), &rb->velocity);

		// update positon
		/* essentially: rb->pos += rb->velocity*dt; */
		vec2_add(&rb->pos, vec2_scale(&rb->velocity, dt, &temp), &rb->pos);
	}
}
