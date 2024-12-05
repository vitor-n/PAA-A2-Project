#include <iostream>
#include <climits>

#include "graph.h"
#include "hashTable.h"
#include "findRoute.h"
#include "trafficAPI.h"

#define printList(v, n) {cout << "[ "; for (int i = 0; i < n; i++) { cout << v[i] << " "; }; cout << " ]" << endl;}


int NUM_ROUTES = 6;

float compareCar(EdgeNode* node) {
    double speed = node->maxSpeed;
    int street = node->street;
    int region = node->region;
    speed = detTraffic(street, region, speed);
    return (node->lenght) / (speed / 3.6);
}

float compareBus(EdgeNode* node) {
    double speed = 70;
    int street = node->street;
    int region = node->region;
    speed = detTraffic(street, region, speed);
    return (node->lenght) / (speed / 3.6);
}

float compareWalking(EdgeNode* node) {
    return (node->lenght) / (5 / 3.6);
}

float compareSubway(EdgeNode* node) {
    return (node->lenght) / (70 / 3.6);
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

bool findRoute(CityGraph& city, int adress1[], int adress2[], int route[], float distance[], float (*func)(EdgeNode*)) {
    int v1 = -1, v2 = -1;
    float dist_v1 = -1, dist_v2 = -1;

    findEdge(city, adress1[0], adress1[1], adress1[2], v1, v2, dist_v1, dist_v2);

    int v3 = -1, v4 = -1;
    float dist_v3 = -1, dist_v4 = -1;

    findEdge(city, adress2[0], adress2[1], adress2[2], v3, v4, dist_v3, dist_v4);

    if (v1 == -1 || v2 == -1 || v3 == -1 || v4 == -1) {
        cout << "Couldn't find a route between the two adresses" << endl;
        return false;
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

    return true;
}

int findClosestSubway(CityGraph& city, float dists[]) {
    float minDist = INT_MAX;
    int v = -1;
    for (auto it = city.subwayStations->begin(); it.hasNext(); it.next()) {
        int station = it.value();
        if (dists[station] < minDist) {
            minDist = dists[station];
            v = station;
        }
    }
    cout << "Closest subway station is: " << v << endl;
    return v;
}

int findClosestBus(CityGraph& city, float dists[]) {
    float minDist = INT_MAX;
    int v = -1;
    for (auto it = city.busPoints->begin(); it.hasNext(); it.next()) {
        int point = it.value();
        if (dists[point] < minDist) {
            minDist = dists[point];
            v = point;
        }
    }
    cout << "Closest bus stop is: " << v << endl;
    return v;
}

float minCost(Graph& graph, int v1, int v2, float (*func)(EdgeNode*)) {
    int parents[graph.numNodes()];
    float distance[graph.numNodes()];
    graph.CPTDijkstra(v1, parents, distance, func);
    return distance[v2];
}

void findBestRoute(CityGraph& city, Graph& subway, Graph& bus, int adress1[], int adress2[], float info[], int maxCost) {
    int v_from = city.numNodes() - 1;
    int v_to = city.numNodes() - 2;

    int routeForwardWalk[city.numNodes()];
    int routeBackwardWalk[city.numNodes()];
    float distanceForwardWalk[city.numNodes()];
    float distanceBackwardWalk[city.numNodes()];

    int routeForwardCar[city.numNodes()];
    int routeBackwardCar[city.numNodes()];
    float distanceForwardCar[city.numNodes()];
    float distanceBackwardCar[city.numNodes()];

    if (!findRoute(city, adress1, adress2, routeForwardWalk, distanceForwardWalk, &compareWalking)) return;
    if (!findRoute(city, adress2, adress1, routeBackwardWalk, distanceBackwardWalk, &compareWalking)) return;

    if (!findRoute(city, adress1, adress2, routeForwardCar, distanceForwardCar, &compareCar)) return;
    if (!findRoute(city, adress2, adress1, routeBackwardCar, distanceBackwardCar, &compareCar)) return;

    int vWSF = findClosestSubway(city, distanceForwardWalk);
    int vWSB = findClosestSubway(city, distanceBackwardWalk);
    int vWBF = findClosestBus(city, distanceForwardWalk);
    int vWBB = findClosestBus(city, distanceBackwardWalk);

    int vCSF = findClosestSubway(city, distanceForwardCar);
    int vCSB = findClosestSubway(city, distanceBackwardCar);
    int vCBF = findClosestBus(city, distanceForwardCar);
    int vCBB = findClosestBus(city, distanceBackwardCar);

    float times[NUM_ROUTES];

    // Walking -> Walking
    times[0] = distanceForwardWalk[v_to];

    // Car -> Car
    times[1] = distanceForwardCar[v_to];

    // Walking -> Subway -> Walking
    times[2] = distanceForwardWalk[vWSF] + minCost(subway, vWSF, vWSB, &compareSubway) + distanceBackwardWalk[vWSB];

    // Walking -> Bus -> Walking
    times[3] = distanceForwardWalk[vWBF] + minCost(bus, vWBF, vWBB, &compareBus) + distanceBackwardWalk[vWBB];

    // Car -> Subway -> Car
    times[4] = distanceForwardCar[vCSF] + minCost(subway, vCSF, vCSB, &compareSubway) + distanceBackwardCar[vCSB];

    // Car -> Bus -> Car
    times[5] = distanceForwardCar[vCBF] + minCost(bus, vCBF, vCBB, &compareBus) + distanceBackwardCar[vCBB];

    int minTime = INT_MAX;
    int bestRoute = -1;

    for (int i = 0; i < NUM_ROUTES; i++) {
        cout << times[i] << " " << i << endl;
        if (times[i] < minTime || bestRoute == -1) {
            minTime = times[i];
            bestRoute = i;
        }
    }

    switch (bestRoute) {
        case -1:
            cout << "Couldn't find a route" << endl;
            break;
        case 0:
            cout << "Walking -> Walking" << endl;
            break;
        case 1:
            cout << "Car -> Car" << endl;
            break;
        case 2:
            cout << "Walking -> Subway -> Walking" << endl;
            break;
        case 3:
            cout << "Walking -> Bus -> Walking" << endl;
            break;
        case 4:
            cout << "Car -> Subway -> Car" << endl;
            break;
        case 5:
            cout << "Car -> Bus -> Car" << endl;
            break;
    }
}