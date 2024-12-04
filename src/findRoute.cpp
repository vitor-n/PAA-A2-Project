#include <iostream>

#include "graph.h"
#include "hashTable.h"
#include "findRoute.h"
#include "trafficAPI.h"

#define printList(v, n) {cout << "[ "; for (int i = 0; i < n; i++) { cout << v[i] << " "; }; cout << " ]" << endl;}

float compareSpeed(EdgeNode* node) {
    double speed = node->maxSpeed;
    int street = node->street;
    int region = node->region;
    speed = detTraffic(street, region, speed);
    cout << speed << endl;
    cout << node->lenght << endl;
    return (node->lenght )/ (speed / 3.6);
}

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
    return;
}

EdgeNode* copyStreetInfo(CityGraph& city, int v1, int v2) {
    EdgeNode* edge = new EdgeNode;
    EdgeNode* node = city.m_edges(v1);
    while (node) {
        if (node->endVertex == v2) {
            edge->maxSpeed = node->maxSpeed;

            edge->nBuildings = node->nBuildings;
            edge->nComercial = node->nComercial;
            edge->nResidential = node->nResidential;
            edge->nIndustrial = node->nIndustrial;
            edge->nTouristic = node->nTouristic;
            return edge;
        }
        node = node->next;
    }
    return nullptr;
}

void findRoute(CityGraph& city, int adress1[], int adress2[], int route[], float distance[], float (*func)(EdgeNode*)) {
    int v1 = -1, v2 = -1;
    float dist_v1 = -1, dist_v2 = -1;

    findEdge(city, adress1[0], adress1[1], adress1[2], v1, v2, dist_v1, dist_v2);

    int v3 = -1, v4 = -1;
    float dist_v3 = -1, dist_v4 = -1;

    findEdge(city, adress2[0], adress2[1], adress2[2], v3, v4, dist_v3, dist_v4);

    if (v1 == -1 || v2 == -1 || v3 == -1 || v4 == -1) {
        cout << "Couldn't find a route between the two adresses" << endl;
        return;
    }

    EdgeNode* edge1 = copyStreetInfo(city, v1, v2);
    EdgeNode* edge2 = copyStreetInfo(city, v1, v2);
    edge1->lenght = dist_v1;
    edge2->lenght = dist_v1;
    EdgeNode* edge3 = copyStreetInfo(city, v3, v4);
    EdgeNode* edge4 = copyStreetInfo(city, v3, v4);
    edge3->lenght = dist_v3;
    edge4->lenght = dist_v3;

    int vTemp1 = city.numNodes()-1;
    int vTemp2 = city.numNodes()-2;

    city.addSegment(vTemp1, v1, edge1);
    city.addSegment(vTemp1, v2, edge2);
    city.addSegment(v3, vTemp2, edge3);
    city.addSegment(v4, vTemp2, edge4);
    
    city.CPTDijkstra(vTemp1, route, distance, func);

    city.removeSegment(vTemp1, v1);
    city.removeSegment(vTemp1, v2);
    city.removeSegment(v3, vTemp2);
    city.removeSegment(v4, vTemp2);
}