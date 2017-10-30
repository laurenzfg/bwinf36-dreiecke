#include "coord.hpp"
#include <math.h>

const double eps = 0.0001; // Standard-Epsilon für Double-Comparison

// Entspricht this (fast) other?
bool Coord::equals (const Coord& other) const {
    if (fabs(x_ - other.x_) > eps) // x unterschiedlich
        return false; // --> zwei verschiedene Coords
    if (fabs(y_ - other.y_) > eps) // y unterschiedlich
        return false; // --> zwei verschiedene Coords

    return true; // --> Coords sind gleich
}
