#include <physics.h>
#include <stdlib.h>
#include <array.h>
#include <vmath.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

// master list of all rigid bodies that are managed by the physics engine.
// each element is a pointer to a rigidbody.
static Array* rb_buff; /* <RigidBody*> */

// time storage for use in the loop function. it out here so that the init function can set it.
static struct timespec last_time;

// list that is used to identify RigidBodies that are colliding
// it contains pointers to arrays that contain a pair of 
static Array* rb_collisions_broad; /* <struct Collision> > */
static Array* rb_collisions_narrow;

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
	rb_collisions_broad = array_init(5, sizeof(struct Collision));
	rb_collisions_narrow = array_init(5, sizeof(struct Collision));

	return 0;
}

void physics_stop()
{
	for ( int i = 0; i < array_get_size(rb_buff); i++ )
	{
		// just call this on every RigidBody, so that there doesn't have to be
		// repeat code
		physics_remove_rigidbody(*(RigidBody**)array_get(rb_buff, i));
	}

	// free the buffer.
	array_kill(rb_buff);

	// free the collisions array
	array_kill(rb_collisions_broad);
	array_kill(rb_collisions_narrow);
}

RigidBody* physics_add_rigidbody()
{
	// create space for the rigidbody
	RigidBody* rb_new = calloc(1, sizeof(RigidBody));
	if ( rb_new == NULL ) // make sure malloc was successful
		return NULL;

	// add that space to the master list of RigidBodies
	// also sets the ID for the RigidBody, so that it can be removed.
	rb_new->_ID = array_add(rb_buff, &rb_new);

	// initialize the forces array.
	rb_new->forces = array_init(2, sizeof(Vec2));

	return rb_new;
}

void physics_remove_rigidbody(RigidBody* rb)
{
	// remove rb from the physics RigidBody buffer
	array_remove(rb_buff, rb->_ID);
	// kill the forces array
	array_kill(rb->forces);
	// free the space allocated for the RigidBody
	free(rb);
}

// finds the largest radius of a rigid body
float rb_find_radius(const RigidBody* rb)
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

void rect_to_polygon(const union Bound_Data* bd, enum bound_types bt, struct Polygon* poly)
{
	switch ( bt )
	{
		case RECT:
		{
			float h = bd->r.h/2;
			float w = bd->r.w/2;
			 poly->p[0] = (Vec2){ -w,  h };
			 poly->p[1] = (Vec2){  w,  h };
			 poly->p[2] = (Vec2){  w, -h };
			 poly->p[3] = (Vec2){ -w, -h };
			break;
		}
		case SQUARE:
		{
			float s = bd->s.s/2;
			poly->p[0] = (Vec2){ -s,  s };
			poly->p[1] = (Vec2){  s,  s };
			poly->p[2] = (Vec2){  s, -s };
			poly->p[3] = (Vec2){ -s, -s };
			break;
		}
		default:
			return;
			break;
	}

	poly->num_p = 4;
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

			//printf("dist_centers: %f\nmax_dist: %f\nradius: %f\n\n", dist_centers, max_dist, rb_find_radius(rb1));
			if ( dist_centers <= max_dist )
			{
				// these two objects might be colliding. Add this pair to the array.
				// TODO find the fastest way to pass this information the array.
				struct Collision c = { rb1, rb2 };
				array_add(rb_collisions_broad, &c);
			}
		}
	}
	
}

/* Synopsis:
 * This function goes through rb_collisions_broad and uses a better test to see if the two objects are
 * indeed colliding. The function uses Separating Axis Theorum to determine this.
 */
static void narrow_phase_collision_detection()
{
	for ( int i = 0; i < array_get_size(rb_collisions_broad); i++ )
	{
		// This is the pair of rigidbodies that will be tested
		struct Collision pair = *(struct Collision*)array_get(rb_collisions_broad, i);

		// Because rigidbodies can have four type of bounding boxes, it is simpler to
		// convert each type to a polygon, and copy it into here.
		Vec2 rb1_buff[4];
		Vec2 rb2_buff[4];
		struct Polygon poly1;
		struct Polygon poly2;

		switch ( pair.rb1->bound_type )
		{
			case RECT:
			{
				poly1 = (struct Polygon){ .p = rb1_buff };
				rect_to_polygon(&pair.rb1->bounds, RECT, &poly1);
				break;
			}
			case SQUARE:
			{
				poly1 = (struct Polygon){ .p = rb1_buff };
				rect_to_polygon(&pair.rb1->bounds, SQUARE, &poly1);
				break;
			}
			case CIRCLE:
			{
				continue; // the broad phase already calculated this one
				break;
			}
			case POLYGON:
			{
				poly1 = pair.rb1->bounds.p;
				break;
			}
		}
		
		// do the same thing as above for the second object
		switch ( pair.rb2->bound_type )
		{
			case RECT:
			{
				struct Polygon poly2 = (struct Polygon){ .p = rb2_buff };
				rect_to_polygon(&pair.rb2->bounds, RECT, &poly2);
				break;
			}
			case SQUARE:
			{
				struct Polygon poly2 = (struct Polygon){ .p = rb2_buff };
				rect_to_polygon(&pair.rb2->bounds, SQUARE, &poly2);
				break;
			}
			case CIRCLE:
			{
				continue;
				break;
			}
			case POLYGON:
			{
				poly2 = pair.rb2->bounds.p;
				break;
			}
		}

		// create pointers to the polygons, so that they can be easily switched later on.
		struct Polygon* p1 = &poly1;
		struct Polygon* p2 = &poly2;

		// transform each polygon so that they are positioned
		// in the same places as their rigid bodies
		for ( int point = 0; point < p1->num_p; point++ )
		{
			// rotate
			Vec2 temp;
			vec2_rotate(p1->p+point, pair.rb1->rot, &temp);
			p1->p[point] = temp;

			// translate
			vec2_add(p1->p+point, &pair.rb1->pos, p1->p+point);
		}
		
		for ( int point = 0; point < p2->num_p; point++ )
		{
			// rotate
			Vec2 temp;
			vec2_rotate(p2->p+point, pair.rb2->rot, &temp);
			p2->p[point] = temp;

			// translate
			vec2_add(p2->p+point, &pair.rb2->pos, p2->p+point);
		}

		// go through each object, so that the shadows of each object can
		// be found on every axis from both objects.
		bool colliding = true;
		for ( int object = 0; object < 2; object++ )
		{
			// flip the objects and run again
			if ( object == 1 )
			{
				p1 = &poly2;
				p2 = &poly1;
			}

			/* loop through all axis of rb1
			 * for every axis in rb1
			 *   for every point in rb2
			 *     find the dot between the axis and the point
			 *     find the min and max point
			 *   for every point in rb1
			 *     find the dot between the axis and the point
			 *     find the min and max point
			 *   detect if the two projections are overlaping
			 */
			for ( int a = 0; a < p1->num_p; a++ )
			{
				int b = (a + 1) % p1->num_p;  // <- lets the points wrap around
				Vec2 axis =                   // to complete the last edge.
				{
					-(p1->p[a].y - p1->p[b].y),  // swap the x and the y to
					p1->p[a].x - p1->p[b].x      // get the normal.
				};

				/* Find the shadow made by poly1 (rb1)
				 */
				float rb1_min =  INFINITY;  // setting the mins and maxs so that that they are
				float rb1_max = -INFINITY;  // gaurunteed to have proper values after iterations.
				for ( int p = 0; p < p1->num_p; p++ )
				{
					// find the "shadow" made by the point
					float temp = vec2_dot(p1->p+p, &axis);

					// set the mins and maxs
					rb1_min = (temp < rb1_min) ? temp : rb1_min;
					rb1_max = (temp > rb1_max) ? temp : rb1_max;
				}

				/* Find the shadow made by poly2 (rb2)
				 */
				float rb2_min =  INFINITY;  // setting the mins and maxs so that that they are
				float rb2_max = -INFINITY;  // gaurunteed to have proper values after iterations.
				for ( int p = 0; p < p2->num_p; p++ )
				{
					// find the "shadow" made by the point
					float temp = vec2_dot(p2->p+p, &axis);

					// set the mins and maxs
					rb2_min = (temp < rb2_min) ? temp : rb2_min;
					rb2_max = (temp > rb2_max) ? temp : rb2_max;
				}
				// detects if the shadows are intersecting or not.
				// returns true if they are not intersecting.
				if ( rb1_min >= rb2_max || rb1_max <= rb2_min )
				{
					colliding = false;
					break;
				}
			}
		}

		// if the algorithm made it through the axis of both objects without finding any gaps between
		// the shadows, then the objects are colliding.
		if ( colliding )
		{
			array_add(rb_collisions_narrow, &pair);
			break;
		}
	}
}

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
	// finds objects that could be colliding
	broad_phase_collision_detection();
	
	// sets the narrow phase array with objects that are actually colliding
	narrow_phase_collision_detection();

	for ( int i = 0; i < array_get_size(rb_collisions_narrow); i++ )
	{
		// placeholder for actions to take on collision
		printf("%d collisions!\n", array_get_size(rb_collisions_narrow));
	}

	// clears out old collisions
	array_reset(rb_collisions_broad);
	array_reset(rb_collisions_narrow);
}
