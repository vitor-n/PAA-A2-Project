#include <iostream>
#include <climits>

#include "graph.h"
#include "genSubway.h"
#include "heap.h"
#include "cityParser.h"

void genBusPoints(CityGraph& cityGraph, int region, int points[]) {
    int maxDist[cityGraph.numNodes()];
    int distances[cityGraph.numNodes()];
    for (int i = 0; i < cityGraph.numNodes(); i++) {
        maxDist[i] = -1;
    }

    for (auto it1 = cityGraph.regions[region]->begin(); it1.hasNext(); it1.next()) {
        cityGraph.CPTDijkstraRegion(it1.value(), distances, region, &compareBuildings);
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

    cout << "Bus Station " << region << ": " << bestNode << endl;
    points[region] = bestNode;
}
/*
Graph* genSubwayLines(CityGraph& city, Graph& subwayFull, int stations[], int **path, bool bPrint){
    int cost[city.numNodes()];

    for (int i = 0; i < city.numRegions(); i++) {
        city.CPTDijkstra(stations[i], path[i], cost, &compareCost);
        for (int j = 0; j < city.numRegions(); j++) {
            if (i == j) continue; 

            EdgeNode* edge = new EdgeNode;
            edge->lenght = cost[stations[j]];

            if (bPrint) cout << i << " " << j << " " << edge->lenght << endl;

            subwayFull.addSegment(i, j, edge);
        }
    }

    Graph* subwayMST = subwayFull.genMSTPrim();
    return subwayMST;
}


*/
