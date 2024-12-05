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

bool parseAdressInput(const string& input, int& region, int& street, int& number) {
    istringstream iss(input);
    int count = 0;
    if (iss >> region) count++;
    if (iss >> street) count++;
    if (iss >> number) count++;

    string remaining;
    if (count == 3 && !(iss >> remaining)) {
        return true;
    }
    return true;
}

void fromto() {
    
    int adress1[3] = {0, 3811, 1};
    int adress2[3] = {8, 641, 2};
    float info[3];
    
    findBestRoute(CITY, SUBWAY, BUS, adress1, adress2, info, 100);
}

void list() {
}

void help()  {
    printf("%sFROMTO%s: Find the best route between two adresses, format <ZIP, STREET, NUMBER>\n", BOLDGREEN, RESET);
    printf("%sLUCKY%s: Find the best route between two random adresses. Feeling lucky?\n", BOLDYELLOW, RESET);
    printf("%sLIST%s: List set of available adresses\n", BOLDGREEN, RESET);
    printf("%sCLEAR%s: Clear the screen\n", BOLDGREEN, RESET);
    printf("%sHELP%s: Show this message\n", BOLDGREEN, RESET);
}

void commands() {
    cout << endl;
    printf ("Commands: %sFROMTO%s | %sLUCKY%s | %sLIST%s | %sCLEAR%s | %sHELP%s\n", BOLDGREEN, RESET, BOLDYELLOW, RESET, BOLDGREEN, RESET, BOLDGREEN, RESET, BOLDGREEN, RESET);
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

void printInfo(int time, float cost, int distance) {
    int h = time/3600;
    int m = (time%3600)/60;
    int s = time%60;

    
    printf("\t%s%02d:%02d:%02d%s | ", YELLOW, h, m, s, RESET);
    printf("%s$%.2f%s | ", MAGENTA, cost, RESET);
    printf("%s%dm%s\n", CYAN, distance, RESET);
}

int main() {
    command_dict c;
    c["FROMTO"] = &fromto;
    c["LIST"] = &list;
    c["LUCKY"] = &list;
    c["HELP"] = &help;
    c["CLEAR"] = &clrscr;

    cout << CLEAR;
    welcome();
    commands();

    string input;
    while(getline(cin, input)) { 
        cout << endl;
        transform(input.begin(), input.end(), input.begin(), ::toupper);
        auto it = c.find(input);
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