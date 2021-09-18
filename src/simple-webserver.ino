#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <Husarnet.h>

#if __has_include("credentials.h")
#include "credentials.h"
#else

#if !defined(WIFI_SSID) && !defined(WIFI_PASS)
#error You need to define WIFI credentials - do it in platform.ini!
#endif
#ifndef HUSARNET_JOINCODE
#error You need to define HUSARNET_JOINCODE - do it in platform.ini!
#endif

// WiFi credentials
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASS;

// Husarnet credentials
const char *hostName = "ota-test";
const char *husarnetJoinCode = HUSARNET_JOINCODE; // find at app.husarnet.com
const char *dashboardURL = "default";

#endif

AsyncWebServer server(3232);

void setup(void)
{
  Serial.begin(115200);
  Serial.printf("Connecting to: %s Wi-Fi network", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    static int cnt = 0;
    delay(500);
    Serial.print(".");
    cnt++;
    if(cnt > 10) {
      ESP.restart();
    }
  }
  Serial.printf("\r\nConnected to %s\r\n", ssid);
  Serial.print("IP address in LAN: ");
  Serial.println(WiFi.localIP());
  Serial.printf("Husarnet VPN hostname: %s", hostName);

  // Start Husarnet
  Husarnet.selfHostedSetup(dashboardURL);
  Husarnet.join(husarnetJoinCode, hostName);
  Husarnet.start();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am ESP32!!!");
  });

  server.on("/reset", HTTP_POST, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Reseting ESP32 after 1s ...");
    Serial.println("Software reset on POST request");
    delay(1000);
    ESP.restart();
  });

  AsyncElegantOTA.begin(&server); // Start ElegantOTA
  server.begin();
  
  Serial.println("HTTP server started");
}

void loop(void)
{
  ;
}