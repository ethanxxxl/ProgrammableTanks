#ifndef VMATH_H
#define VMATH_H

typedef struct Vec2
{
	float x, y;
} Vec2;

typedef struct Vec3
{
	float x, y, z;
} Vec3;

typedef struct Vec4
{
	float x, y, z, w;
} Vec4;



/* 2 Dimensional Vector Functions
 */
float vec2_get_mag(Vec2* v);
float vec2_get_angle(Vec2* v1, Vec2* v2);

// basic operations
Vec2* vec2_add(Vec2* v1, Vec2* v2, Vec2* result);
Vec2* vec2_sub(Vec2* v1, Vec2* v2, Vec2* result);
// Vec3* vec2_cross(Vec2* v1, Vec2* v2, Vec2* result);
float vec2_dot(Vec2* v1, Vec2* v2);

Vec2* vec2_scale(Vec2* v1, float a, Vec2* result);

Vec2* vec2_rotate(Vec2* v1, float a, Vec2* result);


/* 3 Dimensional Vector Functions
 */
float vec3_get_mag(Vec2* v);
float vec3_get_angle(Vec2* v1, Vec2* v2);

Vec3* vec3_add(Vec2* v1, Vec2* v2, Vec2* result);
Vec3* vec3_sub(Vec2* v1, Vec2* v2, Vec2* result);
Vec4* vec3_cross(Vec2* v1, Vec2* v2, Vec2* result);
float vec3_dot(Vec2* v1, Vec2* v2);

Vec3* vec3_scale(Vec2* v1, float a, Vec2* result);
#endif
