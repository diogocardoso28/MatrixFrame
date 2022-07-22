#include <Arduino.h>
#include <ArduinoJson.h>
#include "ESPAsyncWebServer.h"
#include "wifi.h"

#ifndef WEBSERVER_H_
#define WEBSERVER_H_

// Code for header body
class Webserver
{
public:
    Webserver();
    void startWebServer();

private:
    AsyncWebServer server = AsyncWebServer(80);
    AsyncEventSource events = AsyncEventSource("/events"); // event source (Server-Sent events)
    Wifi wifi = Wifi();
};
#endif // WEBSERVER_H_