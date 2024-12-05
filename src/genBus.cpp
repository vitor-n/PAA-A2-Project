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

void genBusLinesFull(CityGraph& city, int points[], float** distMatrix, int** path){
    int cost[city.numNodes()];

    for (int i = 0; i < city.numRegions(); i++) {
        city.CPTDijkstra(points[i], path[i], cost, &compareBuildings);
        for (int j = 0; j < city.numRegions(); j++) {
            if (i == j) continue;
            distMatrix[i][j] = cost[points[j]];
        }
    }
}

float genBusLines(int numRegions, int busLine[], float** distMatrix){
    int INF = 2147483647;
    bool visited[numRegions];
    for(int i = 0; i < numRegions; i++) {visited[i] = false;}

    int v = 0;
    cout << v << " ";
    busLine[v] = 0; visited[v] = true;
    float totalDist = 0;
    int edgeCount = 0;

    while(edgeCount != numRegions - 1){
        int bestV = -1;
        float minDist = INF;

        for(int other = 0; other < numRegions; other++){
            if(!visited[other] && distMatrix[v][other] < minDist){
                minDist = distMatrix[v][other];
                bestV = other;
            }
        }
        busLine[edgeCount + 1] = bestV; visited[bestV] = true;
        totalDist += minDist;
        edgeCount++;
        v = bestV;
        cout << "(" << minDist << ") " << v << " ";
    } cout << "(" << distMatrix[v][0] << ")";
    cout << endl << "Total cost: " << totalDist << endl;
    return totalDist;
}

void swapPath(int busLine[], int i, int j){
    i = i + 1;
    while(i < j){
        int temp = busLine[i];
        busLine[i] = busLine[j];
        busLine[j] = temp;
        i++; j--;
    }
}

float optimizeBusLines(int numRegions, int busLine[], float** distMatrix, float totalDist){
    bool improved = true;
    while(improved){
        improved = false;
        for(int i = 0; i < numRegions - 1; i++){
            for(int j = i + 1; j < numRegions; j++){
                int deltaCost = -distMatrix[busLine[i]][busLine[i + 1]] - distMatrix[busLine[j]][busLine[(j + 1) % numRegions]]
                               + distMatrix[busLine[i]][busLine[j]] + distMatrix[busLine[i + 1]][busLine[(j + 1) % numRegions]];
                if (deltaCost < 0){
                    totalDist += deltaCost;
                    cout << "swapped " << i << " for " << j << ": " << "[ " << busLine[0];
                    swapPath(busLine, i, j);
                    for(int l = 1; l < numRegions; l++){cout << "(" << distMatrix[busLine[l - 1]][busLine[l]] << ") " << busLine[l] << " ";}
                    cout << "] Total cost: " << totalDist << endl;
                    improved = true;
                }
            }
        }
    }
    return totalDist;
}

int main(){
    CityGraph city = cityParser("data/city-1");
    Graph busFull = Graph(city.numRegions(), 0);

    //ESCOLHA DOS PONTOS QUE DEVE PASSAR NA REGIÃO
    int points[city.numRegions()];
    int** path = new int*[city.numRegions()];
    for (int i = 0; i < city.numRegions(); i++){
        path[i] = new int[city.numNodes()];
    }

    for (int i = 0; i < city.numRegions(); i++) {
        genBusPoints(city, i, points);
    }

    //GERAÇÃO BUS FULL
    float** distMatrix = new float*[city.numRegions()];
    for(int i = 0; i < city.numRegions(); i++){
        distMatrix[i] = new float[city.numRegions()];
        for(int j = 0; j < city.numRegions(); j++){
            distMatrix[i][j] = -1;
        }
    }

    genBusLinesFull(city, points, distMatrix, path);

    for(int i = 0; i < city.numRegions(); i++){
        for(int j = 0; j < city.numRegions(); j++){
            cout << distMatrix[i][j] << " ";
        } cout << endl;
    }

    //DETERMINAÇÃO DO PRIMEIRO CICLO HAMILTONIANO QUE TEM
    int busLine[city.numRegions()];
    for(int i = 0; i < city.numRegions(); i++) {busLine[i] = -1;}
    float totalDist = genBusLines(city.numRegions(), busLine, distMatrix);

    //OTIMIZAÇÃO DO CICLO O(r^4)
    totalDist = optimizeBusLines(city.numRegions(), busLine, distMatrix, totalDist);

    //busFull.print();

}
