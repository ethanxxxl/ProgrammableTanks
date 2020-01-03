#include <physics.h>
#include <stdlib.h>

// master list of all rigid bodies that are managed by the physics engine.
static struct RigidBody* rb_buff;
static int rb_buff_size;
static int rb_buff_current_empty;

int physics_init()
{
	// we will start with space for 10 rigid bodies. because I like the number 10.
	rb_buff_size = 10;
	rb_buff_current_empty= 0;
	rb_buff = malloc(sizeof(struct RigidBody*) * rb_buff_size);
	return 0;
}

int physics_stop()
{
	return 0;
}

void physics_update()
{
	// this is where the real meaty suff is going to happen.
}

void physics_add_rigidbody(struct RigidBody** rb)
{
	if ( rb_buff_current_empty >= rb_buff_size )
	{
		// this doubles the size of the buffer. This decreases the amount of times that
		// realloc will need to be called.
		rb_buff = realloc(rb_buff, sizeof(struct RigidBody*)*rb_buff_size*2);
	}

	*rb = rb_buff + rb_buff_current_empty;
	rb_buff_current_empty++;
}
