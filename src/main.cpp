#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include "graph.h"
#include "cityParser.h"
#include "genSubway.h"
#include "heap.h"

#include "hashTable.h"

#define printList(v, n) {cout << "[ "; for (int i = 0; i < n; i++) { cout << v[i] << " "; }; cout << " ]" << endl;}

using namespace std;

void findEdge(CityGraph& city, int region, int street, int number, int& v1, int& v2, float& dist_v1, float& dist_v2) {
    int v = -1;
    int startNode = -1;
    int i = 0;
    for (auto it = city.regions[region]->begin(); it.hasNext(); it.next()) {
        v = it.value();
        
        EdgeNode* node = city.m_edges(v);
        while(node){
            if (node->street == street) { 
                startNode = v;
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
        int lastNode = startNode;
        while (node) {
            if (node->street != street and visited.get(lastNode) != nullptr) { 
                node = node->next;
                continue;
            }
            visited.set(startNode, true);

            if (nBuildings <= number <= nBuildings + node->nBuildings) {
                v1 = lastNode;
                v2 = node->endVertex;
                dist_v1 = (number - nBuildings) * (node->lenght / node->nBuildings);
                dist_v2 = node->lenght - dist_v1;
                return;
            }

            nBuildings += node->nBuildings;
            visited.set(node->endVertex, true);
            node = city.m_edges(node->endVertex);
            lastNode = startNode;
            node = node->next;
        }
        if (number > nBuildings) {
            cout << "Couldn't locate: ";
            cout << "<ZIP: " << region << " Street: " << street << " N: " << number << ">" <<  endl;
            cout << "This street numbers goes from 1 to" << nBuildings << endl;
        }
    }
}

int main () {
    CityGraph city = cityParser("data/city-1");
    Graph subwayFull = Graph(city.numRegions(), 0);

    int stations[city.numRegions()];
    int** path = new int*[city.numRegions()];
    for (int i = 0; i < city.numRegions(); i++){
        path[i] = new int[city.numNodes()];
    }

    for (int i = 0; i < city.numRegions(); i++) {
        genSubwayStations(city, i, stations);
    }

    Graph subwayMST = genSubwayLines(city, subwayFull, stations, path);

    subwayFull.print();
    subwayMST.print();
    
    ofstream outFile1("data/city-1/city-subway.csv");
    ofstream outFile2("data/city-1/subway-stations.csv");

    outFile2 << "station" << endl;
    for (int i = 0; i < city.numRegions(); i++) {
        outFile2 << stations[i] << endl;
    }
    outFile1 << "node1,node2" << endl;

    for (int i = 0; i < subwayMST.numNodes(); i++) {
        EdgeNode* node = subwayMST.m_edges(i);
        while(node){
            int j = node->endVertex;
            if(j < i){
                node = node->next;
                continue;
            }
            int v = stations[j];
            cout << "Region " << i << " to " << node->endVertex << " [ ";
            int last = v;
            while(path[i][v] != v){
                cout << v << " ";
                v = path[i][v];
                if (last != v) { outFile1 << last << "," << v << endl; }
                last = v;
            }
            cout << v << " ]" << endl;
            node = node->next;
        }
    }

    int v1 = -1, v2 = -1;
    float dist_v1 = -1, dist_v2 = -1;

    findEdge(city, 6, 0, 10, v1, v2, dist_v1, dist_v2);

    cout << "v1: " << v1 << " v2: " << v2 << " dist_v1: " << dist_v1 << " dist_v2: " << dist_v2 << endl;

    return 0;
}
