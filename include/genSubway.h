#ifndef GENSUBWAY_H
#define GENSUBWAY_H

#include "graph.h"

void genSubwayStations(CityGraph&, int, int[]);
Graph genSubwayLines(CityGraph&, Graph&, int[], int**, bool);

#endif
