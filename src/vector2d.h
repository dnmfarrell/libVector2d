#ifndef VECTOR2D_H
#define VECTOR2D_H
#include <stdbool.h>

// 2d vector shapes available
typedef struct
{
    float x;
    float y;
} Vector2D;

typedef struct {
    Vector2D base;
    Vector2D direction;
} Line;

typedef struct {
    Vector2D point1;
    Vector2D point2;
} LineSegment;

typedef struct {
    Vector2D center;
    float radius;
} Circle;

typedef struct {
    Vector2D origin;
    Vector2D size;
} Rectangle;

typedef struct{
    Vector2D center;
    Vector2D halfExtend;
    float rotation;
} OrientedRectangle;

// Utility functions
extern float degrees_to_radians(float degrees);
extern float radians_to_degrees(float radians);
extern bool overlapping(float minA, float maxA, float minB, float maxB);
extern bool equal_floats(float a, float b);

// Vector functions
extern Vector2D add_vector(Vector2D a, Vector2D b);
extern Vector2D subtract_vector(Vector2D a, Vector2D b);
extern Vector2D multiply_vector(Vector2D v, float scalar);
extern Vector2D divide_vector(Vector2D v, float divisor);
extern Vector2D negate_vector(Vector2D v);
extern Vector2D project_vector(Vector2D project, Vector2D onto);
extern Vector2D unit_vector(Vector2D v);
extern Vector2D rotate_vector(Vector2D v, float radian);
extern Vector2D rotate_vector_90(Vector2D v);

extern float vector_length(Vector2D v);
extern float dot_product(Vector2D a, Vector2D b);
extern float enclosed_angle(Vector2D a, Vector2D b);

extern void assert_equal_vectors(Vector2D a, Vector2D b);
extern bool parallel_vectors(Vector2D a, Vector2D b);
extern bool equal_vectors(Vector2D a, Vector2D b);

// Line functions
extern bool equivalent_lines(Line a, Line b);

// Collision functions
extern bool points_collide(Vector2D a, Vector2D b);
extern bool rectangles_collide(Rectangle a, Rectangle b);
extern bool circles_collide(Circle a, Circle b);
extern bool lines_collide(Line a, Line b);

#endif
