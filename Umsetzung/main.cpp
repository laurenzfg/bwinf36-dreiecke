#include <iostream>
#include <iomanip>
#include <fstream>
#include <utility>
#include <vector>
#include <array>
#include <string>
#include <math.h>

#include "coord.hpp"
#include "line.hpp"

using std::cin;
using std::cout;
using std::cerr;
using std::pair;
using std::vector;
using std::string;

// Vorwärtsdeklarationen der implementierten Funktionen
// Einlesen / Berechnen der Strecken
vector<Line> parseLines(std::ifstream& inputFile);
// Suche nach Dreiecken
vector<vector<Coord>> searchTriangles(const vector<Line>& lines); 
// Ausgabe der Dreiecke als Vektorgrafik
std::ostream& vector_out(const vector<vector<Coord>>& triangles, const vector<Line>& lines, 
                         std::ostream& os);

// cout: Linien, die die Dreiecke bilden
// cerr: Interaktion mit dem Nutzer
// --> Piping der Ausgabe in Datei möglich!
int main(int argc, char* argv[]) {
    // Parameter für cout
    cout << std::setprecision(2) << std::fixed;

    // Wurde ein Dateiname übergeben?
    if (argc >= 2) {
        std::ifstream inputFile (argv[1]);       // Erstellen des Streams
        auto lines = parseLines(inputFile);      // Parsen der Linien
        auto triangles = searchTriangles(lines); // Suche nach Dreiecken

        cerr << "Habe " << triangles.size() << " Dreieck(e) gefunden. Ist eine Ausgabe \
in Vektorgrafiken gewünscht? (y/n) \n";
        char answer;   // Einlesen der Antwort
        cin >> answer; // auf o.g. Frage

        // Ggfs. Ausgabe in Vektorgrafiken
        if (answer == 'y') {
            cerr << "Dateiname für die HTML-Datei, bitte:\n";
            string filename; // Einlesen der Antwort
            cin >> filename; // auf o.g Frage
            
            std::ofstream file {filename};
            vector_out(triangles, lines, file);
        }
        
        // Ausgabe der gefundenen Dreiecke auf cout
        cout << triangles.size() << "\n";
        for (auto triangle : triangles) { // ALLE Dreicke
            for (Coord coord : triangle) { // ALLE Linien
                cout << coord.x_ << " " << coord.y_ << " ";
            }
            cout << "\n";
        }
    } else {
        cerr << "Das Programm muss mit dem Dateinamen der Geradenliste als einzigen Parameter aufgerufen werden.\n";
    }
}

// Laden der Strecken aus der Eingabedatei
vector<Line> parseLines(std::ifstream& inputFile) {
    vector<Line> coordList;
    
    if (inputFile.is_open()) {
        int n; // Streckenzahl in der Eingabe
        inputFile >> n;

        for (int i = 0; i < n; ++i) {
            double x1, y1, x2, y2;
            inputFile >> x1 >> y1 >> x2 >> y2;
            // Speichern der Koordinaten in Aggregaten
            Coord a = {x1, y1};
            Coord b = {x2, y2};
            
            // C++11 ist NINJA-C++
            // Konstruktur invoken ohne auch nur die Klasse explizit zu nennen
            // WIE GEIL IST DAS DEN?
            // BTW: Die Geradengleichung wird bei dieser Gelegenheit mit aufgestellt
            coordList.emplace_back(a,b);
        }
    }

    return coordList;
}

// Testet rekursiv alle möglichen Geradenkombinationen auf das Bilden eine Dreiecks
// const vector<Line>& lines: Geradengleichungen aller Geraden
// pos: Aktuelle Tiefe der Rekursion
// vector<int> soFar: Im aktuellen Stack ausgewählte Geraden
vector<vector<Coord>> searchTriangles(const vector<Line>& lines, int pos, vector<int> soFar) {
    // Rekursionsanker A: Es wurden drei Geraden ausgewählt.
    //                    Test ob sie ein Dreieck bilden.
    if (soFar.size() == 3) {
        vector<Coord> intersections; // Vektor über die Schnittpunkte

        // Checke folgende Schnittpunkte 0-1, 0-2, 1-2
        for (int i = 0; i < 2; ++i) {
            Line a = lines[soFar[i]]; // Gerade A
            for (int j = i + 1; j < 3; j++) {
                Line b = lines[soFar[j]]; // Gerade B

                // Abrufen der Daten eines evtl. vorhandenen Schnittpunkts
                auto intersectionData = a.calculateIntersection(b);
                if (intersectionData.first) { // Kreuzung existiert
                    // Liegt der Schnittpunkt nicht nur auf den Geraden,
                    // sondern auch in beiden Fällen auf den **Strecken**?
                    Coord intersection = intersectionData.second;
                    if (a.isOnLine(intersection) &&
                          b.isOnLine(intersection)) {
                        // Haben wir einen schon bekannten Schnittpunkt nochmal gefunden?
                        for (Coord other : intersections) {
                            if (intersection.equals(other))
                                // Alle Schnittpunkte müssen unterschiedlich sein!
                                return {};
                        }
                        // Wir haben einen gültigen Schnittpunkt gefunden
                        intersections.push_back(intersection); // Speichern
                    } else {
                        // Bei mindestens einer Strecke liegt der Punkt außerhalb
                        return {};
                    }
                } else {
                    // Es existiert kein Schnittpunkt
                    return {};
                }
            }
        }
        
        // Alle Strecken schneiden sich auf einem Punkt auf der Strecke
        // --> ein neues Dreieck wurde gefunden
        vector<vector<Coord>> newVector;
        newVector.push_back(intersections);

        return newVector;
    
    // Rekursionsanker B: Die Rekursionstiefe überschreitet die Anzahl der Geraden.
    //                    Dann ist keine valide Geradenauswahl getroffen worden.
    } else if (lines.size() == pos) {
        // Nicht genug Linien gepickt --> NOPE
        return {};
    } else  {
        // Es gilt, noch eine Strecke zu picken
        // Es gibt zwei Möglichkeiten:
        // Die Strecke pos ist entweder unter drei zu testende
        // Strecken oder halt nicht
        auto soFarPlusOne = soFar; // Bisher gewählte Strecken + pos
        soFarPlusOne.push_back(pos);
        
        // Rekursion weiter durchführen sowohl mit als auch ohne Pos
        auto a = searchTriangles(lines, pos + 1, soFar);
        auto b = searchTriangles(lines, pos + 1, soFarPlusOne);
        
        // Vereinen des Ergebnisses von beiden Blättern des Rekursionsbaumes
        a.insert(a.end(), b.begin(), b.end());        

        return a; // Eine Ebene höher / Endgültige Rückgabe
    }
}

// Überladung von oberer Funktion zum Aufruf aus main()
vector<vector<Coord>> searchTriangles(const vector<Line>& lines) {
    vector<int> empty; // Bisher noch keine Strecke gepickt
    return searchTriangles(lines, 0, empty);
}

// Gibt alle Dreiecke in eine HTML-Datei aus
std::ostream& vector_out(const vector<vector<Coord>>& triangles, const vector<Line>& lines, 
                         std::ostream& os) {
    double max = 0.0; // Größte gefunde Koordinate für Canvasgröße
    
    for (Line line : lines) { // wird hiermit gesucht
        auto coords = line.coords();
        max = fmax(max, coords.first.x_);
        max = fmax(max, coords.second.x_);
        max = fmax(max, coords.first.y_);
        max = fmax(max, coords.second.y_);
    }
    max += 20.0; // Margin von 20px
    double total = max + 20.0; // Gesamtgröße mit 2x Margin

    // Titelei
    os << "<!DOCTYPE html>\n<html>\n<body>\n";

    unsigned cnt = 0; // Dreieckscounter für die Überschriften
    for (auto triangle : triangles) { // ALLE Dreicke
        os << "<h2>Dreieck #" << ++cnt << "</h2>\n"; // Überschrift
        os << "<svg width=\"" << total << "\" height=\"" << total << "\" "; // Canvasgröße
        os << "viewBox=\"-20 -20 " << max << " " << max << "\">\n"; // Koordinatensystem
        os << "<rect width=\"100%\" height=\"100%\" fill=\"white\" />\n"; // Weißer BG

        // Ausgabe der Zeichnung
        for (Line line : lines) {
            auto coords = line.coords();
            os << "<line x1=\"" << coords.first.x_ << "\" y1=\"" << coords.first.y_ << "\"";
            os << "x2=\"" << coords.second.x_ << "\" y2=\"" << coords.second.y_ << "\"";
            os << "stroke=\"black\" stroke-width=\"1\" />\n";
        }

        // Ausgabe des jeweiligen Dreiecks
        os << "<polygon points=\"";
        for (Coord coord : triangle) { // ALLE Linien
            os << coord.x_ << "," << coord.y_ << " ";
        }
        os << "\" stroke=\"orange\" stroke-width=\"3\" fill=\"transparent\" />\n";
        os << "</svg>\n";
    }

    os << "</body>\n</html>";
    return os;
}
