#ifndef GENBUS_H
#define GENBUS_H

#include "graph.h"

void genBusPoints(CityGraph&, int, int[], bool);
void genBusLinesFull(CityGraph&, int[], float**, int**);
float genBusLines(int, int[], float**, bool);
void swapPath(int[], int, int);
float optimizeBusLines(int, int[], float**, float, bool);
Graph buildBusGraph(CityGraph&, bool);

#endif
