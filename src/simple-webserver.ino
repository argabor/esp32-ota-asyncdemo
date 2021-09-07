#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <Husarnet.h>

#if __has_include("credentials.h")
#include "credentials.h"
#else

#ifndef WIFI_SSID
#error You need to define WIFI_SSID!
#endif
#ifndef WIFI_PASS
#error You need to define WIFI_PASS!
#endif
#ifndef HUSARNET_JOINCODE
#error You need to define HUSARNET_JOINCODE!
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
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start Husarnet
  Husarnet.selfHostedSetup(dashboardURL);
  Husarnet.join(husarnetJoinCode, hostName);
  Husarnet.start();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am ESP32?");
  });

  AsyncElegantOTA.begin(&server); // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void)
{
  AsyncElegantOTA.loop();
}