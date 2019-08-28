# Irrigation System
# 1. Introduction
Irrigation is the application of controlled amounts of water to plants at needed intervals. Irrigation helps to grow agricultural crops, maintain landscapes, and revegetate disturbed soils in dry areas and during periods of less than average rainfall. Irrigation also has other uses in crop production, including frost protection, suppressing weed growth in grain fields and preventing soil consolidation.
Automated irrigation system provides the solution for watering the lawn and the garden automatically even in the absence of human.
No individual presence is needed for watering as the system is automated considering one or more of the various parameters available for irrigation scheduling such as soil moisture measurement, evapotranspiration estimates, leaf water potential canopy temperature etc.

# 2. Environment
## 2.1. Hardware
- [ESP32 - System On Chip Microcontoller including WiFi and Bluetooth.](https://www.espressif.com/en/products/hardware/esp32/overview)
- [DHT11 - Temperature and Humidity sensor.](https://www.mouser.com/ds/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf)
- [BreadBoard and jumper cables.](https://www.roboter-bausatz.de/en/diy-electronics/diy-accessories/prototyping-boards/140/syb-120-700-breadboard-65-jumper-cables-700-contacts)
- LED


## 2.2. Software
- [Arduino (IDE)](https://www.arduino.cc/en/main/software)
  - WiFi.h - This library allows ESP32 to connect your router.
  - PubSubClient.h - This library allows you to send and receive MQTT messages.
  - DHT.h - This library allows DHT11, DHT21 and DHT22 sensors for measuring temperature and humidity.
- [Node.js](https://nodejs.org/en/)
- [Node-Red](https://nodered.org/)
  - [Node-Red-Dashboard](https://flows.nodered.org/node/node-red-dashboard)
    - ``` npm install node-red-dashboard ```
  - [Node-Red-MQTT](https://flows.nodered.org/node/node-red-contrib-mqtt-broker)
    - ``` npm install node-red-contrib-mqtt-broker ```
  - [Node-Red-Firebase - This library allows Node-RED to connect your Firebase database.](https://flows.nodered.org/node/node-red-contrib-firebase)
    - ``` npm install node-red-contrib-firebase ```
  - [Node-Red-Bigml - This library allows Node-RED to create Machine Learning workflows.](https://flows.nodered.org/node/bigml-nodered)
    - ``` npm install bigml-nodered ```
  - [Node-Red-Blynk-Ws](https://flows.nodered.org/node/node-red-contrib-blynk-ws)
    - ``` npm install node-red-contrib-blynk-ws ```
    
## 2.3. Database
- [Firebase](https://firebase.google.com)
  - [Firebase-Realtime-Database](https://firebase.google.com/docs/database)
  - [Cloud-Firestore](https://firebase.google.com/docs/firestore)
  
## 2.4. Android & iOS
- [Blynk](https://blynk.io)

# 3. Configuration
## 3.1. WiFi
```c
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
```

## 3.2. MQTT
```c
const char* mqtt_server = "REPLACE_WITH_YOUR_RPI_IP_ADDRESS";
```

## 3.3. DHT Pin
```c
const int DHTPin = REPLACE_WITH_YOUR_ESP32_PIN_CONNECTION;
```

## 3.4. LED Pin
```c
const int LEDPin = REPLACE_WITH_YOUR_ESP32_PIN_CONNECTION;
```

# 4. Screenshots
| MQTT Broker with Firebase | Live Mode UI |
|:-:|:-:|
| ![First](/Images/MQTT_Node_RED.JPG?raw=tru) | ![Sec](/Images/Live_Mode.JPG?raw=tru) |

| Blynk | Test Mode UI |
|:-:|:-:|
| ![Third](/Images/Blynk.jpg?raw=true) | ![Fourth](/Images/Test_Mode.JPG?raw=true) |

| Hardware | MQTT Call Back |
|:-:|:-:|
| ![Five](/Images/ESP32_DHT11.jpg?raw=true) | ![Six](/Images/MQTT_Call_Back.JPG?raw=true) |
