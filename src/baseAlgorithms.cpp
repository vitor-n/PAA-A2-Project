#include <iostream>
#include <cstdio>

#include "graph.h"
#include "heap.h"

template<typename T>
T compareLenght(EdgeNode* node){
    return node->lenght;
}

template<typename T>
T compareCost(EdgeNode* node){
    return node->escavationCost;
}

template <typename T>
void CityGraph::CPTDijkstra(vertex v0, vertex parents[], T distance[], T (*func)(EdgeNode*)){
    //inicialization
    const T INF = 2147483647;
    bool visited[m_numVertices];
    for(vertex v = 0; v < m_numVertices; v++){
        parents[v] = -1;
        distance[v] = INF;
        visited[v] = false;
    }
    //root case
    distance[v0] = 0;
    parents[v0] = v0;

    //heap inicialization and insertion
    Heap heap = Heap<T>(m_numVertices, MIN);
    heap.set_mapping(distance);
    heap.insert(v0);

    while(heap.heapSize() != 0){
        vertex v1 = heap.pop_top(); //removing v1 from heap
        if(distance[v1] == INF) break;

        EdgeNode* node = m_edges(v1);
        while(node){
            vertex v2 = node->endVertex;
            if(!visited[v2]){
                if (distance[v1] + func(node) < distance[v2]){
                    distance[v2] = distance[v1] + func(node);
                    parents[v2] = v1;
                    heap.insert(v2);
                }
            }
            node = node->next;
        }
        visited[v1] = true;
    }
}

template <typename T>
void CityGraph::CPTDijkstraRegion(vertex v0, T distance[], int region, T (*func)(EdgeNode*)){
    //inicialization
    const T INF = 2147483647;
    bool visited[m_numVertices];
    for(vertex v = 0; v < m_numVertices; v++){
        distance[v] = INF;
        visited[v] = false;
    }
    //root case
    distance[v0] = 0;

    //heap inicialization and insertion
    Heap heap = Heap<T>(m_numVertices, MIN);
    heap.set_mapping(distance);
    heap.insert(v0);

    while(heap.heapSize() != 0){
        vertex v1 = heap.pop_top(); //removing v1 from heap
        if (distance[v1] == INF) break;

        EdgeNode* node = m_edges(v1);
        while(node) {
            vertex v2 = node->endVertex;
            if (node->region != region) {
                node = node->next;
                continue;
            }
            if (!visited[v2]) {
                if (distance[v1] + func(node) < distance[v2]){
                    distance[v2] = distance[v1] + func(node);
                    heap.insert(v2);
                }
            }
            node = node->next;
        }
        visited[v1] = true;
    }
}

template void CityGraph::CPTDijkstra(vertex, vertex[], int[], int(EdgeNode*));
template void CityGraph::CPTDijkstra(vertex, vertex[], float[], float(EdgeNode*));

template void CityGraph::CPTDijkstraRegion(vertex, int[], int, int(EdgeNode*));
template void CityGraph::CPTDijkstraRegion(vertex, float[], int, float(EdgeNode*));

template int compareLenght(EdgeNode*);
template int compareCost(EdgeNode*);
template float compareLenght(EdgeNode* node);
template float compareCost(EdgeNode*);
