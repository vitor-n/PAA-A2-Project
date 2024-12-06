#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <climits>

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
    vector<string> row;
    
    getline(file, line);
    vector<string> header = split(line, CSVDELIMITER);

    while (getline(file, line)) {
        row = split(line, CSVDELIMITER);
        int node1 = stoi(row[0]);
        int node2 = stoi(row[1]);
        int region = stoi(row[2]);
        int street = stoi(row[3]);
        int streetStart = stoi(row[4]);
        float length = stof(row[5]);
        int maxSpeed = stoi(row[6]);
        int nBuildings = stoi(row[7]);
        int numResidentials = stoi(row[8]);
        int numIndustrial = stoi(row[9]);
        int numComercial = stoi(row[10]);
        int numTouristic = stoi(row[11]);
        float escavationCost = stof(row[12]);

        cityGraph.regions[region]->add(node1);
        cityGraph.regions[region]->add(node2);

        EdgeNode* node = new EdgeNode;
        node->lenght = length;
        node->maxSpeed = maxSpeed;
        node->escavationCost = escavationCost;

        node->nBuildings = nBuildings;
        node->nResidential = numResidentials;
        node->nIndustrial = numIndustrial;
        node->nComercial = numComercial;
        node->nTouristic = numTouristic;

        node->region = region;
        node->street = street;
        node->streetStart = streetStart;

        cityGraph.addSegment(node1, node2, node);
    }
}

CityGraph cityParser(string folderPath) {
    map<string, int> cfg = configFromCFG(folderPath);

    CityGraph cityGraph(cfg["num_nodes"], cfg["num_regions"]);
    
    nodesFromCSV(cityGraph, folderPath);
    edgesFromCSV(cityGraph, folderPath);

    return cityGraph;
}
