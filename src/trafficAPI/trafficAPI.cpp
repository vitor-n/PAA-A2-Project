#include <iostream>
#include <chrono>
#include <ctime>

#include "trafficAPI.h"

using namespace std;

/**
 * @brief Retrieves the current hour from the system.
 * 
 * @return int The current hour (0-23).
 */
int getTime()
{
    time_t now = time(0);
    tm* localTime = localtime(&now);
    return localTime->tm_hour;
}

int getMinutes()
{
    time_t now = time(0);
    tm* localTime = localtime(&now);
    return localTime->tm_min;
}



/**
 * @brief Detemines a penalty factor based on the current hour.
 * 
 * @param time The current hour (0-23).
 * @return int A penalty factor associated with the given hour.
 */
int timeDivisor(int time)
{
    int penalty[24] = {3, 2, 1, 1, 2, 2, 4, 5, 5, 7, 5, 5, 8, 8, 7, 7, 10, 9, 10, 8, 7, 6, 4, 4};
    return penalty[time];
}

/**
 * @brief Determines the adjusted speed for a street based on its ID, time-based traffic penalty and the street's maximum allowed speed.
 * 
 * @param street The street ID
 * @param region The region ID
 * @param maxSpeed The maximun allowed speed for the street
 * @return double The djusted speed considering traffic and time.
 *
 * @details
 * - The calculation takes into account:
 * - The street's inherent traffic level (derived from its ID).
 * - The region's modifier traffic level (derived form its ID).
 * - The penalty associated with the current hour of the day.
 * - A reduction factor that modifies the maximum speed based on these variables.
 */
double detTraffic(int street, int region, int maxSpeed)
{
    double howBusy = street % 12;
    double regionPenalty = 0.2 * (region % 3) + 0.8;

    double now = getTime();
    double timePenalty = timeDivisor(now);

    double reductionFactor = (howBusy/12) * (timePenalty/10);
    double result = regionPenalty * (maxSpeed * (1 - reductionFactor));

    return result;
}

int secondsToSubway(int v)
{
    int nowMin = getMinutes();
    
    if (nowMin % 10 == 0) 
        return 0;
    else 
        return (10 - nowMin % 10) * 60; 
}