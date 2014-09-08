#include <stdio.h>
#include "vector2d.h"
#include <stdbool.h>
#include <math.h>
#include <assert.h>

Vector2D add_vector(Vector2D a, Vector2D b)
{
    Vector2D c = {a.x + b.x, a.y + b.y};
    return c;
}

Vector2D subtract_vector(Vector2D a, Vector2D b)
{
    Vector2D c = {a.x - b.x, a.y - b.y};
    return c;
}

Vector2D negate_vector(Vector2D v)
{
    Vector2D n = {-v.x, -v.y};
    return n;
}

bool equal_floats(float a, float b)
{
    float threshold = 1.0f / 8192.0f;
    return fabsf(a - b) < threshold;
}

void assert_equal_vectors(Vector2D a, Vector2D b)
{
    assert(equal_floats(a.x, b.x));
    assert(equal_floats(a.y, b.y));
}

Vector2D multiply_vector(Vector2D v, float scalar)
{
    Vector2D r = {v.x * scalar, v.y * scalar};
    return r;
}

Vector2D divide_vector(Vector2D v, float divisor)
{
    Vector2D r = {0, 0};
    if (divisor != 0) 
    {
        r.x = v.x / divisor;
        r.y = v.y / divisor;
    }
    return r;
}

float vector_length(Vector2D v)
{
    return sqrtf(v.x * v.x + v.y * v.y);
}

Vector2D unit_vector(Vector2D v)
{
    float length = vector_length(v);
    if(length > 0)
        return divide_vector(v, length);
    return v;
}

Vector2D rotate_vector(Vector2D v, float radian)
{
    float sine = sinf(radian);
    float cosine = cosf(radian);
    Vector2D r = {v.x * cosine + v.y * sine, v.x * sine + v.y * cosine};
    return r;
}

float dot_product(Vector2D a, Vector2D b)
{
    return a.x * b.x + a.y * b.y;
}

float enclosed_angle(Vector2D a, Vector2D b)
{
    Vector2D ua = unit_vector(a);
    Vector2D ub = unit_vector(b);
    float dp = dot_product(ua, ub);
    return acosf(dp);
}

Vector2D project_vector(Vector2D project, Vector2D onto)
{
    float d = dot_product(onto, onto);
    if(d > 0)
    {
        float dp = dot_product(project, onto);
        return multiply_vector(onto, dp / d);
    }
    return onto;
}

float degrees_to_radians(float degrees)
{
    float pi = 3.14159265358979323846f;
    return degrees * pi / 180.0f;
}

float radians_to_degrees(float radians)
{
    float pi = 3.14159265358979323846f;
    return radians / pi * 180.0f;
}

bool overlapping(float minA, float maxA, float minB, float maxB)
{
    return minB <= maxA && minA <= maxB;
}

bool points_collide(Vector2D a, Vector2D b)
{
    return equal_floats(a.x, b.x) && equal_floats(a.y, b.y);
}

bool rectangles_collide(Rectangle a, Rectangle b)
{
    float aLeft = a.origin.x;
    float aRight = aLeft + a.size.x;
    float bLeft = b.origin.x;
    float bRight = bLeft + b.size.x;
    float aBottom = a.origin.y;
    float aTop = aBottom + a.size.y;
    float bBottom = b.origin.y;
    float bTop = bBottom + b.size.y;
    return overlapping(aLeft, aRight, bLeft, bRight) && overlapping(aBottom, aTop, bBottom, bTop);
}

bool circles_collide(Circle a, Circle b)
{
    float radiusSum = a.radius + b.radius;
    Vector2D distance = subtract_vector(a.center, b.center);
    return vector_length(distance) <= radiusSum;
}

Vector2D rotate_vector_90(Vector2D v)
{
    Vector2D r;
    r.x = -v.y;
    r.y = v.x;
    return r;
}
bool parallel_vectors(Vector2D a, Vector2D b)
{
    Vector2D na = rotate_vector_90(a);
    return equal_floats(0, dot_product(na, b));
}

bool equal_vectors(Vector2D a, Vector2D b)
{
    return equal_floats(a.x - b.x, 0) && equal_floats(a.y - b.y, 0);
}

bool equivalent_lines(Line a, Line b)
{
    if(!parallel_vectors(a.direction, b.direction))
    {
        return false;
    }
    Vector2D d = subtract_vector(a.base, b.base);
    return parallel_vectors(d, a.direction);
}

bool lines_collide(Line a, Line b)
{
    if(parallel_vectors(a.direction, b.direction))
    {
        return equivalent_lines(a, b);
    }
        return true;
}
