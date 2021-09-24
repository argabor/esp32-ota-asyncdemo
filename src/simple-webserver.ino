#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <Husarnet.h>

#define BUFFER_SIZE 2000

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
  // ===============================================
  // Wi-Fi, OTA and Husarnet VPN configuration
  // ===============================================

  Serial.begin(115200);
  Serial.printf("Connecting to: %s Wi-Fi network", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Waitinf for Wi-Fi connection
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

  // Start Husarnet P2P VPN service
  Husarnet.selfHostedSetup(dashboardURL);
  Husarnet.join(husarnetJoinCode, hostName);
  Husarnet.start();

  // POST request API for ESP32 remote reset
  server.on("/reset", HTTP_POST, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Reseting ESP32 after 1s ...");
    Serial.println("Software reset on POST request");
    delay(1000);
    ESP.restart();
  });

  // OTA webserver: /update path
  AsyncElegantOTA.begin(&server); // Start ElegantOTA
  server.begin();
  
  // ===============================================
  // PLACE YOUR APPLICATION CODE BELOW
  // ===============================================

  // Example webserver hosting table with known Husarnet Hosts
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    char buffer[BUFFER_SIZE];
    int i = 0;
    i += sprintf(buffer+i, "Known hosts: </br></br>");
    i += sprintf(buffer+i, "<table border=\"1\"><tr><th>Hostname</th><th>IPv6</th></tr>");
    for (auto const &host : Husarnet.listPeers()) {
      i += sprintf(buffer+i, "<tr><td>%s</td><td>%s</td></tr>", host.second.c_str(), host.first.toString().c_str());
      if(BUFFER_SIZE - i < 100) {
        i += sprintf(buffer+i, "<td>Increase the BUFFER_SIZE</td>");
      }
    }
    i += sprintf(buffer+i, "</table>");
    request->send(200, "text/html", String(buffer));
  });

  Serial.println("HTTP server started");
}

void loop(void)
{
  ;
}