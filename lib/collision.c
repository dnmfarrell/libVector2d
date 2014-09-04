#include <stdio.h>
#include "libvector2d.h"

Vector2d add_vector (Vector2d v1, Vector2d v2)
{
    Vector2d v = {v1.x + v2.x, v1.y + v2.y};
    return v;
}
