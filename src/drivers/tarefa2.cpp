#include <iostream>
#include "graph.h"
#include "genBus.h"
#include "cityParser.h"

int main(){
    CityGraph city = cityParser("data/city-1");
    buildBusGraph(city, true);
}
