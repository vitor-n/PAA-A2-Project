#include <iostream>
#include <climits>

#include "graph.h"
#include "genSubway.h"

void genSubwayStations(CityGraph& cityGraph, int region) {
    int maxDist[cityGraph.numNodes()];
    int distances[cityGraph.numNodes()];
    for (int i = 0; i < cityGraph.numNodes(); i++) {
        maxDist[i] = -1;
    }

    for (auto it1 = cityGraph.regions[region]->begin(); it1.hasNext(); it1.next()) {
        cityGraph.CPTDijkstraRegion(it1.value(), distances, region);
        for (auto it2 = cityGraph.regions[region]->begin(); it2.hasNext(); it2.next()) {
            if (distances[it2.value()] > maxDist[it2.value()]) {
                maxDist[it2.value()] = distances[it2.value()];
            }
        }
    }

    int minValue = INT_MAX;
    int bestNode = -1;

    for (auto it = cityGraph.regions[region]->begin(); it.hasNext(); it.next()) {
        if (maxDist[it.value()] < minValue) {
            minValue = maxDist[it.value()];
            bestNode = it.value();
        }
    }

    cout << "subway: " << bestNode << endl;
}