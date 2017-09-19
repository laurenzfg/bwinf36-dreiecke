#include "line.hpp"
#include <math.h>

using std::pair;
using std::make_pair;

const double eps = 0.0001; // Standard-Epsilon für Double-Comparison

Line::Line(Coord a, Coord b)
    : a_ {a}, b_ {b} {
    // Aufstellen der Funktionsgleichung
    if (fabs (b_.x_ - a_.x_) < eps) {
        // ==> Senkrechte zur x-Achse
        isLinearFunction_ = false;
        m_ = a.x_;
    } else {
        // ==> lineare Funktion
        isLinearFunction_ = true;
        // Formeln s. Doku
        m_ = (b_.y_ - a_.y_) / (b_.x_ - a_.x_);
        n_ = a_.y_ - (a_.x_ * m_);
    }    
}

// Berechnet wenn vorhanden den Schnittpunkt zwischen this und otherLine
// Wenn ein Schnittpunkt vorhanden ist, hat der zurückgegebene Bool true
// und das Coord enthält den Schnittpunkt
pair<bool, Coord> Line::calculateIntersection (const Line& otherLine) const {
    // Je nach Funktionstypenkombination ist eine andere Schnittpunktermittelungs-
    // funktion anzuwenden
    bool thisLinear = isLinearFunction_;
    bool otherLinear = otherLine.isLinearFunction_;
    if (!thisLinear && !otherLinear) {
        // Fall 1: Zwei Senkrechten
        // lt. Aufgabenstellung dürfen zwei Senkrechten nicht aufeinander liegen,
        // sonst gäbe es aber unendlich Schnittpunkte
        // Ich machs mir einfach und sag' es gibt gar keinen Schnittpunkt
        Coord emptyCoord = {0, 0};
        return (make_pair(false, emptyCoord));
    } else if (!thisLinear && otherLinear) {
        // Fall 2: Diese Strecke ist eine Senkrechte, die andere eine lineare Funktion
        // ==> es gibt definitv einen Schnittpunkt auf der x-Positon der Senkrechten
        // Der y-Wert ergibt sich aus der Funktion der linearen Funktion
        double y = otherLine.m_ * m_ + otherLine.n_;
        Coord intersection = {m_, y};
        return (make_pair(true, intersection));
    } else if (thisLinear && !otherLinear) {
        // Fall 3: Diese Strecke ist eine lineare Funktion, die andere eine Senkrechte
        // ==> es gibt definitv einen Schnittpunkt auf der x-Positon der Senkrechten
        // Der y-Wert ergibt sich aus der Funktion der linearen Funktion
        double y = m_ * otherLine.m_ + n_;
        Coord intersection = {otherLine.m_, y};
        return (make_pair(true, intersection));
    } else {
        // Fall 4: Beide Strecken sind lineare Funktionen
        // ==> Sodenn beide lineare Funktionen nicht parallel sind,
        // gibt es einen Schnittpunkt.
        if (fabs(m_ - otherLine.m_) < eps) {
            // Parallel
            Coord emptyCoord = {0, 0};
            return (make_pair(false, emptyCoord));
        } else {
            // Nicht parallel, Schnittpunkt s. Doku
            double x = (n_ - otherLine.n_) / (otherLine.m_ -m_);
            double y = m_ * x + n_;
            Coord intersection = {x, y};
            return (make_pair(true, intersection));
        }
    }
}

// Liegt der übergebene Punkt auf der **Strecke** (NICHT Gerade)
bool Line::isOnLine (const Coord& coord) const {
    // Anfang und Ende der Strecke auf der y-Achse
    double upperL = fmax (a_.y_, b_.y_);
    double lowerL = fmin (a_.y_, b_.y_);

    // Fallunterscheidung nach Funktionstyp nötig
    if (isLinearFunction_) {
        // Lineare Funktion
        // Bed. 1: Liegt der y-Wert des Punktes nahe bei dem erwarteten Wert unter
        // Beachtung der Geradengleichung
        // Bed. 2 & 3: Liegt der der Punkt zwischen den beiden Limits
        double expectedY = m_ * coord.x_ + n_;
        return (fabs(coord.y_ - expectedY) < eps) &&
               (coord.y_ >= lowerL) &&
               (coord.y_ <= upperL);
    } else {
        // Senkrechte
        // Bed. 1: Liegt der Punkt ziemlich genau bei der Nullstelle der Senkrechten
        //         (horizontal)
        // Bed. 2 & 3: Liegt der Punkt zwischen den beiden Limits an der y-Achse
        return (fabs(coord.x_ - m_) < eps) &&
               (coord.y_ >= lowerL) &&
               (coord.y_ <= upperL);
    }

}

// Ausgabe des Funktionstermes zu Debugzwecken
std::ostream& Line::debug_out(std::ostream& os) const {
    if (isLinearFunction_) {
        return os << "y = " << m_ << "x + " << n_ << "\n";
    } else {
        return os << "x = " << m_ << "\n";
    }
}
