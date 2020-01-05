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
float vec2_get_mag(const Vec2* v);
float vec2_get_angle(const Vec2* v1, const Vec2* v2);

// basic operations
Vec2 vec2_add(const Vec2* v1, const Vec2* v2);
Vec2 vec2_sub(const Vec2* v1, const Vec2* v2);
// Vec3* vec2_cross(Vec2* v1, Vec2* v2, Vec2* result);
float vec2_dot(const Vec2* v1, const Vec2* v2);

Vec2 vec2_scale(const Vec2* v1, float a);

Vec2 vec2_rotate(const Vec2* v1, float a);


/* 3 Dimensional Vector Functions
 */
float vec3_get_mag(const Vec3* v);
float vec3_get_angle(const Vec3* v1, const Vec2* v2);

Vec3 vec3_add(const Vec3* v1, const Vec3* v2);
Vec3 vec3_sub(const Vec3* v1, const Vec3* v2);
Vec3 vec3_cross(const Vec3* v1, const Vec3* v2);
float vec3_dot(const Vec3* v1);

Vec3 vec3_scale(const Vec3* v1, float a);
#endif
