
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
#define VERSION_URL "https://raw.githubusercontent.com/CTRL-ALT-D3L/OTAtest/main/meta/version.txt"
#define FIRMWARE_URL "https://raw.githubusercontent.com/CTRL-ALT-D3L/OTAtest/main/meta/firmware.bin"

void connectWifi();
void firmwareUpdate();
int getGithubFirmwareVersion();

//************************************************************************************************//

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  connectWifi();

  Serial.print("\nCurrent firmware version: ");
  Serial.println(VERSION);

  int v = getGithubFirmwareVersion();
  Serial.print("Github firmware version: ");
  Serial.println(v);
  if (v > VERSION)
  {
    Serial.println("Firmware update starting...");
    //firmwareUpdate();
  }
}

void loop()
{

  delay(10000);
  //Serial.println(":LKSDFJ:SLDKFJ:SDFJ:LFSJ");
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

int getGithubFirmwareVersion()
{
  int out = -1;
  WiFiClientSecure client;
  client.setCACert(rootCACertificate);
  { // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure client is
    HTTPClient https;
    if (https.begin(client, VERSION_URL))
    {
      Serial.print("[HTTPS] GET...\n");
      int httpCode = https.GET();
      if (httpCode == HTTP_CODE_OK)
        out = https.getString().toInt();
      else
      {
        Serial.print("error in downloading version file:");
        Serial.println(httpCode);
      }
      https.end();
    }
  }
  return out;
}

//