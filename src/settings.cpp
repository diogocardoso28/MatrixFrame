#include "settings.h"
#include "filesystem.h"
Settings::Settings()
{
    if (!Serial)
        Serial.begin(9600);
    // Reads Wifi settings
    File wifiFile = Filesystem::openFile("/settings/wifi.json");
    deserializeJson(wifiConfigurations, wifiFile);
    Filesystem::closeFile(wifiFile);
}

boolean Settings::getWifiApMode()
{
    return wifiConfigurations["apMode"].as<boolean>();
}

boolean Settings::getWifiAutoApMode()
{
    return wifiConfigurations["autoAp"].as<boolean>();
}

String Settings::getWifiSsid()
{
    return wifiConfigurations["ssid"].as<String>();
}

String Settings::getWifiPassword()
{
    return wifiConfigurations["password"].as<String>();
}

String Settings::getApSSID()
{
    return wifiConfigurations["apSsid"].as<String>();
}

String Settings::getApPassword()
{
    return wifiConfigurations["apPassword"].as<String>();
}
