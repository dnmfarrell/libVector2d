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

typedef struct
{
    float minimum;
    float maximum;
} Range;

// Utility functions
extern float degrees_to_radians(float degrees);
extern float radians_to_degrees(float radians);
extern bool overlapping(float minA, float maxA, float minB, float maxB);
extern bool equal_floats(float a, float b);
extern float minimum(float a, float b);
extern float maximum(float a, float b);

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
extern bool on_one_side(Line axis, LineSegment s);

// Range functions
extern Range sort_range(Range r);
extern Range project_segment(LineSegment s, Vector2D onto);
extern bool overlapping_ranges(Range a, Range b);
extern Range range_hull(Range a, Range b);
extern float clamp_on_range(float x, float min, float max);

// Rectangle functions
extern LineSegment oriented_rectangle_edge(OrientedRectangle r, int nr);
extern bool separating_axis_for_oriented_rectangle(LineSegment axis, OrientedRectangle r);
extern Vector2D clamp_on_rectangle(Vector2D p, Rectangle r);
extern Vector2D oriented_rectangle_corner(OrientedRectangle r, int nr);
extern Rectangle enlarge_rectangle_point(Rectangle r, Vector2D p);
extern Rectangle oriented_rectangle_rectangle_hull(OrientedRectangle r);

// Collision functions
extern bool points_collide(Vector2D a, Vector2D b);
extern bool rectangles_collide(Rectangle a, Rectangle b);
extern bool circles_collide(Circle a, Circle b);
extern bool lines_collide(Line a, Line b);
extern bool segments_collide(LineSegment a, LineSegment b);
extern bool oriented_rectangles_collide(OrientedRectangle a, OrientedRectangle b);
extern bool circle_point_collide(Circle c, Vector2D p);
extern bool circle_line_collide(Circle c, Line l);
extern bool circle_segment_collide(Circle c, LineSegment s);
extern bool circle_rectangle_collide(Circle c, Rectangle r);
extern bool circle_oriented_rectangle_collide(Circle c, OrientedRectangle r);
extern bool point_rectangle_collide(Vector2D p, Rectangle r);
extern bool line_rectangle_collide(Line l, Rectangle r);
extern bool rectangle_segment_collide(Rectangle r, LineSegment s);
extern bool oriented_rectangle_rectangle_collide(OrientedRectangle or, Rectangle aar);
extern bool line_point_collide(Line l, Vector2D p);
extern bool point_segment_collide(Vector2D p, LineSegment s);
extern bool oriented_rectangle_point_collide(OrientedRectangle r, Vector2D p);
extern bool line_segment_collide(Line l, LineSegment s);
extern bool line_oriented_rectangle_collide(Line l, OrientedRectangle r);
extern bool oriented_rectangle_segment_collide(OrientedRectangle r, LineSegment s);

#endif
