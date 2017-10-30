#include "coord.hpp"
#include <math.h>

const double eps = 0.0001; // Standard-Epsilon für Double-Comparison

bool Coord::equals (const Coord& other) const {
    if (fabs(x_ - other.x_) > eps)
        return false;
    if (fabs(y_ - other.y_) > eps)
        return false;

    return true;
}
