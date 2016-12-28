/**
 * Helloworld style, connect an ESP8266 to the IBM IoT Foundation
 * 
 * This is a modified version of the sketch that goes with the developerWorks recipe: https://developer.ibm.com/recipes/tutorials/use-http-to-send-data-to-the-ibm-iot-foundation-from-an-esp8266/
 * 
 * This sketch is modified to show how to use a secure https connection between the ESP8266 and the Watson IoT Platform.
 * 
 * A complication is that presently Watson only support TLS v1.2 where as the latest release of the ESP8266 Arduino SDK, 2.3.0 only support TLS v1.1. 
 * The latest ESP8266 code does support TLS v1.2, however as its not come out in an official release yet you need to get the code directly from Github. 
 * Thats a little harder than getting the ESP8266 support with the Arduino Board Manager facility, but its still fairly straight forward and the code 
 * appears stable and for me works well. How to do it is described here: https://github.com/esp8266/Arduino#using-git-version
 * 
 * An additional complication is that even with that latest code the ESP8266 HTTPClient function appears to still not yet support TLS v1.2, so you need 
 * to make the HTTP calls manually using WiFiClientSecure. I've raised a bug about this: https://github.com/esp8266/Arduino/issues/2783
 * 
 * Author: Ant Elder
 * License: Apache License v2
 */
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <base64.h>

//-------- Customise these values -----------
const char* ssid = "<yourSSID>";
const char* password = "<yourPassword>";

#define ORG "quickstart" // your organization or "quickstart"
#define TOKEN "xxxxxxxxxxxxxxx" // your registered device token or not used with "quickstart"
#define DEVICE_TYPE "ESP8266"  // not used with quickstart or customize to your registered device type
#define DEVICE_ID "TestSecure1" // use anything for quickstart or customize to your registered device id
#define EVENT "myEvent" // use this default or customize to your event type
//-------- Customise the above values --------

String urlPath = "/api/v0002/device/types/" DEVICE_TYPE "/devices/" DEVICE_ID "/events/" EVENT;
String urlHost = ORG ".messaging.internetofthings.ibmcloud.com";
int urlPort = 8883;
String authHeader;

void setup() {
  Serial.begin(115200); Serial.println(); 

  initWifi();

  Serial.println("View the published data on Watson at: "); 
  if (ORG == "quickstart") {
    Serial.println("https://quickstart.internetofthings.ibmcloud.com/#/device/" DEVICE_ID "/sensor/");
  } else {
    Serial.println("https://" ORG ".internetofthings.ibmcloud.com/dashboard/#/devices/browse/drilldown/" DEVICE_TYPE "/" DEVICE_ID);
  }  
  
  if (ORG == "quickstart") {
    authHeader = "";
  } else {
    authHeader = "Authorization: Basic " + base64::encode("use-token-auth:" TOKEN) + "\r\n";
  }  
}

void loop() {
  doWiFiClientSecure();
  delay(10000);
}

void doWiFiClientSecure() {
  WiFiClientSecure client;

  Serial.print("connect: "); Serial.println(urlHost);
  while ( ! client.connect(urlHost.c_str(), urlPort)) {
    Serial.print(".");
  }
  Serial.println("Connected");

  String postData = String("{  \"d\": {\"aMessage\": \"") + millis()/1000 + "\"}  }";

  String msg = "POST " + urlPath + " HTTP/1.1\r\n"
                "Host: " + urlHost + "\r\n"
                "" + authHeader + ""
                "Content-Type: application/json\r\n"
                "Content-Length: " + postData.length() + "\r\n"
                "\r\n" + postData;

  client.print(msg);
  Serial.print(msg);

  Serial.print("\n*** Request sent, receiving response...");
  while (!!!client.available()) {
    delay(50);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("Got response");  
  while(client.available()){
    Serial.write(client.read());
  }
  
  Serial.println(); Serial.println("closing connection");
  client.stop();
}

void initWifi() {
  Serial.print("Connecting to: "); Serial.print(WiFi.SSID());
  WiFi.mode(WIFI_STA);   
  WiFi.begin(ssid, password);  

  while (WiFi.status() != WL_CONNECTED) {
     delay(250);
     Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());
}
