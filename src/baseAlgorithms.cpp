#include "graph.h"
#include "heap.h"

template <typename T>
void CityGraph::CPTDijkstra(vertex v0, vertex parents[], T distance[]){
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
                if (distance[v1] + node->lenght < distance[v2]){
                    distance[v2] = distance[v1] + node->lenght;
                    parents[v2] = v1;
                    heap.insert(v2);
                }
            }
            node = node->next;
        }
        visited[v1] = true;
    }
}

template void CityGraph::CPTDijkstra(vertex, vertex[], int[]);
template void CityGraph::CPTDijkstra(vertex, vertex[], float[]);
