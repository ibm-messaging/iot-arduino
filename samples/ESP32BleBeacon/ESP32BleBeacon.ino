/*
    Example of a Bluetooth LE beacon using ESP32/Arduino SDK

    This is a BLE Periphiral Node regularly broadcasting an advertising packet and then
    deep sleeping. It could have some sensor and include the reading in the packet, for 
    now this just sends the count of wakeups. Sending the value is a bit hacky because
    the current ESP32/Arduino SimpleBLE library doesn't yet support BLE advertising  
    data so the sensor value is encoded into the device name.
    
    While sleeping the current consumption is about 5 microamps, and it takes about 
    120 milliseconds to wake up, send the advertising packet, and go back to sleep. So
    it could last for quite a while powered by batteries - over a year on AA alkalines.

    Tested with the ESP32/Arduino code from Github on 23 Feb

    Author: Anthony ELder
    License: Apache License v2
*/
#include "SimpleBLE.h"

#define SLEEP_SECS 8

SimpleBLE ble;

String beaconMsg = "ESP32xx"; // the x's get overwritten

// this variable maintained over deep sleeps
RTC_DATA_ATTR static uint16_t wakeupCount = 0;

void setup() {
  Serial.begin(115200);

  byte* bytes = (byte*)beaconMsg.c_str();
  bytes[5] = wakeupCount;
  bytes[6] = wakeupCount >> 8;

  ble.begin(beaconMsg);

  // a delay before shutdown otherwise the packets aren't sent completely
  // the delay required seems to depend on the payload length 
  delay(10); 

  ble.end();

  Serial.printf("Wakeup count=%i, awake for %i ms, deep sleeping for %i secs...\n", 
     wakeupCount++, millis(), SLEEP_SECS);  
  esp_deep_sleep(SLEEP_SECS * 1000000);
}

void loop() {
  // doesn't ever get here
}
