#ifndef COORD_HPP
#define COORD_HPP

struct Coord {
        double x_;
        double y_;

        // Überprüft, ob zwei Koordinaten (nahezu) identisch isnd
        bool equals (const Coord& other) const;
};
#endif //COORD_HPP
