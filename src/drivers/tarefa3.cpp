#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <functional>
#include <chrono>
#include <ctime> 

#include "graph.h"
#include "genSubway.h"
#include "genBus.h"
#include "cityParser.h"
#include "findRoute.h"

typedef std::map< std::string, std::function<void(void)>> command_dict;

#define CLEAR "\033[2J\033[1;1H"
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


CityGraph CITY = cityParser("data/city-1");
Graph SUBWAY = buildSubwayGraph(CITY, false);
Graph BUS = buildBusGraph(CITY, false);

using namespace std;

void printInfo(float info[]) {
    int time = info[0];
    float cost = info[1];
    //int distance = info[2];

    int h = time/3600;
    int m = (time%3600)/60;
    int s = time%60;

    printf("\t%s%02d:%02d:%02d%s | ", YELLOW, h, m, s, RESET);
    printf("%s$%.2f%s | ", MAGENTA, cost, RESET);
    //printf("%s%dm%s\n", CYAN, distance, RESET);
}

void quit() {
    cout << "Bye!" << endl;
    exit(0);
}

void help()  {
    printf("%sLUCKY%s: Find the best route between two random adresses < ZIP STREET N >. Feeling lucky?\n", BOLDYELLOW, RESET);
    printf("%sCLEAR%s: Clear the screen\n", BOLDGREEN, RESET);
    printf("%sQUIT%s: Quit\n", BOLDGREEN, RESET);
    printf("%sHELP%s: Show this message\n", BOLDGREEN, RESET);
}

void commands() {
    cout << endl;
    printf ("Commands:  %sLUCKY%s | %sCLEAR%s | %sQUIT%s | %sHELP%s\n", BOLDYELLOW, RESET, BOLDGREEN, RESET, BOLDGREEN, RESET, BOLDGREEN, RESET);
    cout << "> ";
}

void welcome() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    printf ("Welcome to %sG%so%so%sg%so%sl%s-Routes!\n", BOLDGREEN, BOLDBLUE, BOLDRED, BOLDGREEN, BOLDBLUE, BOLDYELLOW, RESET);
    cout << "Making your routes 10^100 times better!" << endl;
    cout << "System time: " << localTime->tm_hour << ":" << localTime->tm_min << endl;
}

void clrscr() { 
    cout << CLEAR;
    welcome();    
}

void getRandomAdress(int adress[]) {
    int id = rand() % (CITY.numNodes() / 2) + 1;
    
    adress[0] = CITY.m_edges(id)->region;
    adress[1] = CITY.m_edges(id)->street;
    adress[2] = rand() % CITY.m_edges(id)->nBuildings;
}

void lucky() {
    
    int adress1[3];
    int adress2[3];
    float info[3] = {0, 0, 0};
    float maxCost = 0;

    printf("%sMAXCOST:%s ", BOLDCYAN, RESET);
    scanf("%f", &maxCost);

    getRandomAdress(adress1);
    getRandomAdress(adress2);

    printf("%sFROM:%s Adress 1: < ", BOLDCYAN, RESET);
    for (int i = 0; i < 3; i++) { cout << adress1[i] << " "; }
    cout << ">" << endl;
    printf("%sTO:%s   Adress 2: < ", BOLDCYAN, RESET);
    for (int i = 0; i < 3; i++) { cout << adress2[i] << " "; }
    cout << ">" << endl;
    cout << endl;

    findBestRoute(CITY, SUBWAY, BUS, adress1, adress2, info, maxCost);

    printInfo(info);
    cout << endl;
}

void dunder() {
    return;
}

int main() {
    command_dict c;
    c["LUCKY"] = &lucky;
    c["lucky"] = &lucky;
    c["HELP"] = &help;
    c["help"] = &help;
    c["QUIT"] = &quit;
    c["quit"] = &quit;
    c["CLEAR"] = &clrscr;
    c["clear"] = &clrscr;

    cout << CLEAR;
    welcome();
    commands();

    string input;
    while(getline(cin, input)) { 
        auto it = c.find(input);
        if (input == "") continue;
        cout << endl;
        if(it != end(c)) {
            (it->second)(); // execute the command
            commands();
        } else {
            cout << CLEAR;
            welcome();
            commands();
            cout << "Command \"" << input << "\" not known" << endl;
            cout << "> ";
        }
    }
}