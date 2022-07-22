#include <Arduino.h>
#include <ArduinoJson.h>
#include <string.h>
#ifndef SETTINGS_H_
#define SETTINGS_H_

// Code for header body

class Settings
{

public:
    Settings();
    boolean getWifiApMode();
    boolean getWifiAutoApMode();
    String getWifiSsid();
    String getWifiPassword();
    String getApSSID();
    String getApPassword();
    DynamicJsonDocument getWifiSettings();

private:
    DynamicJsonDocument wifiConfigurations = DynamicJsonDocument(500);
};

#endif // SETTINGS_H_