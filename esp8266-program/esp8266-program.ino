#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Test

// DHT
#define DHTPIN 2  // GPIO2 (D4 on NodeMCU)
#define DHTTYPE DHT11
float t = 0.0;
float h = 0.0;

DHT dht(DHTPIN, DHTTYPE);

// PubSub
WiFiClient espClient;
PubSubClient mqtt_client(espClient);

// Replace with your network credentials
const char* ssid = "P3DN";
const char* password = "p3dnsemarang";

// Define LED pins using GPIO numbers
// GPIO12 = D6, GPIO13 = D7, GPIO15 = D8 on NodeMCU
const int LED1_PIN = 12;  // GPIO12 (D6 on NodeMCU)
const int LED2_PIN = 13;  // GPIO13 (D7 on NodeMCU)
const int LED3_PIN = 15;  // GPIO15 (D8 on NodeMCU)
const int LED_PIN = 16;   // GPIO16 (D0 on NodeMCU) - Original blue LED

// MQTT Broker settings
const char *mqtt_broker = "mqtt.revolusi-it.com";
const char *mqtt_sub = "iot/G.231.22.0154/sub";
const char *mqtt_pub = "iot/G.231.22.0154";
const char *mqtt_username = "usm";
const char *mqtt_password = "usmjaya1";
const int mqtt_port = 1883;

void connectToWiFi();
void connectToMQTTBroker();
void mqttCallback(char *topic, byte *payload, unsigned int length);

void connectToWiFi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
}

void connectToMQTTBroker() {
    while (!mqtt_client.connected()) {
        String client_id = "G.231.22.0154";
        Serial.printf("Connecting to MQTT Broker as %s.....\n", client_id.c_str());
        if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Connected to MQTT broker");
            mqtt_client.subscribe(mqtt_sub);
            // Publish message upon successful connection
            mqtt_client.publish(mqtt_pub, "Hi Nata I'm ESP8266 ^^");
        } else {
            Serial.print("Failed to connect to MQTT broker, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);
    Serial.print("Message: ");
    
    // Convert the payload to a string for easier processing
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
        Serial.print((char)payload[i]);
    }
    Serial.println();
    
    // Handle LED control messages from web interface
    if (message.startsWith("LED1_")) {
        if (message.endsWith("ON")) {
            digitalWrite(LED1_PIN, HIGH);
            Serial.println("LED1 (GPIO12) turned ON");
        } else if (message.endsWith("OFF")) {
            digitalWrite(LED1_PIN, LOW);
            Serial.println("LED1 (GPIO12) turned OFF");
        }
    } 
    else if (message.startsWith("LED2_")) {
        if (message.endsWith("ON")) {
            digitalWrite(LED2_PIN, HIGH);
            Serial.println("LED2 (GPIO13) turned ON");
        } else if (message.endsWith("OFF")) {
            digitalWrite(LED2_PIN, LOW);
            Serial.println("LED2 (GPIO13) turned OFF");
        }
    } 
    else if (message.startsWith("LED3_")) {
        if (message.endsWith("ON")) {
            digitalWrite(LED3_PIN, HIGH);
            Serial.println("LED3 (GPIO15) turned ON");
        } else if (message.endsWith("OFF")) {
            digitalWrite(LED3_PIN, LOW);
            Serial.println("LED3 (GPIO15) turned OFF");
        }
    }
    
    Serial.println("-----------------------");
}

JsonDocument doc;
char output[100];

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  connectToWiFi();

  mqtt_client.setServer(mqtt_broker, mqtt_port);
  mqtt_client.setCallback(mqttCallback);
  connectToMQTTBroker();

  // Initialize all LED pins as outputs using GPIO numbers
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // Initialize all LEDs to OFF state
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  dht.begin();

  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("IoT Monitoring");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
  
  // Print GPIO pin information
  Serial.println("LED1 on GPIO12 (D6)");
  Serial.println("LED2 on GPIO13 (D7)");
  Serial.println("LED3 on GPIO15 (D8)");
  Serial.println("DHT11 on GPIO2 (D4)");
}

void loop() {
  // Check MQTT connection
  if (!mqtt_client.connected()) {
    connectToMQTTBroker();
  }
  mqtt_client.loop();
  
  // Read sensor data
  t = dht.readTemperature();
  h = dht.readHumidity();

  // Check if any reads failed
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Update LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Humid: ");
  lcd.print(h);
  lcd.print(" %");

  Serial.println(t);
  Serial.println(h);

  // Prepare JSON data
  doc["suhu"] = t;
  doc["kelembapan"] = h;
  doc.shrinkToFit();  // optional

  serializeJson(doc, output);

  // Publish sensor data
  mqtt_client.publish(mqtt_pub, output);

  delay(1000);
}