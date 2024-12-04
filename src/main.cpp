#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include "graph.h"
#include "cityParser.h"
#include "genSubway.h"
#include "heap.h"
#include "times.h"
#include "findRoute.h"

#define printList(v, n) {cout << "[ "; for (int i = 0; i < n; i++) { cout << v[i] << " "; }; cout << " ]" << endl;}

using namespace std;



int main () {
     CityGraph city = cityParser("data/city-1");
/*  CityGraph city = cityParser("data/city-1");
    
    int stations[city.numRegions()];
    int** path = new int*[city.numRegions()];
    for (int i = 0; i < city.numRegions(); i++){
        path[i] = new int[city.numNodes()];
    }

    for (int i = 0; i < city.numRegions(); i++) {
        genSubwayStations(city, i, stations);
    }

    Graph subwayMST = *genSubwayLines(city, subwayFull, stations, path, true);

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
            cout << "Region " << i << " to " << node->endVertex;
            cout << " (cost: " << node->lenght << ") [ ";
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

    findEdge(city, 6, 0, 18, v1, v2, dist_v1, dist_v2);

    cout << "v1: " << v1 << " v2: " << v2 << " dist_v1: " << dist_v1 << " dist_v2: " << dist_v2 << endl;
*/  
    int adress1[] = {18, 0, 5};
    int adress2[] = {13, 6091, 19};

    int parents[city.numNodes()];
    int route[city.numNodes()];
    float distance[city.numNodes()];

    int v1x = city.numNodes()-1;
    int v2x = city.numNodes()-2;

    findRoute(city, adress1, adress2, route, distance, &compareLenght);
    findRoute(city, adress2, adress1, route, distance, &compareLenght);
    cout << compareSpeed(city.m_edges(11)) << endl;
    cout << distance[v2x] << endl;

    return 0;
}
