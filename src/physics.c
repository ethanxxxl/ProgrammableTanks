#include <physics.h>
#include <stdlib.h>
#include <array.h>
#include <vmath.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

#include <SDL2/SDL.h>

// master list of all rigid bodies that are managed by the physics engine.
// each element is a pointer to a rigidbody.
static Array* rb_buff; /* <RigidBody*> */

// time storage for use in the loop function. it out here so that the init function can set it.
static struct timespec last_time;

// list that is used to identify RigidBodies that are colliding
// it contains pointers to arrays that contain a pair of 
static Array* rb_collisions; /* <struct Collision> > */

int physics_init()
{
	// we will start with space for 10 rigid bodies. because I like the number 10.
	rb_buff = array_init(10, sizeof(RigidBody*));

	// check if malloc was successfull.
	if ( rb_buff == NULL )
		return -1;

	// set the clock
	clock_gettime(CLOCK_REALTIME, &last_time);

	// initialize the collisions array
	rb_collisions = array_init(5, sizeof(struct Collision));

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

	// free the collisions array
	array_kill(rb_collisions);
}

RigidBody* physics_add_rigidbody()
{
	// create space for the rigidbody
	RigidBody* rb_new = malloc(sizeof(RigidBody));
	if ( rb_new == NULL ) // make sure malloc was successful
		return NULL;

	// add that space to the master list of RigidBodies
	array_add(rb_buff, &rb_new);
	rb_new->ID = array_get_size(rb_buff) - 1;

	// initialize the forces array.
	rb_new->forces = array_init(2, sizeof(Vec2));

	return rb_new;
}

// finds the largest radius of a rigid body
float rb_find_radius(RigidBody* rb)
{
	switch ( rb->bound_type )
	{
		case CIRCLE:
		{
			return rb->bounds.c.r;
			break;
		}
		case RECT:
		{
			Vec2 p = { rb->bounds.r.h, rb->bounds.r.w };
			vec2_scale(&p, 0.5f, &p);
			return vec2_get_mag(&p);
			break;
		}
		case SQUARE:
		{
			return rb->bounds.s.s * M_SQRT2 / 2;
			break;
		}
		case POLYGON:
		{
			float largest = 0;
			for ( int i = 0; i < rb->bounds.p.num_p; i++ )
			{
				float temp = vec2_get_mag(&rb->bounds.p.p[i]);
				largest = temp > largest ? temp : largest;
			}
			return largest;
			break;
		}
	}
}

static void broad_phase_collision_detection()
{
	// if we compare rb 0 with all the other rbs, we don't need to compare
	// it with any others, because we have already checked it. If they were colliding, that
	// collision pair would already have been added to the collision array.
	/* All of the RigidBodies need to be checked against each other for 
	 */
	for ( int i = 0; i < array_get_size(rb_buff); i++ )
	{
		for ( int j = i+1; j < array_get_size(rb_buff); j++ )
		{
			// get pointers to the two rigidbodies
			RigidBody* rb1 = (*(RigidBody**)array_get(rb_buff, i));
			RigidBody* rb2 = (*(RigidBody**)array_get(rb_buff, j));

			// find the distance between the two rigidbodies centers
			Vec2 dist_centers_vec;
			vec2_sub(&rb1->pos, &rb2->pos, &dist_centers_vec);
			float dist_centers = vec2_get_mag(&dist_centers_vec);


			// find the maximum distance that the objects have to be in order to
			//  not collide.
			float max_dist = rb_find_radius(rb1) + rb_find_radius(rb2);

			printf("dist_centers: %f\nmax_dist: %f\nradius: %f\n\n", dist_centers, max_dist, rb_find_radius(rb1));
			if ( dist_centers <= max_dist )
			{
				// these two objects might be colliding. Add this pair to the array.
				// TODO find the fastest way to pass this information the array.
				struct Collision c = { rb1, rb2 };
				array_add(rb_collisions, &c);
			}
		}
	}
	
}

static void narrow_phase_collision_detection();

void physics_update()
{
	/* Synopsis
	 * - Every object needs their forces summed up, to find the net force.
	 *   - This might change, when I change the way that forces are handled. (I might change
	 *     it so that the forces are just added and subtracted from the net force.)
	 * - Update the positions of each RigidBody
	 *   - The net force is used to find acceleration,
	 *   - acceleration is used to find velocity
	 *   - Velocity is used to find the new position.
	 * - Look for collisions
	 *   - there will be a broad phase and a narrow phase.
	 *   - the broad phase creates a circular bounding box around each RigidBody, and solves for
	 *     any collisions using that.
	 *   - the narrow phase uses Separating Axis Theorum to determine if the objects are actually
	 *     colliding.
	 * - Apply Dynamic Feedback
	 *   - Adjust the positions of the objects so that they do not intersect. (static feedback)
	 *   - update the colliding objects velocities to reflect that of the collision. (dynamic feedback)
	 */

	/* Calculate change in time between frames, or physics updates.
	 * (this is called delta time, or dt.)
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

	/* Collision Handling
	 */

	// sets the collisions array with all of the broad phase collisions
	broad_phase_collision_detection();

	for ( int i = 0; i < array_get_size(rb_collisions); i++ )
	{
		// placeholder for actions to take on collision
		printf("%d collisions!\n", array_get_size(rb_collisions));
	}

	// clears out old collisions
	array_reset(rb_collisions);
}
