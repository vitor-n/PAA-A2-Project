#ifndef TRAFFICAPI_H
#define TRAFFICAPI_H

int getTime();
int getMinutes();
double detTraffic(int street, int region, int maxSpeed);
int timeDivisor(int time);
int secondsToSubway();

#endif