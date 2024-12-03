#include <iostream>
#include "graph.h"
#include "cityParser.h"
#include "genSubway.h"

int main () {
    CityGraph city = cityParser("data/city-1");

    for (int i = 0; i < city.numRegions(); i++) {
        genSubwayStations(city, i);
    }
}
