#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "graph.h"
#include "cityParser.h"

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

void nodesFromCSV(Graph& cityPath, string folderPath) {
    ifstream file(folderPath + "/city-edges.csv");

    return;
}

void edgesFromCSV(Graph& cityGraph, string folderPath) {
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

        cityGraph.addEdge(node1, node2);
    }
}

Graph cityParser(string folderPath) {
    map<string, int> cfg = configFromCFG(folderPath);

    Graph cityGraph(cfg["num_nodes"]);
    
    nodesFromCSV(cityGraph, folderPath);
    edgesFromCSV(cityGraph, folderPath);

    return cityGraph;
}

int main () {
    Graph cityGraph = cityParser("data/city-1");
    cityGraph.print();
}