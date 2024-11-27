#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "graph.h"
#include "cityParser.h"

#define printList(v, n) {cout << "[ "; for (int i = 0; i < n; i++) { cout << v[i] << " "; }; cout << " ]" << endl;}

using namespace std;

const char CSVDELIMITER = ',';
const char CFGDELIMITER = '=';

vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

map<string, int> configFromCFG(string folderPath) {
    ifstream file(folderPath + "/city.cfg");
    string line;
    vector<string> row;
    map<string, int> config;

    while (getline(file, line)) {
        row = split(line, CFGDELIMITER);
        config[row[0]] = stoi(row[1]);
    }
    return config;
}

void nodesFromCSV(CityGraph& cityPath, string folderPath) {
    ifstream file(folderPath + "/city-edges.csv");

    return;
}

void edgesFromCSV(CityGraph& cityGraph, string folderPath) {
    ifstream file(folderPath + "/city-edges.csv");
    string line;
    int numRows = 0;
    vector<string> row;
    
    getline(file, line);
    vector<string> header = split(line, CSVDELIMITER);

    int node1;
    int node2;
    float length;
    int numBuildings;
    int numResidentials;
    int maxSpeed;
    string region;
    string street;
    bool hasBusLane;
    float costToEscavate;

    while (getline(file, line)) {
        row = split(line, CSVDELIMITER);
        node1 = stoi(row[0]);
        node2 = stoi(row[1]);
        length = stof(row[2]);
        numBuildings = stoi(row[3]);
        numResidentials = stoi(row[4]);
        maxSpeed = stoi(row[5]);
        region = row[6];
        street = row[7];
        hasBusLane = stoi(row[8]);

        EdgeNode* node = new EdgeNode;
        node->lenght = length;
        node->maxSpeed = maxSpeed;
        node->escavationCost = costToEscavate;

        node->nComercial = numResidentials - numBuildings;
        node->nResidential = numResidentials;
        node->nIndustrial = 0;
        node->nTouristic = 0;

        node->region = region;
        node->street = street;
        node->streetOffset = 0;

        cityGraph.addSegment(node1, node2, node);
    }
}

CityGraph cityParser(string folderPath) {
    map<string, int> cfg = configFromCFG(folderPath);

    CityGraph cityGraph(cfg["num_nodes"]);
    
    nodesFromCSV(cityGraph, folderPath);
    edgesFromCSV(cityGraph, folderPath);

    return cityGraph;
}

int main () {
    CityGraph cityGraph = cityParser("data/city-1");

    vertex g7Parents[10000];
    int g7Distances[10000];

    int v1 = 0;
    int v2 = 7;

    cityGraph.CPTDijkstra(v1, g7Parents, g7Distances);
    
    for(int i = 0; i < 10; i++){
        cout << " " << g7Distances[i];
    }
    cout << endl;
    printList(g7Parents, 10);
    cout << g7Parents[105] << endl;
    cout << g7Parents[104] << endl;
    cout << g7Parents[102] << endl;
    cout << g7Distances[v2] << endl;

}
