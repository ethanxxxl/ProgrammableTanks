#include <vmath.h>
#include <math.h>

/* 2 Dimensional Vector Implementation
 */
float vec2_get_mag(Vec2* v)
{
	return sqrtf(pow(v->x, 2) + pow(v->y, 2));
}

float vec2_get_angle(Vec2* v1, Vec2* v2)
{
	return atanf(v1->y / v1->x) - atanf(v2->y / v2->x);
}

Vec2* vec2_add(Vec2* v1, Vec2* v2, Vec2* result)
{
	result->x = v1->x + v2->x;
	result->y = v1->y + v2->y;
	return result;
}
Vec2* vec2_sub(Vec2* v1, Vec2* v2, Vec2* result)
{
	result->x = v1->x - v2->x;
	result->y = v1->y - v2->y;
	return result;
}
float vec2_dot(Vec2* v1, Vec2* v2)
{
	return vec2_get_mag(v1) * vec2_get_mag(v2) * cos(vec2_get_angle(v1, v2));
}

Vec2* vec2_scale(Vec2* v1, float a, Vec2* result)
{
	result->x = v1->x * a;
	result->y = v1->y * a;
	return result;
}

Vec2* vec2_rotate(Vec2* v1, float a, Vec2* result)
{
	result->x = cos(a)*(v1->x) - sin(a)*(v1->y);
	result->y = sin(a)*(v1->x) + cos(a)*(v1->y);
	return result;
}

/* 3 Dimensional Vector Implementation
 */
// TODO add Vec3 implementation;
