#include <iostream>

#include "graph.h"
#include "hashTable.h"
#include "findRoute.h"

void findEdge(CityGraph& city, int region, int street, int number, int& v1, int& v2, float& dist_v1, float& dist_v2) {
    int v = -1;
    int startNode = -1;
    int i = 0;
    for (auto it = city.regions[region]->begin(); it.hasNext(); it.next()) {
        v = it.value();
        
        EdgeNode* node = city.m_edges(v);
        while(node){
            if (node->street == street) { 
                startNode = node->streetStart;
                break;
             }
            node = node->next;
        }
    }

    HashTable<bool> visited = HashTable<bool>(20);
    int nBuildings = 0;

    if (startNode == -1) {
        cout << "Couldn't locate: ";
        cout << "<ZIP: " << region << " Street: " << street << " N: " << number << ">" <<  endl;
        cout << "Consider taking a look at the city map (city-edges.csv)" << endl;
        return;
    } else {
        EdgeNode* node = city.m_edges(startNode);
        while (node) {
            if (node->street != street || visited.get(node->endVertex) != nullptr) {
                node = node->next;
                continue;
            }

            visited.set(startNode, true);
            cout << startNode << node->endVertex << endl;
            startNode = node->endVertex;
            node = city.m_edges(node->endVertex);

            if (nBuildings <= number && number <= nBuildings + node->nBuildings) {
                v1 = node->endVertex;
                v2 = startNode;
                dist_v1 = (number - nBuildings) * node->lenght / node->nBuildings;
                dist_v2 = node->lenght - dist_v1;
                return;
            }
            nBuildings += node->nBuildings;

        }
    }
    if (number > nBuildings) {
        cout << "Couldn't locate: ";
        cout << "<ZIP: " << region << " Street: " << street << " N: " << number << ">" <<  endl;
        cout << "This street numbers goes from 1 to " << nBuildings << endl;
    }
}