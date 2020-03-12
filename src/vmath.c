#include <vmath.h>
#include <math.h>
#include <stdio.h>

const Vec2 Y_AXIS = {0, 1};
const Vec2 X_AXIS = {0, 1};

/* 2 Dimensional Vector Implementation
 */
float vec2_get_mag(const Vec2* v)
{
	return sqrtf(pow(v->x, 2) + pow(v->y, 2));
}

float vec2_get_angle(const Vec2* v1, const Vec2* v2)
{
	float angle = acosf(vec2_dot(v1, v2)/(vec2_get_mag(v1)*vec2_get_mag(v2)));
	return !isnanf(angle) ? angle : 0;
}

Vec2* vec2_add(const Vec2* v1, const Vec2* v2, Vec2* result)
{
	result->x = v1->x + v2->x;
	result->y = v1->y + v2->y;
	return result;
}

Vec2* vec2_sub(const Vec2* v1, const Vec2* v2, Vec2* result)
{
	result->x = v1->x - v2->x;
	result->y = v1->y - v2->y;
	return result;
}

float vec2_dot(const Vec2* v1, const Vec2* v2)
{
	return (v1->x * v2->x) + (v1->y * v2->y);
}

Vec2* vec2_scale(const Vec2* v1, float a, Vec2* result)
{
	result->x = v1->x * a;
	result->y = v1->y * a;
	return result;
}

Vec2* vec2_rotate(const Vec2* v1, float a, Vec2* result)
{
	result->x = cos(a)*(v1->x) - sin(a)*(v1->y);
	result->y = sin(a)*(v1->x) + cos(a)*(v1->y);
	return result;
}

Vec2* vec2_get_unitv(const Vec2* v1, Vec2* result)
{
	return vec2_scale(v1, 1/vec2_get_mag(v1), result);
}

/* 3 Dimensional Vector Implementation
 */
// TODO add Vec3 implementation;
