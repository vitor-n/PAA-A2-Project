#ifndef GENBUS_H
#define GENBUS_H

#include "graph.h"

void genBusPoints(CityGraph&, int, int[]);
void genBusLinesFull(CityGraph&, int[], float**, int**);
float genBusLines(int, int[], float**);
void swapPath(int[], int, int);
float optimizeBusLines(int, int[], float**, float);

#endif
