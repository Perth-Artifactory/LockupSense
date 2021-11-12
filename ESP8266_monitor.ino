/**
 * ESP8266 Online Status
 * by Blake Samuels aka Tazard
 * 
 * Periodically call a URL to show that this device is 
 * powered and can reach the internet. Can be used as
 * part of internet connectivity monitor, wifi monitor,
 * or to show when some piece of Artifactory equipment
 * is inadvertently left on.
 * 
 * Its a minimal modification of BasicHTTPClient.ino 
 * example for ESP8266 and will call a HTTPS URL at 
 * given interval (e.g. statuscake, healthchecks.io).
 * 
 * Major Update, Date 2021-10-27
*/

// Set WIFI details
const char* const SSID = "SSID";
const char* const PASSWORD = "PASSWORD";

// URL to periodically GET (e.g. statuscake push)
const char* const URL = "https://push.statuscake.com/?...";

// Time between calling URL (milliseconds)
unsigned long DELAY = 10000;

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>

ESP8266WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println("ESP8266 Online Status Push");
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(SSID, PASSWORD);
  
  // enable onboard LED for status notifications
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    // Ignore the SSL certificate, we are just pushing status updates so it doesn't matter.
    client->setInsecure();

    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, URL)) {  // HTTPS

      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          Serial.println(payload);
          
          // we pinged successfully, status LED on
          digitalWrite(LED_BUILTIN, LOW); 
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        
        // we aren't pinging correctly, status LED off
        digitalWrite(LED_BUILTIN, HIGH);
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
      // we aren't pinging correctly, status LED off
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }

  Serial.println("Wait 10s before next round...");
  delay(DELAY);
}
