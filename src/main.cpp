#include <iostream>

#include "graph.h"
#include "genBus.h"
#include "genSubway.h"
#include "cityParser.h"
#include "findRoute.h"

#define printList(v, n) {cout << "[ "; for (int i = 0; i < n; i++) { cout << v[i] << " "; }; cout << " ]" << endl;}

using namespace std;
/*
int main () {
    CityGraph city = cityParser("data/city-1");
    Graph subway = buildSubwayGraph(city, false);
    Graph bus = buildBusGraph(city, false);

    int adress1[3] = {3, 0, 1};
    int adress2[3] = {6, 120, 2};

    findBestRoute(city, subway, bus, adress1, adress2);

    return 0;
}*/
