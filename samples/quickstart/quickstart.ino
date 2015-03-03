#include <SPI.h>
#include <Ethernet.h>
#include <IPStack.h>
#include <Countdown.h>
#include <MQTTClient.h>

#define MS_PROXY "quickstart.messaging.internetofthings.ibmcloud.com"
#define MQTT_PORT 1883
#define MQTT_MAX_PACKET_SIZE 100

// Update these with values suitable for your network.
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };

//The convention to be followed is d:quickstart:iotsample-arduino:<MAC Address>
#define MQTT_CLIENTID "d:quickstart:iotsample-arduino:00aabbccde02"

#define MQTT_TOPIC "iot-2/evt/status/fmt/json"
  
  
EthernetClient c;  
IPStack ipstack(c);  
MQTT::Client<IPStack, Countdown, MQTT_MAX_PACKET_SIZE> client(ipstack);

String deviceEvent;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac);
  delay(2000);
}

void loop() {
  int rc = -1;
  if (!client.isConnected()) {
    Serial.println("Connecting to IoT Foundation for publishing Temperature");
    while (rc != 0) {
      rc = ipstack.connect(MS_PROXY, MQTT_PORT);
    }
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = MQTT_CLIENTID;
    rc = -1;
    while ((rc = client.connect(data)) != 0)
      ;
    Serial.println("Connected successfully\n");
    Serial.println("Temperature(in C)\tDevice Event (JSON)");
    Serial.println("____________________________________________________________________________");
  }

  MQTT::Message message;
  message.qos = MQTT::QOS0; 
  message.retained = false;

  deviceEvent = String("{\"d\":{\"myName\":\"Arduino Uno\",\"temperature\":");
  char buffer[60];
  dtostrf(getTemp(),1,2, buffer);  
  deviceEvent += buffer;
  deviceEvent += "}}";
  Serial.print("\t");
  Serial.print(buffer);
  Serial.print("\t\t");

  deviceEvent.toCharArray(buffer, 60);
  Serial.println(buffer);
  message.payload = buffer; 
  message.payloadlen = strlen(buffer);
  
  rc = client.publish(MQTT_TOPIC, message);
  if (rc != 0) {
    Serial.print("return code from publish was ");
    Serial.println(rc);
  }
  client.yield(1000);
}

/*
This function is reproduced as is from Arduino site => http://playground.arduino.cc/Main/InternalTemperatureSensor
*/
double getTemp(void) {
  unsigned int wADC;
  double t;
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN); // enable the ADC
  delay(20); // wait for voltages to become stable.
  ADCSRA |= _BV(ADSC); // Start the ADC
  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));
  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;
  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;
  // The returned temperature is in degrees Celcius.
  return (t);
}
