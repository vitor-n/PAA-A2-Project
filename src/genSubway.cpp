#include <iostream>
#include <climits>
#include <fstream>

#include "graph.h"
#include "genSubway.h"
#include "heap.h"
#include "cityParser.h"

using namespace std;

Graph* Graph::genMSTPrim(bool verbose) {
    Graph* mst = new Graph(m_numVertices, 0);
    const int INF = 2147483647;
    int distance[m_numVertices];
    bool visited[m_numVertices];
    int parent[m_numVertices];

    for (vertex v = 0; v < m_numVertices; v++) {
        distance[v] = INF;
        visited[v] = false;
        parent[v] = -1;
    }

    distance[0] = 0;
    parent[0] = -1;

    Heap<int> heap = Heap<int>(m_numVertices, MIN);
    heap.set_mapping(distance);
    heap.insert(0);

    while (heap.heapSize() != 0) {
        vertex v1 = heap.pop_top(); // Removing v1 from heap
        visited[v1] = true;

        EdgeNode* node = m_edges(v1);
        while (node) {
            vertex v2 = node->endVertex;
            if (!visited[v2] && node->lenght < distance[v2]) {
                distance[v2] = node->lenght;
                parent[v2] = v1;
                heap.insert(v2);
            }
            node = node->next;
        }
    }

    // Build the MST using the parent array
    if (verbose) cout << endl << "Determinando quais estacoes estarao interligadas:" << endl;
    for (vertex v = 1; v < m_numVertices; v++) {
        if (parent[v] != -1) {
            EdgeNode* mstEdge1 = new EdgeNode;
            EdgeNode* mstEdge2 = new EdgeNode;
            mstEdge1->lenght = distance[v];
            mstEdge2->lenght = distance[v];
            mst->addSegment(parent[v], v, mstEdge1);
            mst->addSegment(v, parent[v], mstEdge2);
            if (verbose) cout << "Estacoes " << parent[v] << " e " << v <<
                " (custo de escavacao: " << distance[v] << ")" << endl;
        }
    }

    return mst;
}

void genSubwayStations(CityGraph& cityGraph, int region, int stations[], bool verbose) {
    int maxDist[cityGraph.numNodes()];
    int distances[cityGraph.numNodes()];
    for (int i = 0; i < cityGraph.numNodes(); i++) {
        maxDist[i] = -1;
    }

    for (auto it1 = cityGraph.regions[region]->begin(); it1.hasNext(); it1.next()) {
        cityGraph.CPTDijkstraRegion(it1.value(), distances, region, &compareLenght);
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

    if (verbose) cout << "Regiao " << region << ": " << bestNode << endl;
    stations[region] = bestNode;
}

Graph* genSubwayLines(CityGraph& city, int stations[], int **path, bool verbose){
    int cost[city.numNodes()];
    Graph subwayFull = Graph(city.numRegions(), 0);

    for (int i = 0; i < city.numRegions(); i++) {
        city.CPTDijkstra(stations[i], path[i], cost, &compareCost);
        for (int j = 0; j < city.numRegions(); j++) {
            if (i == j) continue; 

            EdgeNode* edge = new EdgeNode;
            edge->lenght = cost[stations[j]];

            subwayFull.addSegment(i, j, edge);
        }
    }

    Graph* subwayMST = subwayFull.genMSTPrim(verbose);
    return subwayMST;
}

Graph buildSubwayGraph(CityGraph& city, bool verbose) {
    int stations[city.numRegions()];
    int** path = new int*[city.numRegions()];
    if (verbose) cout << "Selecionando qual cruzamento sera a estacao de cada regiao:" << endl;
    for (int i = 0; i < city.numRegions(); i++){
        path[i] = new int[city.numNodes()];
    }
    for (int i = 0; i < city.numRegions(); i++) {
        genSubwayStations(city, i, stations, verbose);
    }
    Graph subwayMST = *genSubwayLines(city, stations, path, verbose);

    Graph subwayGraph = Graph(city.numNodes(), 0);
    ofstream outFile1("data/city-1/subway-edges.csv");
    ofstream outFile2("data/city-1/subway-nodes.csv");
    outFile2 << "station" << endl;
    for (int i = 0; i < city.numRegions(); i++) {
        outFile2 << stations[i] << endl;
    }
    outFile1 << "node1,node2" << endl;
    if (verbose) cout << endl << "Exibindo caminhos completos entre cada estacao: " << endl;
    for (int i = 0; i < subwayMST.numNodes(); i++) {
        EdgeNode* node = subwayMST.m_edges(i);
        while(node){
            int j = node->endVertex;
            if(j < i){
                node = node->next;
                continue;
            }
            int v = stations[j];
            if (verbose) cout << "Estacao " << i << " para " << node->endVertex << " [ ";
            int last = v;
            while(path[i][v] != v){
                if (verbose) cout << v << " ";
                city.subwayStations->add(v);
                EdgeNode* node1 = copyStreetInfo(city, path[i][v], v);
                EdgeNode* node2 = copyStreetInfo(city, v, path[i][v]);
                subwayGraph.addSegment(path[i][v], v, node1);
                subwayGraph.addSegment(v, path[i][v], node2);
                v = path[i][v];
                if (last != v) { outFile1 << last << "," << v << endl; }
                last = v;
            }
            if (verbose) cout << v << " ]" << endl;
            node = node->next;
        }
    }
    return subwayGraph;
}
