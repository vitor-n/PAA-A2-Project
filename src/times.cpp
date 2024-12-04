#include <iostream>
#include <chrono>
#include <fstream>
#include <limits>
#include "graph.h"
#include "cityParser.h"
#include "genSubway.h"
#include "heap.h"

using namespace std;

using chrono::high_resolution_clock;
using chrono::duration_cast;
using chrono::nanoseconds;


#define printList(v, n) {cout << "[ "; for (int i = 0; i < n; i++) { cout << v[i] << " "; }; cout << " ]" << endl;}


void get_times(string numCity, string strFILENAME) {
    CityGraph city = cityParser("data/city-" + numCity);
    Graph subwayFull = Graph(city.numRegions(), 0);

    auto timeStart1 = high_resolution_clock::now();

    int stations[city.numRegions()];

    for (int i = 0; i < city.numRegions(); i++) {
        genSubwayStations(city, i, stations);
    }

    int path;

    Graph subwayMST = genSubwayLines(city, subwayFull, stations, path);

    auto timeStop1 = high_resolution_clock::now();

    ofstream outputFile(strFILENAME, ios::out | ios::trunc);

    outputFile << "Tempo operação 1" << endl;

    auto timeDuration1 = duration_cast<nanoseconds>(timeStop1 - timeStart1);

    outputFile << timeDuration1.count() << endl;

    outputFile.close();
}
