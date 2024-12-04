#include <iostream>
#include <limits>
#include "graph.h"
#include "cityParser.h"
#include "genSubway.h"
#include "heap.h"

#define printList(v, n) {cout << "[ "; for (int i = 0; i < n; i++) { cout << v[i] << " "; }; cout << " ]" << endl;}

using namespace std;

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

    for(int i = 0; i < subwayMST.numNodes(); i++){
        EdgeNode* node = subwayMST.m_edges(i);
        while(node){
            int j = node->endVertex;
            if(j < i){
                node = node->next;
                continue;
            }
            int v = stations[j];
            cout << "Region " << i << " to " << node->endVertex << " [ ";
            while(path[i][v] != v){
                cout << v << " ";
                v = path[i][v];
            }
            cout << v << " ]" << endl;
            node = node->next;
        }
    }

    return 0;
}
