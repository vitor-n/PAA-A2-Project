#include <iostream>
#include <limits>
#include "graph.h"
#include "cityParser.h"
#include "genSubway.h"
#include "heap.h"

#define printList(v, n) {cout << "[ "; for (int i = 0; i < n; i++) { cout << v[i] << " "; }; cout << " ]" << endl;}


int main () {
    CityGraph city = cityParser("data/city-1");
    Graph subwayFull = Graph(city.numRegions(), 0);

    int stations[city.numRegions()];

    for (int i = 0; i < city.numRegions(); i++) {
        genSubwayStations(city, i, stations);
    }

    int cost[city.numNodes()];
    int path[city.numNodes()];

    for (int i = 0; i < city.numRegions(); i++) {
        for (int j = 0; j < city.numRegions(); j++) {
            if (i == j) { continue; }

            city.CPTDijkstra(stations[i], path, cost);
            EdgeNode* edge = new EdgeNode;
            edge->lenght = cost[stations[j]];
            cout << i << " " << j << " " << edge->lenght << endl;

            subwayFull.addSegment(i, j, edge);
        }
    }

    Graph subwayMST = subwayFull.genMST();

    subwayFull.print();
    subwayMST.print();

    return 0;
}
