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

bool on_one_side(Line axis, LineSegment s)
{
    Vector2D d1 = subtract_vector(s.point1, axis.base);
    Vector2D d2 = subtract_vector(s.point2, axis.base);
    Vector2D n = rotate_vector_90(axis.direction);
    return dot_product(n, d1) * dot_product(n, d2) > 0;
}

Range sort_range(Range r)
{
    Range sorted = r;
    if(r.minimum > r.maximum)
    {
        sorted.minimum = r.maximum;
        sorted.maximum = r.minimum;
    }
    return sorted;
}

Range project_segment(LineSegment s, Vector2D onto)
{
    Vector2D ontoUnit = unit_vector(onto);
    Range r;
    r.minimum = dot_product(ontoUnit, s.point1);
    r.maximum = dot_product(ontoUnit, s.point2);
    r = sort_range(r);
    return r;
}

bool overlapping_ranges(Range a, Range b)
{
    return overlapping(a.minimum, a.maximum, b.minimum, b.maximum);
}

bool segments_collide(LineSegment a, LineSegment b)
{
    Line axisA, axisB;
    axisA.base = a.point1;
    axisA.direction = subtract_vector(a.point2, a.point1);

    if(on_one_side(axisA, b))
        return false;

    axisB.base = b.point1;
    axisB.direction = subtract_vector(b.point2, b.point1);

    if(on_one_side(axisB, a))
        return false;

    if(parallel_vectors(axisA.direction, axisB.direction))
    {
        Range rangeA = project_segment(a, axisA.direction);
        Range rangeB = project_segment(b, axisA.direction);
        return overlapping_ranges(rangeA, rangeB);
    }
    else
        return true;
}

Range range_hull(Range a, Range b)
{
    Range hull;
    hull.minimum = a.minimum < b.minimum ? a.minimum : b.minimum;
    hull.maximum = a.maximum > b.maximum ? a.maximum : b.maximum;
    return hull;
}

LineSegment oriented_rectangle_edge(OrientedRectangle r, int nr)
{
    LineSegment edge;
    Vector2D a = r.halfExtend;
    Vector2D b = r.halfExtend;

    switch(nr % 4)
    {
        case 0:
            a.x = -a.x;
            break;
        case 1:
            b.y = -b.y;
            break;
        case 2:
            a.y = -a.y;
            b = negate_vector(b);
        break;
        default:
            a = negate_vector(a);
            b.x = -b.x;
            break;
    }

    a = rotate_vector(a, r.rotation);
    a = add_vector(a, r.center);
    b = rotate_vector(b, r.rotation);
    b = add_vector(b, r.center);
    edge.point1 = a;
    edge.point2 = b;
    return edge;
}

bool separating_axis_for_oriented_rectangle(LineSegment axis, OrientedRectangle r)
{
    Range axisRange, r0Range, r2Range, rProjection;
    LineSegment rEdge0 = oriented_rectangle_edge(r, 0);
    LineSegment rEdge2 = oriented_rectangle_edge(r, 2);
    Vector2D n = subtract_vector(axis.point1, axis.point2);

    axisRange = project_segment(axis, n);
    r0Range = project_segment(rEdge0, n);
    r2Range = project_segment(rEdge2, n);
    rProjection = range_hull(r0Range, r2Range);

    return !overlapping_ranges(axisRange, rProjection);
}

bool oriented_rectangles_collide(OrientedRectangle a, OrientedRectangle b)
{
    LineSegment edge = oriented_rectangle_edge(a, 0);

    if(separating_axis_for_oriented_rectangle(edge, b))
        return false;

    edge = oriented_rectangle_edge(a, 1);

    if(separating_axis_for_oriented_rectangle(edge, b))
        return false;

    edge = oriented_rectangle_edge(b, 0);

    if(separating_axis_for_oriented_rectangle(edge, a))
        return false;

    edge = oriented_rectangle_edge(b, 1);

    return !separating_axis_for_oriented_rectangle(edge, a);
}

bool circle_point_collide(Circle c, Vector2D p)
{
    Vector2D distance = subtract_vector(c.center, p);
    return vector_length(distance) <= c.radius;
}

bool circle_line_collide(Circle c, Line l)
{
    Vector2D lc = subtract_vector(c.center, l.base);
    Vector2D p = project_vector(lc, l.direction);
    Vector2D nearest = add_vector(l.base, p);
    return circle_point_collide(c, nearest);
}

bool circle_segment_collide(Circle c, LineSegment s)
{
    if(circle_point_collide(c, s.point1))
        return true;

    if(circle_point_collide(c, s.point2))
        return true;

    Vector2D d = subtract_vector(s.point2, s.point1);
    Vector2D lc = subtract_vector(c.center, s.point1);
    Vector2D p = project_vector(lc, d);
    Vector2D nearest = add_vector(s.point1, p);

    return circle_point_collide(c, nearest)
            && vector_length(p) <= vector_length(d)
            && 0 <= dot_product(p, d);
}

float clamp_on_range(float x, float min, float max)
{
    if(x < min)
        return min;

    else if(max < x)
        return max;

    else
        return x;
}

Vector2D clamp_on_rectangle(Vector2D p, Rectangle r)
{
    Vector2D clamp;
    clamp.x = clamp_on_range(p.x, r.origin.x, r.origin.x + r.size.x);
    clamp.y = clamp_on_range(p.y, r.origin.y, r.origin.y + r.size.y);
    return clamp;
}

bool circle_rectangle_collide(Circle c, Rectangle r)
{
    Vector2D clamped = clamp_on_rectangle(c.center, r);
    return circle_point_collide(c, clamped);
}

bool circle_oriented_rectangle_collide(Circle c, OrientedRectangle r)
{
    Rectangle lr;
    lr.origin.x = 0;
    lr.origin.y = 0;
    lr.size = multiply_vector(r.halfExtend, 2);
    Circle lc = {{0, 0}, c.radius};
    Vector2D distance = subtract_vector(c.center, r.center);
    distance = rotate_vector(distance, -r.rotation);
    lc.center = add_vector(distance, r.halfExtend);
    return circle_rectangle_collide(lc, lr);
}

bool point_rectangle_collide(Vector2D p, Rectangle r)
{
    float left = r.origin.x;
    float right = left + r.size.x;
    float bottom = r.origin.y;
    float top = bottom + r.size.y;
    return left <= p.x && bottom <= p.y && p.x <= right && p.y <= top;
}

bool line_rectangle_collide(Line l, Rectangle r)
{

    Vector2D n = rotate_vector_90(l.direction);
    float dp1, dp2, dp3, dp4;

    Vector2D c1 = r.origin;
    Vector2D c2 = add_vector(c1, r.size);
    Vector2D c3 = {c2.x, c1.y};
    Vector2D c4 = {c1.x, c2.y};

    c1 = subtract_vector(c1, l.base);
    c2 = subtract_vector(c2, l.base);
    c3 = subtract_vector(c3, l.base);
    c4 = subtract_vector(c4, l.base);

    dp1 = dot_product(n, c1);
    dp2 = dot_product(n, c2);
    dp3 = dot_product(n, c3);
    dp4 = dot_product(n, c4);

    return (dp1 * dp2 <= 0) || (dp2 * dp3 <= 0) || (dp3 * dp4 <= 0);
}

bool rectangle_segment_collide(Rectangle r, LineSegment s)
{
    Line sLine;
    sLine.base = s.point1;
    sLine.direction = subtract_vector(s.point2, s.point1);

    if(!line_rectangle_collide(sLine, r))
        return false;

    Range rRange, sRange;
    rRange.minimum = r.origin.x;
    rRange.maximum = r.origin.x + r.size.x;
    sRange.minimum = s.point1.x;
    sRange.maximum = s.point2.x;
    sRange = sort_range(sRange);

    if(!overlapping_ranges(rRange, sRange))
        return false;

    rRange.minimum = r.origin.y;
    rRange.maximum = r.origin.y + r.size.y;
    sRange.minimum = s.point1.y;
    sRange.maximum = s.point2.y;
    sRange = sort_range(sRange);

    return overlapping_ranges(rRange, sRange);
}

Vector2D rectangle_corner(Rectangle r, int nr)
{
    Vector2D corner = r.origin;
    switch(nr % 4)
    {
        case 0:
            corner.x += r.size.x;
            break;
        case 1:
            corner = add_vector(corner, r.size);
            break;
        case 2:
            corner.y += r.size.y;
            break;
        default:
        /* corner = r.origin */
            break;
    }
    return corner;
}

bool separating_axis_for_rectangle(LineSegment axis, Rectangle r)
{
    LineSegment rEdgeA, rEdgeB;
    Range axisRange, rEdgeARange, rEdgeBRange, rProjection;
    Vector2D n = subtract_vector(axis.point1, axis.point2);
    rEdgeA.point1 = rectangle_corner(r, 0);
    rEdgeA.point2 = rectangle_corner(r, 1);
    rEdgeB.point1 = rectangle_corner(r, 2);
    rEdgeB.point2 = rectangle_corner(r, 3);

    rEdgeARange = project_segment(rEdgeA, n);
    rEdgeBRange = project_segment(rEdgeB, n);
    rProjection = range_hull(rEdgeARange, rEdgeBRange);

    axisRange = project_segment(axis, n);
    return !overlapping_ranges(axisRange, rProjection);
}

Vector2D oriented_rectangle_corner(OrientedRectangle r, int nr)
{
    Vector2D c = r.halfExtend;

    switch(nr % 4) {
        case 0:
            c.x = -c.x;
            break;
        case 1:
            /* c = r.halfExtend */
            break;
        case 2:
            c.y = -c.y;
            break;
        default:
            c = negate_vector(c);
            break;
    }
    c = rotate_vector(c, r.rotation);
    return add_vector(c, r.center);
}

float minimum(float a, float b)
{
    return a < b ? a : b;
}

float maximum(float a, float b)
{
    return a > b ? a : b;
}

Rectangle enlarge_rectangle_point(Rectangle r, Vector2D p)
{
    Rectangle enlarged;
    enlarged.origin.x = minimum(r.origin.x, p.x);
    enlarged.origin.y = minimum(r.origin.y, p.y);
    enlarged.size.x = maximum(r.origin.x + r.size.x, p.x);
    enlarged.size.y = maximum(r.origin.y + r.size.y, p.y);
    enlarged.size = subtract_vector(enlarged.size, enlarged.origin);
    return enlarged;
}

Rectangle oriented_rectangle_rectangle_hull(OrientedRectangle r)
{
    Rectangle h = {r.center, {0, 0}};
    int nr;
    Vector2D corner;

    for(nr = 0; nr < 4; ++nr)
    {
        corner = oriented_rectangle_corner(r, nr);
        h = enlarge_rectangle_point(h, corner);
    }
    return h;
}

bool oriented_rectangle_rectangle_collide(OrientedRectangle or, Rectangle aar)
{
    Rectangle orHull = oriented_rectangle_rectangle_hull(or);

    if(!rectangles_collide(orHull, aar))
        return false;

    LineSegment edge = oriented_rectangle_edge(or, 0);

    if(separating_axis_for_rectangle(edge, aar))
        return false;

    edge = oriented_rectangle_edge(or, 1);

    return !separating_axis_for_rectangle(edge, aar);
}

bool line_point_collide(Line l, Vector2D p)
{
    if(points_collide(l.base, p))
        return true;

    Vector2D lp = subtract_vector(p, l.base);

    return parallel_vectors(lp, l.direction);
}

bool point_segment_collide(Vector2D p, LineSegment s)
{
    Vector2D d = subtract_vector(s.point2, s.point1);
    Vector2D lp = subtract_vector(p, s.point1);
    Vector2D pr = project_vector(lp, d);

    return equal_vectors(lp, pr)
        && vector_length(pr) <= vector_length(d)
        && 0 <= dot_product(pr, d);
}

bool oriented_rectangle_point_collide(OrientedRectangle r, Vector2D p)
{
    Rectangle lr;
    lr.origin.x = 0;
    lr.origin.y = 0;
    lr.size = multiply_vector(r.halfExtend, 2);

    Vector2D lp = subtract_vector(p, r.center);
    lp = rotate_vector(lp, -r.rotation);
    lp = add_vector(lp, r.halfExtend);

    return point_rectangle_collide(lp, lr);
}

bool line_segment_collide(Line l, LineSegment s)
{
    return !on_one_side(l, s);
}

bool line_oriented_rectangle_collide(Line l, OrientedRectangle r)
{
    Rectangle lr;
    lr.origin.x = 0;
    lr.origin.y = 0;
    lr.size = multiply_vector(r.halfExtend, 2);

    Line ll;
    ll.base = subtract_vector(l.base, r.center);
    ll.base = rotate_vector(ll.base, -r.rotation);
    ll.base = add_vector(ll.base, r.halfExtend);
    ll.direction = rotate_vector(l.direction, -r.rotation);

    return line_rectangle_collide(ll, lr);
}

bool oriented_rectangle_segment_collide(OrientedRectangle r, LineSegment s)
{
    Rectangle lr;
    lr.origin.x = 0;
    lr.origin.y = 0;
    lr.size = multiply_vector(r.halfExtend, 2);

    LineSegment ls;
    ls.point1 = subtract_vector(s.point1, r.center);
    ls.point1 = rotate_vector(ls.point1, -r.rotation);
    ls.point1 = add_vector(ls.point1, r.halfExtend);
    ls.point2 = subtract_vector(s.point2, r.center);
    ls.point2 = rotate_vector(ls.point2, -r.rotation);
    ls.point2 = add_vector(ls.point2, r.halfExtend);

    return rectangle_segment_collide(lr, ls);
}
