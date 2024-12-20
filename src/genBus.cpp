#include <iostream>
#include <fstream>
#include <climits>

#include "graph.h"
#include "genSubway.h"
#include "heap.h"
#include "cityParser.h"

using namespace std;

void genBusPoints(CityGraph& cityGraph, int region, int points[], bool verbose) {
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

    if (verbose) cout << "Ponto obrigatorio da regiao " << region << ": " << bestNode << endl;
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

float genBusLines(int numRegions, int busLine[], float** distMatrix, bool verbose){
    int INF = 2147483647;
    bool visited[numRegions];
    for(int i = 0; i < numRegions; i++) {visited[i] = false;}

    int v = 0;
    busLine[v] = 0; visited[v] = true;
    float totalDist = 0;
    int edgeCount = 0;

    if(verbose) cout << "[ " << v << " ";
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
        if(verbose) cout << "(" << minDist << ") " << bestV << " ";
        totalDist += minDist;
        edgeCount++;
        v = bestV;
        if(edgeCount == numRegions - 1 && verbose) cout << "(" << distMatrix[v][0] << ") ]" << endl;
    }
    return totalDist;
}

void swapPath(int busLine[], int i, int j) {
    i = i + 1;
    while(i < j){
        int temp = busLine[i];
        busLine[i] = busLine[j];
        busLine[j] = temp;
        i++; j--;
    }
}

float optimizeBusLines(int numRegions, int busLine[], float** distMatrix, float totalDist, bool verbose){
    bool improved = true;
    while(improved){
        improved = false;
        for(int i = 0; i < numRegions - 1; i++){
            for(int j = i + 1; j < numRegions; j++){
                int deltaCost = -distMatrix[busLine[i]][busLine[i + 1]] - distMatrix[busLine[j]][busLine[(j + 1) % numRegions]]
                               + distMatrix[busLine[i]][busLine[j]] + distMatrix[busLine[i + 1]][busLine[(j + 1) % numRegions]];
                if (deltaCost < 0){
                    totalDist += deltaCost;
                    swapPath(busLine, i, j);
                    improved = true;
                    if(verbose) cout << "Pontos " << busLine[i + 1] << " e " << busLine[j] << " trocaram de lugar no ciclo: [ " << busLine[0] << " ";
                    if(verbose) for(int k = 1; k < numRegions; k++) {cout << "(" << distMatrix[busLine[k - 1]][busLine[k]] << ") " << busLine[k] << " ";}
                    if(verbose) cout << " ] novo custo e: " << totalDist << endl;
                }
            }
        }
    }
    return totalDist;
}

Graph buildBusGraph(CityGraph& city, bool verbose) {
    Graph busFull = Graph(city.numRegions(), 0);

    //ESCOLHA DOS PONTOS QUE DEVE PASSAR NA REGIÃO
    if(verbose) cout << "Fazendo escolha dos pontos nos quais a linha deve passar, para cada regiao: " << endl;
    int points[city.numRegions()];
    int** path = new int*[city.numRegions()];
    for (int i = 0; i < city.numRegions(); i++){
        path[i] = new int[city.numNodes()];
    }

    for (int i = 0; i < city.numRegions(); i++) {
        genBusPoints(city, i, points, verbose);
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

    //DETERMINAÇÃO DO PRIMEIRO CICLO HAMILTONIANO QUE TEM
    if(verbose) cout << endl << "Definindo o primeiro ciclo, que sera otimizado:" << endl;
    int busLine[city.numRegions()];
    for(int i = 0; i < city.numRegions(); i++) {busLine[i] = -1;}
    float totalDist = genBusLines(city.numRegions(), busLine, distMatrix, verbose);

    //OTIMIZAÇÃO DO CICLO O(r^4)
    if(verbose) cout << endl << "Otimizando o ciclo, trocando pares de arestas:" << endl;
    totalDist = optimizeBusLines(city.numRegions(), busLine, distMatrix, totalDist, verbose);

    Graph busGraph = Graph(city.numNodes(), 0);

    ofstream outFile1("data/city-1/bus-edges.csv");
    outFile1 << "node1,node2" << endl;
    //busFull.print();
    if (verbose) cout << endl << "Linha completa: [ ";
    for(int i = city.numRegions(); i > 0; i--){
        int v = points[busLine[i % city.numRegions()]];
        int end = points[busLine[i - 1]];
        int last = v;
        while(end != v){
            if (verbose) cout << v << " ";
            city.busPoints->add(v);
            EdgeNode* node = copyStreetInfo(city, path[busLine[i - 1]][v], v);
            busGraph.addSegment(path[busLine[i - 1]][v], v, node);
            v = path[busLine[i - 1]][v];
            if (last != v) { outFile1 << last << "," << v << endl; }
            last = v;
        }
        //if(i == 1){
        //    EdgeNode* node = copyStreetInfo(city, v, points[busLine[0]]);
        //    busGraph.addSegment(v, points[busLine[0]], node);
        //}
    } 
    if (verbose) cout << "]" << endl;
    return busGraph;
}

