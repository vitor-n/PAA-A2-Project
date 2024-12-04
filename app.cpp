#include <map>
#include <functional>
#include <string>
#include <iostream>

typedef std::map< std::string, std::function<void(void)>> command_dict;
//                                           ^^^^^^^^
//                               the signature of your commands. probably should have an error code.

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

using namespace std;

void command1() { cout << "commanda" << endl; }
void command2() { cout << "commandb" << endl; }
void command3() { cout << "commandc" << endl; }

void welcome() {
    cout << CLEAR;
    printf ("Welcome to %sG%so%so%sg%so%sl%s-Routes!\n", BOLDGREEN, BOLDBLUE, BOLDRED, BOLDGREEN, BOLDBLUE, BOLDYELLOW, RESET);
    cout << "Making your routes 10^100 times better!" << endl;

    cout << endl;
}

void routeInput() {
    cout << "╭FROM: " << endl;
    cout << "╰TO:   " << endl;
}

void clrscr() { cout << CLEAR; }

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
    c["a"] = &command1;
    c["b"] = &command2;
    c["c"] = &command3;
    c["clear"] = &clrscr;

    welcome();
    routeInput();

    string input;
    while(getline(cin, input)) { // quit the program with ctrl-d
        auto it = c.find(input);
        if(it != end(c)) {
            (it->second)(); // execute the command
        } else {
            cout << CLEAR;
            welcome();
            cout << "Command \"" << input << "\" not known" << endl;
        }
    }
}