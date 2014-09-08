#include <stdio.h>
#include "vector2d.h"

Vector2D add_vector (Vector2D v1, Vector2D v2)
{
    Vector2D v = {v1.x + v2.x, v1.y + v2.y};
    return v;
}
