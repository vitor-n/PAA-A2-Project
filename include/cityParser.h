#ifndef CITYPARSER_H
#define CITYPARSER_H

#include <vector>
#include <string>
#include <map>

#include "graph.h"

std::vector<std::string> split(const std::string &s, char delimiter);
std::map<std::string, int> configFromCFG(std::string folderPath);
void nodesFromCSV(Graph& cityPath, std::string folderPath);
void edgesFromCSV(Graph& cityGraph, std::string folderPath);
Graph cityParser(std::string folderPath);

#endif