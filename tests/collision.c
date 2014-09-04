#include <stdio.h>
#include "vector2d.h"
#include <assert.h>
#include <math.h>

int main (void)
{
	//add
	Vector2D aa = {3, 5};
	Vector2D bb = {8, 2};
	Vector2D cc = add_vector(aa, bb);
	assert(cc.x == 11 && cc.y == 7);
	
	// subtract
	Vector2D a = {7, 4};
	Vector2D b = {3, -3};
	Vector2D c = subtract_vector(a, b);
	assert(c.x == 4 && c.y == 7);
	
	// assert_equal, negate
	Vector2D d = {7, 4};
	Vector2D e = {3, -3};
	Vector2D f = add_vector(d, negate_vector(e));
	assert_equal_vectors(f, subtract_vector(d, e));
	
	// multiply
	Vector2D g = {6, 3};
	Vector2D h = multiply_vector(g, 2);
	assert(equal_floats(h.x, 12) && equal_floats(h.y, 6));
	
	// divide
	Vector2D i = {8, 4};
	Vector2D j = divide_vector(i, 2);
	assert(equal_floats(j.x, 4) && equal_floats(j.y, 2));
	
	// length
	Vector2D k = {10, 5};
	assert(equal_floats(vector_length(k), 11.1803398874));
	
	// unit vector
	Vector2D l = {10, 5};
	assert(1 < vector_length(l));
	Vector2D m = unit_vector(l);
	assert(equal_floats(1, vector_length(m)));
	
	// rotate
	Vector2D n = {12, 3};
	Vector2D o = rotate_vector(n, 6.283185307179586476925286766559);
	assert(equal_floats(o.x,12) && equal_floats(o.y,3));
	
	// dot product
	Vector2D p = {8, 2};
	Vector2D q = {-2, 8};
	Vector2D r = {-5, 5};
	assert(equal_floats(0, dot_product(p, q)));
	assert(0 > dot_product(p, r) && 0 < dot_product(q, r));

	// project
	Vector2D s = {12, 5};
	Vector2D t = {5, 6};
	//Vector2D u = project_vector(t, s);
	
	// enclosed angle
	Vector2D v = {8, 2};
	Vector2D w = {-2, 8};
	assert(equal_floats(90, radians_to_degrees(enclosed_angle(v, w))));
	assert(equal_floats(0, dot_product(v, w)));
	
	// rectangle collision
	Rectangle x = {{1, 1}, {4, 4}};
	Rectangle y = {{2, 2}, {5, 5}};
	Rectangle z = {{6, 4}, {4, 2}};
	assert(rectangles_collide(x, y));
	assert(rectangles_collide(y, z));
	assert(!rectangles_collide(x, z));
	
	// circle collision
	Circle ab = {{4, 4}, 2};
	Circle ac = {{7, 4}, 2};
	Circle ad = {{10, 4}, 2};
	assert(circles_collide(ab, ac));
	assert(circles_collide(ac, ad));
	assert(!circles_collide(ab, ad));
	
	// points collision
	Vector2D ae = {2, 3};
	Vector2D af = {2, 3};
	Vector2D ag = {3, 4};
	assert(points_collide(ae, af));
	assert(!points_collide(ae, ag));
	assert(!points_collide(af, ag));
	
	// line collision
	Vector2D ah = {3, 5};
	Vector2D ai = {3, 2};
	Vector2D aj = {8, 4};
	Vector2D down = {5, -1};
	Vector2D up = {5, 2};
	
	Line l1 = {ah, down};
	Line l2 = {ah, up};
	Line l3 = {ai, up};
	Line l4 = {aj, down};
	
	assert(lines_collide(l1, l2));
	assert(lines_collide(l1, l3));
	assert(!lines_collide(l2, l3));
	assert(lines_collide(l1, l4));
	
	printf("All tests pass\n");
	return 0;
}