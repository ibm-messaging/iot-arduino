/**
 * Helloworld style, connect an ESP8266 to the IBM IoT Foundation
 * 
 * This is a modified version of the sketch that goes with the developerWorks recipe: https://developer.ibm.com/recipes/tutorials/connect-an-esp8266-with-the-arduino-sdk-to-the-ibm-iot-foundation/
 * 
 * This sketch is modified to show how to use a secure connection between the ESP8266 and the Watson IoT Platform.
 * 
 * A complication is that presently Watson only support TLS v1.2 where as the latest release of the ESP8266 Arduino SDK, 2.3.0 only support TLS v1.1. 
 * The latest ESP8266 code does support TLS v1.2, however as its not come out in an official release yet you need to get the code directly from Github. 
 * Thats a little harder than getting the ESP8266 support with the Arduino Board Manager facility, but its still fairly straight forward and the code 
 * appears stable and for me works well. How to do it is described here: https://github.com/esp8266/Arduino#using-git-version
 * 
 * Author: Ant Elder
 * License: Apache License v2
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h> 

//-------- Customise these values -----------
const char* ssid = "<yourSSID>";
const char* password = "<yourPassword>";

#define ORG "quickstart" // "quickstart" or use your organisation
#define DEVICE_ID "Test5"      
#define DEVICE_TYPE "ESP8266" // your device type or not used for "quickstart"
#define TOKEN "xxxxxxxxxxxxx" // your device token or not used for "quickstart"
//-------- Customise the above values --------

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/status/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

WiFiClientSecure wifiClient;
PubSubClient client(server, 8883, wifiClient);

void setup() {
  Serial.begin(115200); Serial.println();

  Serial.print("Connecting to "); Serial.print(ssid);
  if (strcmp (WiFi.SSID().c_str(), ssid) != 0) {
     WiFi.begin(ssid, password);
  }
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }  
  Serial.println("");
  Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  Serial.println("View the published data on Watson at: "); 
  if (ORG == "quickstart") {
    Serial.println("https://quickstart.internetofthings.ibmcloud.com/#/device/" DEVICE_ID "/sensor/");
  } else {
    Serial.println("https://" ORG ".internetofthings.ibmcloud.com/dashboard/#/devices/browse/drilldown/" DEVICE_TYPE "/" DEVICE_ID);
  }
}

void loop() {
   if (!!!client.connected()) {
      Serial.print("Reconnecting client to "); Serial.println(server);
      while ( ! (ORG == "quickstart" ? client.connect(clientId) : client.connect(clientId, authMethod, token))) {
        Serial.print(".");
        delay(500);
     }
     Serial.println();
   }

  String payload = "{\"d\":{\"myName\":\"ESP8266.Test5\",\"counter\":";
  payload += millis() / 1000;
  payload += "}}";
  
  Serial.print("Sending payload: "); Serial.println(payload);
    
  if (client.publish(topic, (char*) payload.c_str())) {
    Serial.println("Publish ok");
  } else {
    Serial.println("Publish failed");
  }

  delay(5000);
}

