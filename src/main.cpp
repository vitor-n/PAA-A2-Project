#include <iostream>
#include <limits>
#include "graph.h"
#include "cityParser.h"
#include "genSubway.h"
#include "heap.h"

#define printList(v, n) {cout << "[ "; for (int i = 0; i < n; i++) { cout << v[i] << " "; }; cout << " ]" << endl;}


int main () {
    CityGraph city = cityParser("data/city-1");
    Graph subwayFull = Graph(city.numRegions(), 0);

    int stations[city.numRegions()];

    for (int i = 0; i < city.numRegions(); i++) {
        genSubwayStations(city, i, stations);
    }

    Graph subwayMST = genSubwayLines(city, subwayFull, stations);

    subwayFull.print();
    subwayMST.print();

    return 0;
}
