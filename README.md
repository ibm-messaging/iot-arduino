Arduino Uno Sample
====================

This repository contains the quickstart and registered device sample, and contains samples for connecting Arduino Uno devices to the IBM Internet of Things Foundation

The events that are emitted in this sample are:

+ Internal sensor temperature 


Pre-requisite setup for the 2 flows
============================
1. Connect Ethernet / Wifi shield to Arduino Uno
2. Connect Ethernet cable to the Ethernet / Wifi shield 
3. Install sketch on desktop / laptop
4. Download Paho mqtt library, for Ardunio, from this link (https://projects.eclipse.org/projects/technology.paho/downloads) in the sketch  
	a) To load it into the Arduino IDE, the zip file should be downloaded intact, without unzipping.  
	b) Then in the IDE: Select Sketch -> Import Library -> Add Library  and select the zip file  

5. Connect the USB cable to the Arduino Uno and other end to desktop / laptop which has the sketch installed on it
6. The samples folder of this repository (https://github.com/ibm-messaging/iot-arduino) contains 2 folders, each containining 1 flow - 
	a) Quickstart flow
	b) Registered flow
7. Compile the 2 skectch codes (corresponding to the flows)
8. Depending upon the requirement, push one of the flows to the Arduino device
9. Reset the Ethernet / Wifi shield
10. Wait for about 10 seconds and the flow starts working


Quickstart flow
=======================
1. Modify the clientId, in the sketch code, by providing the values in the following format "d:quickstart:iotsample-arduino:aabbccde02", by replacing "aabbccde02" with the MAC Address of the device
2. Open the quickstart dashboard (http://quickstart.internetofthings.ibmcloud.com/#/) 
3. Provide the MAC Address (in case of the example, its aabbccde02) in the textbox "Ready to View data?"


Registered Flow
===============================
This has 2 flows
1) Registered Flow from device
2) Registered Flow to device

Registered Flow from device and to device
===========================================
1. Modify the AUTHTOKEN in the sketch code
2. Modify the MS_PROXY, in the sketch code, by providing the values in the following format "uguhsp.messaging.internetofthings.ibmcloud.com", by replacing "uguhsp" with your organization
3. Modify the CLIENT_ID, in the sketch code, by providing the values in the following format "d:uguhsp:iotsample-arduino:00aabbccde03", by replacing "aabbccde03" with the MAC Address and "uguhsp" with the organization
4. Use mqttpublisher / mqttsubscriber to publish and subscribe the commands / events sent to / received from the Arduino Uno
5. Modify the mac Address (given in the sample as { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x03 } ) by the MAC Address of the Ethernet shield


Development
===============================
+ The code contains comments, which explains how to modify the parameters
+ In case of registered flow, you will have to connect to https://internetofthings.ibmcloud.com/dashboard/#/ and create the following
+ Organization
+ Device
+ Auth tokens
