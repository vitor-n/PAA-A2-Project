#include <iostream>
#include <climits>

#include "graph.h"
#include "genSubway.h"
#include "heap.h"

Graph Graph::genMST() {
    Graph mst = Graph(m_numVertices, 0);
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

    Heap heap = Heap<int>(m_numVertices, MIN);
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
    for (vertex v = 1; v < m_numVertices; v++) {
        if (parent[v] != -1) {
            EdgeNode* mstEdge1 = new EdgeNode;
            EdgeNode* mstEdge2 = new EdgeNode;
            mstEdge1->lenght = distance[v];
            mstEdge2->lenght = distance[v];
            mst.addSegment(parent[v], v, mstEdge1);
            mst.addSegment(v, parent[v], mstEdge2);
        }
    }

    return mst;
}

void genSubwayStations(CityGraph& cityGraph, int region, int stations[]) {
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

    cout << "SubwayStation " << region << ": " << bestNode << endl;
    stations[region] = bestNode;
}