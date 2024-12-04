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
int main(){
    int INF = 2147483647;
    CityGraph city = cityParser("data/city-1");
    Graph busFull = Graph(city.numRegions(), 0);

    //GERAÇÃO BUS FULL
    int points[city.numRegions()];
    int** path = new int*[city.numRegions()];
    for (int i = 0; i < city.numRegions(); i++){
        path[i] = new int[city.numNodes()];
    }


    float distMatrix[city.numRegions()][city.numRegions()];
    for(int i = 0; i < city.numRegions(); i++){
        for(int j = 0; j < city.numRegions(); j++){
            distMatrix[i][j] = -1;
        }
    }

    for (int i = 0; i < city.numRegions(); i++) {
        genBusPoints(city, i, points);
    }

    int cost[city.numNodes()];

    for (int i = 0; i < city.numRegions(); i++) {
        city.CPTDijkstra(points[i], path[i], cost, &compareBuildings);
        for (int j = 0; j < city.numRegions(); j++) {
            if (i == j) continue;

            EdgeNode* edge = new EdgeNode;
            edge->lenght = cost[points[j]];

            cout << i << " " << j << " " << edge->lenght << endl;
            distMatrix[i][j] = edge->lenght;
            busFull.addSegment(i, j, edge);
        }
    }

    //GERAÇÃO BUSLINE
    int busLine[city.numRegions()];
    bool visited[city.numRegions()];
    for(int i = 0; i < city.numRegions(); i++) {busLine[i] = -1; visited[i] = false;}
    int edgeCount = 0;

    int v = 0;
    cout << v << " ";
    busLine[v] = 0;
    visited[v] = true;
    float totalDist = 0;

    while(edgeCount != city.numRegions() - 1){
        int best = -1;
        int minDist = INF;

        for(int other = 0; other < city.numRegions(); other++){
            if(!visited[other] && distMatrix[v][other] < minDist){
                minDist = distMatrix[v][other];
                best = other;
            }
        }
        edgeCount++;
        busLine[edgeCount] = best;
        totalDist += minDist;
        v = best;
        visited[v] = true;
        cout << "(" << minDist << ") " << v << " ";
    }
    cout << endl << "Total cost: " << totalDist << endl;
    for(int i = 0; i < city.numRegions() - 1; i++){
        for(int j = i + 1; j < city.numRegions(); j++){
            j =
            int deltaCost = distMatrix[]
        }
    }

    busFull.print();


}
