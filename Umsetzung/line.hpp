#ifndef LINE_HPP
#define LINE_HPP

#include <iostream>
#include <utility>
#include "coord.hpp"

// Punkt A bildet mit Punkt B folgende lineare Funktion
// y = mx + n
// ODER: Punkt A liegt mit Punkt B auf einer Senkrechten zur x-Achse
// x = (Zahl)   (Zur y-Achse parallele vertikale Linie)
class Line {
    Coord a_, b_; // Die Punkte die auf der Linie liegen
    
    bool isLinearFunction_;
    // Bei einer linearen Funktion speichert m_ und n_ den jeweiligen 
    // Funktionsbestandteil, bei einer vertikalen Linie speichert n_
    // die Nullstelle der Senkrechten
    double m_, n_;
     
    public:
        // Ausgabe der Geraden zu Debugzwecken
        std::ostream& debug_out(std::ostream& os) const;
        // Gibt es einen Schnittpunkt zwischen beiden Graphen? Und wenn ja: wo?
        std::pair<bool, Coord> calculateIntersection (const Line& otherLine) const;
        // Liegt der übergebene Punkt auf der **Strecke** (NICHT Gerade)
        bool isOnLine (const Coord& coord) const;
        // Konstruktor
        Line(Coord a, Coord b);
};
#endif //LINE_HPP
