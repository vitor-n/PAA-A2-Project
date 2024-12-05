#include <iostream>
#include <chrono>
#include <fstream>
#include <limits>
#include "graph.h"
#include "cityParser.h"
#include "genSubway.h"
#include "genBus.h"
#include "heap.h"
#include "times.h"

using namespace std;

using chrono::high_resolution_clock;
using chrono::duration_cast;
using chrono::nanoseconds;


void getTimes(int numCities) {

    for (int numCity = 1; numCity < numCities + 1; numCity++){
        string strNumCity = to_string(numCity);
        string strPath = "data/times/city-" + strNumCity + ".csv";
        ofstream outputFile(strPath, ios::out | ios::trunc);
        cout << strPath << endl;
        outputFile << "genSubway, genBusLine" << endl;

        CityGraph city = cityParser("data/city-" + strNumCity);

        // Começa a contagem do tempo da primeira operação
        auto timeStart1 = high_resolution_clock::now();

        buildSubwayGraph(city, false);

        // Encerra a contagem de tempo da primeira operação
        auto timeStop1 = high_resolution_clock::now();

        // Começa a contagem do tempo da segunda operação
        auto timeStart2 = high_resolution_clock::now();

        buildBusGraph(city, false);

        // Encerra a contagem de tempo da segunda operação
        auto timeStop2 = high_resolution_clock::now();

        auto timeDuration1 = duration_cast<nanoseconds>(timeStop1 - timeStart1) * 0.000000001;

        auto timeDuration2 = duration_cast<nanoseconds>(timeStop2 - timeStart2) * 0.000000001;
        
        outputFile << timeDuration1.count() << "," << timeDuration2.count() << endl;

        outputFile.close();
    }
}
