#include <iostream>
#include <fstream>
#include <utility>
#include <vector>

#include "coord.hpp"

using std::cin;
using std::cout;
using std::cerr;
using std::pair;
using std::vector;

vector<pair<Coord, Coord>> parseLines(std::ifstream& inputFile);

int main(int argc, char* argv[]) {
    // Wurde ein Dateiname übergeben?
    if (argc >= 2) {
        std::ifstream inputFile (argv[1]); // Erstellen des Streams
        auto lines = parseLines(inputFile);

        cout << lines[4].first.x;
    } else {
        cerr << "Das Programm muss mit dem Dateinamen der Geradenliste als einzigen Parameter aufgerufen werden.\n";
    }
}

vector<pair<Coord, Coord>> parseLines(std::ifstream& inputFile) {
    vector<pair<Coord, Coord>> coordList;
    
    if (inputFile.is_open()) {
        int n;
        inputFile >> n;

        for (int i = 0; i < n; ++i) {
            pair<Coord, Coord> coord;

            double x1, y1, x2, y2;
            inputFile >> x1 >> y1 >> x2 >> y2;
            coord.first = {x1, y1};
            coord.second = {x2, y2};
            
            coordList.push_back(coord);
        }
    }

    return coordList;
}
