#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <array>

#include "coord.hpp"
#include "line.hpp"

using std::cin;
using std::cout;
using std::cerr;
using std::pair;
using std::vector;

vector<Line> parseLines(std::ifstream& inputFile);
vector<vector<int>> searchTriangles(const vector<Line>& lines); 

int main(int argc, char* argv[]) {
    // Wurde ein Dateiname übergeben?
    if (argc >= 2) {
        std::ifstream inputFile (argv[1]);       // Erstellen des Streams
        auto lines = parseLines(inputFile);      // Parsen der Linien
        auto triangles = searchTriangles(lines); // Suche nach Dreiecken

        cout << "Habe " << triangles.size() << " Dreieck(e) gefunden. Ist eine Ausgabe \
                                                in Vektorgrafiken gewünscht? (Y/N) \n";
        char answer;
        cin >> answer;

        if (answer == 'Y') {
            cout << "Ätschibetsch, nicht implementiert \n";
        }
    } else {
        cerr << "Das Programm muss mit dem Dateinamen der Geradenliste als einzigen Parameter aufgerufen werden.\n";
    }
}

// Testet rekursiv alle möglichen Geradenkombinationen auf das Bilden eine Dreiecks
// const vector<Line>& lines: Geradengleichungen aller Geraden
// pos: Aktuelle Tiefe der Rekursion
// vector<int> soFar: Im aktuellen Stack ausgewählte Geraden
vector<vector<int>> searchTriangles(const vector<Line>& lines, int pos, vector<int> soFar) {
    // Rekursionsanker A: Es wurden drei Geraden ausgewählt.
    //                    Test ob sie ein Dreieck bilden.
    if (soFar.size() == 3) {
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
                    if (!(a.isOnLine(intersection) &&
                          b.isOnLine(intersection))) {
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
        vector<vector<int>> newVector;
        newVector.push_back(soFar);

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

// Überladung zum Aufruf aus main()
vector<vector<int>> searchTriangles(const vector<Line>& lines) {
    vector<int> empty; // Bisher noch keine Strecke gepickt
    return searchTriangles(lines, 0, empty);
}

// Laden der Strecken aus der Eingabedatei
vector<Line> parseLines(std::ifstream& inputFile) {
    vector<Line> coordList;
    
    if (inputFile.is_open()) {
        int n; // Streckenzahl
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
