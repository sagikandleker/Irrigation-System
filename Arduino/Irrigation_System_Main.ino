#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <Wire.h> // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: #include "SSD1306.h"

SSD1306Wire display(0x3c, 5,4);

// DHT sensor type we're using, for other DHT sensor you can use the other defines
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

//*****WiFi connection variables*****
// Change the credentials below, so your ESP32 connects to your router
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

//*****MQTT connection variable*****
//Change the variable to your IP address, so it connects to your MQTT broker
//you should connect your computer and ESP32 to same network
const char* mqtt_server = "REPLACE_WITH_YOUR_RPI_IP_ADDRESS";

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;
PubSubClient client(espClient);

// DHT Sensor - GPIO 12 = D1 connect you DHT Pin to pin #12 on board
const int DHTPin = REPLACE_WITH_YOUR_ESP32_PIN_CONNECTION;

// Lamp - LED - GPIO 15 = D2 connect you Lamp Pin to pin #15 on board
const int LEDPin = REPLACE_WITH_YOUR_ESP32_PIN_CONNECTION;

// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;

// Don't change the function below. This functions connects your ESP32 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions is executed when some device publishes a message to a topic that your ESP32 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP32 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  //Serial.print("Message arrived on topic: ");
  //Serial.print(topic);
  //Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    //Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  //Serial.println();

  // If a message is received on the topic flower/LEDPin, you check if the message is either on or off. 
  //Turns the LEDPin GPIO according to the message
  if(topic=="flower/lamp"){
      //Serial.print("Changing LEDPin to ");
      if(messageTemp == "1"){
        static char ansOn[7];
        dtostrf(1, 6, 2, ansOn);
        client.publish("led",ansOn);
        digitalWrite(LEDPin, HIGH);
        //Serial.print("On");
      }
      else if(messageTemp == "0"){
        static char ansOff[7];
        dtostrf(0, 6, 2, ansOff);
        client.publish("led",ansOff);
        digitalWrite(LEDPin, LOW);
        //Serial.print("Off");
      }
  }
  //Serial.println();
}

  // This functions reconnects your ESP32 to your MQTT broker
  // Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    /*
     YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a new name to the ESP32.
     Here's how it looks:
       if (client.connect("ESP32Client")) {
     You can do it like this:
       if (client.connect("ESP1_Office")) {
     Then, for the other ESP:
       if (client.connect("ESP2_Garage")) {
      That should solve your MQTT multiple connections problem
    */
    if (client.connect("ESP32Client")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("flower/LEDPin");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {
  pinMode(LEDPin, OUTPUT);
  // Initializing the UI will init the display too.
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  dht.begin();
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP32Client");

  now = millis();
  // Publishes new temperature and humidity every 30 seconds (30000ms)
  if (now - lastMeasure > 30000) {
    lastMeasure = now;
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);
    
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Computes temperature values in Celsius
    float hic = dht.computeHeatIndex(t, h, false);
    
    // Uncomment to compute temperature values in Fahrenheit 
    // float hif = dht.computeHeatIndex(f, h);
     static char temperatureTemp[7];
     dtostrf(hic, 6, 2, temperatureTemp);
    
    static char humidityTemp[7];
    dtostrf(h, 6, 2, humidityTemp);
    
    // Publishes Temperature and Humidity values
    client.publish("temperature", temperatureTemp);
    client.publish("humidity", humidityTemp);
    
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawStringMaxWidth(0, 0, 128, "Temp: " + String(temperatureTemp));
    display.drawStringMaxWidth(0, 30, 128, "Humidity: " + String(humidityTemp));
    display.display();
    
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print(f);
    Serial.print(" *F\t Heat index: ");
    Serial.print(hic);
    Serial.println(" *C ");
  }
} 
