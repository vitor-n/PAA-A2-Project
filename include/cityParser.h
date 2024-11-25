#ifndef CITYPARSER_H
#define CITYPARSER_H

#include <vector>
#include <string>
#include <map>

#include "graph.h"

std::vector<std::string> split(const std::string &s, char delimiter);
std::map<std::string, int> configFromCFG(std::string folderPath);
void nodesFromCSV(CityGraph& cityPath, std::string folderPath);
void edgesFromCSV(CityGraph& cityGraph, std::string folderPath);
CityGraph cityParser(std::string folderPath);

#endif
