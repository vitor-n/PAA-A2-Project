#ifndef GENSUBWAY_H
#define GENSUBWAY_H

#include "graph.h"

void genSubwayStations(CityGraph&, int, int[], bool);
Graph* genSubwayLines(CityGraph&, int[], int**, bool);
Graph buildSubwayGraph(CityGraph&, bool);

#endif
