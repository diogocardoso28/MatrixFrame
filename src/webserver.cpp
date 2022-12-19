#include <Arduino.h>
#include <ArduinoJson.h>
#include "ESPAsyncWebServer.h"
#include "webserver.h"
#include "wifi_coms.h"
#include "settings.h"
#include "FS.h"
#include "SPIFFS.h"

extern Settings settings;

Webserver::Webserver()
{
}

void Webserver::startWebServer()
{
    wifi.connect(); // Connects to wifi

    // server.serveStatic("/", SPIFFS, "/webInterface/").setDefaultFile("index.html"); // serves Index page
    server.on("/", HTTP_ANY, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/webInterface/index.html"); });
    server.on("/js/jquery.js", HTTP_ANY, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/webInterface/js/jquery.js"); });
    // // Gets wifi prefferences
    // server.on("/get_wifi_settings", HTTP_GET, [](AsyncWebServerRequest *request)
    //           {
    //               AsyncResponseStream *response = request->beginResponseStream("application/json");
    //               serializeJson(settings.getWifiSettings(), *response); // Serializes the document returned by getWifiSettings and stores it to response to be sent back
    //               request->send(response); });

    server.begin();
}
