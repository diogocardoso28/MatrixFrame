#include <Arduino.h>
#ifndef WIFI_H_
#define WIFI_H_

// Code for header body
class Wifi
{

public:
    Wifi();
    void connect();
    void initializeWifiAp();

private:
    int apStatus = 0;
};
#endif // WIFI_H_