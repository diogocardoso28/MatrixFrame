#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>
#include "ESPAsyncWebServer.h"
#include "filesystem.h"
#include "settings.h"
#include "wifi.h"

extern Settings settings;

Wifi::Wifi()
{
    if (!Serial)
        Serial.begin(9600);
}

void Wifi::connect()
{
    Serial.print("Apmode: ");
    Serial.println(settings.getWifiApMode());
    if (settings.getWifiApMode() == false)
    {
        Serial.print("Attempting to Connect to Wifi network: ");
        Serial.println(settings.getWifiSsid());

        // Attenpts wifi connection
        WiFi.begin(settings.getWifiSsid().c_str(), settings.getWifiPassword().c_str());

        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.print(".");
            delay(500); // For some reason delay(was not working)
            if (millis() > 20000)
            {
                WiFi.disconnect();
                Serial.println(" ");
                if (settings.getWifiAutoApMode())
                {
                    Serial.println("Error while connecting to wifi, initializing Wifi Hotspot");
                    initializeWifiAp();
                    // startWebServer();
                    return;
                }
                else
                {
                    Serial.println("Error while connecting to wifi!");
                    return;
                }
            }
        }
        Serial.println("");
        Serial.println("Wifi Connected Successfully!");
        Serial.print("Ip address: ");
        Serial.println(WiFi.localIP());
        Serial.println(WiFi.RSSI());
        Serial.println("");
        WiFi.setHostname("Frame"); // define hostname
        // Define mdns
        if (!MDNS.begin("Frame"))
        {
            Serial.println("Error starting mDNS");
            return;
        }

        // startWebServer();
    }
    else
    {
        Serial.println("Starting AP mode!");
        initializeWifiAp();
        // startWebServer();
    }
}

void Wifi::initializeWifiAp()
{
    apStatus = 1; // Instanciates that AP was enabled

    IPAddress local_IP(1, 2, 3, 4);
    IPAddress gateway(1, 1, 1, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(local_IP, gateway, subnet);

    WiFi.softAP(settings.getApSSID().c_str(), settings.getApPassword().c_str());
    Serial.println(WiFi.softAPIP());
    WiFi.setHostname("rfid"); // define hostname
    // Define mdns
    if (!MDNS.begin("rfid"))
    {
        Serial.println("Error starting mDNS");
        return;
    }
}
