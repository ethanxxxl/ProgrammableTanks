#include <physics.h>
#include <stdlib.h>
#include <array.h>

// master list of all rigid bodies that are managed by the physics engine.
// each element is a pointer to a rigidbody.
Array* rb_buff;

int physics_init()
{
	// we will start with space for 10 rigid bodies. because I like the number 10.
	rb_buff = array_init(10, sizeof(RigidBody**));

	// check if malloc was successfull.
	if ( rb_buff == NULL )
		return -1;

	return 0;
}



void physics_stop()
{
	for ( int i = 0; i < array_get_size(rb_buff); i++ )
{
		// free all the rigidbodies
		free(*(RigidBody**)array_get(rb_buff, i));
	}

	// free the buffer.
	array_kill(rb_buff);
}

RigidBody* physics_add_rigidbody()
{
	RigidBody* rb_new = malloc(sizeof(RigidBody));
	if ( rb_new == NULL ) // make sure malloc was successful
		return NULL;

	array_add(rb_buff, &rb_new);
	
	return rb_new;
}

void physics_update()
{
	// solve for all forces
// update positions
// check for collisions
// move objects and update forces.

}
