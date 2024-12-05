#include <iostream>
#include "graph.h"
#include "genSubway.h"
#include "cityParser.h"

int main(){
    CityGraph city = cityParser("data/city-1");
    buildSubwayGraph(city, true);
}
