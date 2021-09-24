#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <Husarnet.h>

#define BUFFER_SIZE 2000
#define HTTP_PORT 3232

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

AsyncWebServer server(HTTP_PORT);

void setup(void)
{
  // ===============================================
  // Wi-Fi, OTA and Husarnet VPN configuration
  // ===============================================

  Serial.begin(115200,SERIAL_8N1, 16, 17); // remap default Serial from P3 & P1 to P16 & P17
  Serial1.begin(115200,SERIAL_8N1, 3, 1); // remap Serial1 from P9 & P10 to P3 & P1

  Serial1.println("GitHub Actions OTA example");
  Serial1.printf("📻 Connecting to: %s Wi-Fi network ", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Waiting for Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED) {
    static int cnt = 0;
    delay(500);
    Serial1.print(".");
    cnt++;
    if(cnt > 10) {
      ESP.restart();
    }
  }
  Serial1.println(" done");

  // Start Husarnet P2P VPN service
  Husarnet.selfHostedSetup(dashboardURL);
  Husarnet.join(husarnetJoinCode, hostName);
  Husarnet.start();

  // POST request API for ESP32 remote reset
  server.on("/reset", HTTP_POST, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Reseting ESP32 after 1s ...");
    Serial1.println("Software reset on POST request");
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

  Serial1.println("⌛ Waiting for Husarnet to be ready");
  delay(15000); // TODO: check connection status instead

  Serial1.println("🚀 HTTP server started");
  Serial1.printf("Visit:\r\nhttp://%s:%d/\r\n", hostName, HTTP_PORT);
}

void loop(void)
{
  ;
}