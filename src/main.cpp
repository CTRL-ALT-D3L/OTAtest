
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>
#include "cert.h"
#include "other.h"

#define SSID "Berm"
#define PASS "0737373783"
#define VERSION 001
#define VERSION_URL "https://raw.githubusercontent.com/CTRL-ALT-D3L/OTAtest/main/version.txt"
#define FIRMWARE_URL "https://raw.githubusercontent.com/CTRL-ALT-D3L/OTAtest/main/firmware.bin"

void connectWifi();
void firmwareUpdate();
int checkFirmwareVersion();

//************************************************************************************************//

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.print("Active firmware version:");
  Serial.println(VERSION);
  connectWifi();
}

void loop()
{
  int v = checkFirmwareVersion();
  Serial.print("\nCurrent firmware version: ");
  Serial.println(VERSION);
  Serial.print("Github firmware version: ");
  Serial.println(v);
  if (v > VERSION)
  {
    Serial.println("Firmware update Starting..");
    firmwareUpdate();
  }
  delay(10000);
}

//************************************************************************************************//

void connectWifi()
{
  Serial.print("Connecting to WiFi.");
  WiFi.begin(SSID, PASS);
  for (uint8_t i = 0; i < 50; i++)
  {
    if (WiFi.isConnected())
      break;
    Serial.print('.');
    delay(300);
  }
  Serial.println();

  if (WiFi.isConnected())
  {
    Serial.print("Connected on IP: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("Could not connect.");
    while (true)
      ;
  }
}

void firmwareUpdate()
{
  WiFiClientSecure client;
  client.setCACert(rootCACertificate);
  t_httpUpdate_return ret = httpUpdate.update(client, FIRMWARE_URL);

  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
    break;

  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("HTTP_UPDATE_NO_UPDATES");
    break;

  case HTTP_UPDATE_OK:
    Serial.println("HTTP_UPDATE_OK");
    break;
  }
}

int checkFirmwareVersion()
{
  String payload;
  int httpCode = HTTP_CODE_BAD_REQUEST;
  String fwurl = "";
  fwurl += VERSION_URL;
  fwurl += "?";
  fwurl += String(rand());
  Serial.println(fwurl);
  WiFiClientSecure *client = new WiFiClientSecure;

  if (client)
  {
    client->setCACert(rootCACertificate);

    // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
    HTTPClient https;

    if (https.begin(*client, fwurl))
    { // HTTPS
      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      delay(100);
      httpCode = https.GET();
      delay(100);
      if (httpCode == HTTP_CODE_OK) // if version received
      {
        payload = https.getString(); // save received version
      }
      else
      {
        Serial.print("error in downloading version file:");
        Serial.println(httpCode);
      }
      https.end();
    }
    delete client;
  }

  if (httpCode == HTTP_CODE_OK) // if version received
  {
    payload.trim();
    int v = charArray::toInt(payload.c_str());
    Serial.println(v);
    if (v <= VERSION)
    {
      Serial.print("Device already on latest firmware version: ");
      Serial.println(VERSION);
    }
    else
    {
      Serial.println(payload);
      Serial.println("New firmware detected");
    }
    return v;
  }
  else
  {
    Serial.println("HTTP did not connexfrsadas");
    Serial.println(httpCode);
  }
  return 0;
}

//