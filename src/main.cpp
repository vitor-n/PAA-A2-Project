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



//int main () {
//    CityGraph city = cityParser("data/city-1");
//    CityGraph city = cityParser("data/city-1");


    // int adress1[] = {18, 0, 5};
    // int adress2[] = {13, 6091, 19};
    // int parents[city.numNodes()];
    // int route[city.numNodes()];
    // float distance[city.numNodes()];
    // int v1x = city.numNodes()-1;
    // int v2x = city.numNodes()-2;
    // findRoute(city, adress1, adress2, route, distance, &compareLenght);
    // findRoute(city, adress2, adress1, route, distance, &compareLenght);
    // cout << compareSpeed(city.m_edges(11)) << endl;
    // cout << distance[v2x] << endl;
    // return 0;
//}
