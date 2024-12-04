#ifndef FINDROUTE_H
#define FINDROUTE_H

#include "hashTable.h"

void findEdge(CityGraph& city, int region, int street, int number, int& v1, int& v2, float& dist_v1, float& dist_v2);
EdgeNode* copyStreetInfo(CityGraph& city, int v1, int v2);
void findRoute(CityGraph& city, int adress1[], int adress2[], int route[], float distance[], float (*func)(EdgeNode*));
float compareSpeed(EdgeNode* node);

#endif