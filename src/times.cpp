#include <iostream>
#include <chrono>
#include <fstream>
#include <limits>
#include "graph.h"
#include "cityParser.h"
#include "genSubway.h"
#include "heap.h"
#include "times.h"

using namespace std;

using chrono::high_resolution_clock;
using chrono::duration_cast;
using chrono::nanoseconds;

void getTimes(int numCities, const long int N) {

    for (int numCity = 1; numCity < numCities + 1; numCity++){
        string strNumCity = to_string(numCity);
        string strPath = "data/times/city-" + strNumCity + ".csv";
        ofstream outputFile(strPath, ios::out | ios::trunc);
        cout << strPath << endl;
        outputFile << "genSubway" << endl;

        CityGraph city = cityParser("data/city-" + strNumCity);

        Graph subwayFull = Graph(city.numRegions(), 0);

        for (int i = 0; i < N; i++){

            // Começa a contagem do tempo da primeira operação
            auto timeStart1 = high_resolution_clock::now();

            int stations[city.numRegions()];
            
            int** path = new int*[city.numRegions()];

            for (int i = 0; i < city.numRegions(); i++){
                path[i] = new int[city.numNodes()];
                genSubwayStations(city, i, stations);
            }

            Graph* subwayMST = genSubwayLines(city, subwayFull, stations, path, false);
            // Encerra a contagem de tempo da primeira operação
            auto timeStop1 = high_resolution_clock::now();

            auto timeDuration1 = duration_cast<nanoseconds>(timeStop1 - timeStart1);

            outputFile << timeDuration1.count() << endl;

            for (int i = 0; i < city.numRegions(); i++) delete path[i];

            delete[] path;
            delete subwayMST;
        }
        outputFile.close();
    }
}
