# IoT Temperature & Humidity Monitoring System

A complete IoT monitoring system that collects temperature and humidity data from ESP8266 sensors and displays it in a modern web interface with real-time updates and LED control capabilities.

## Project Overview

This project consists of two main components:
- **ESP8266 Hardware** - Monitors temperature and humidity using a DHT11 sensor and allows remote control of LEDs
- **Web Interface** - Displays sensor data in real-time charts and provides LED control toggles

## Features

- 📊 **Real-time Data Visualization** - Live charts for temperature and humidity readings
- 💡 **LED Control** - Remote control of three LEDs connected to the ESP8266
- 📱 **Responsive Design** - Works across desktop and mobile devices
- 🔔 **Alert System** - Audible alerts when temperature or humidity exceeds thresholds
- 💾 **Data Logging** - Stores sensor readings in a MySQL database

## Project Structure

```
uas-iot/
├── esp8266-program/
│   └── esp8266-program.ino   # ESP8266 Arduino code
└── website/
    ├── index.html            # Main HTML file
    ├── assets/
    │   ├── css/
    │   │   └── style.css     # Extracted CSS styles
    │   ├── js/
    │   │   ├── app.js        # Main application logic (MQTT handling, charts)
    │   │   └── custom.js     # Custom UI enhancements and overrides
    │   └── images/           # For future image assets
    └── api/
        └── save_data.php     # Backend API for saving data to database
```

## Hardware Requirements

- NodeMCU ESP8266 development board
- DHT11 Temperature & Humidity sensor
- 3 LEDs (connected to GPIO12/D6, GPIO13/D7, GPIO15/D8)
- Jumper wires
- Breadboard
- 5V power supply

## Software Requirements

- Arduino IDE with ESP8266 board support
- Web server with PHP and MySQL support
- Modern web browser (Chrome, Firefox, Edge, etc.)

## Setup Instructions

### ESP8266 Setup

1. Connect the DHT11 sensor to GPIO2 (D4) of the ESP8266
2. Connect LEDs to GPIO12 (D6), GPIO13 (D7), and GPIO15 (D8) with appropriate resistors
3. Open the Arduino IDE and install the necessary libraries:
   - DHT sensor library
   - PubSubClient (MQTT)
   - ESP8266WiFi
   - ArduinoJSON
   - LiquidCrystal_I2C (if using an LCD display)

4. Update the WiFi and MQTT credentials in the `esp8266-program.ino` file:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```

5. Upload the code to your ESP8266 board

### Web Interface Setup

1. Set up a web server with PHP and MySQL support
2. Create a MySQL database named `iot_database` with the following structure:
   ```sql
   CREATE TABLE `data` (
     `id` int(11) NOT NULL AUTO_INCREMENT,
     `suhu` float NOT NULL,
     `kelembapan` float NOT NULL,
     `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
     PRIMARY KEY (`id`)
   );
   ```

3. Update the database connection details in `api/save_data.php`:
   ```php
   $dbHost = "localhost";
   $dbUser = "your_database_username";
   $dbPass = "your_database_password";
   $dbName = "iot_database";
   ```

4. Update the MQTT broker information in `assets/js/app.js` if needed:
   ```javascript
   const brokerHost = "mqtt.revolusi-it.com";
   const brokerPort = 9001;
   const nim = "YOUR_NIM";
   ```

5. Upload all files to your web server

## How It Works

1. **Data Collection**: The ESP8266 reads temperature and humidity data from the DHT11 sensor
2. **Data Transmission**: Data is sent to the MQTT broker in JSON format
3. **Web Interface**: The web interface subscribes to the MQTT topic and displays real-time data
4. **LED Control**: The web interface sends MQTT messages to control LEDs connected to the ESP8266
5. **Data Storage**: Data is stored in the MySQL database via AJAX calls to save_data.php

## MQTT Topics

- **Topic Subscribe**: `iot/G.231.22.0154` - For receiving sensor data
- **Topic Publish**: `iot/G.231.22.0154/sub` - For sending LED control commands

## LED Control Commands

- `LED1_ON` / `LED1_OFF` - Control LED connected to GPIO12 (D6)
- `LED2_ON` / `LED2_OFF` - Control LED connected to GPIO13 (D7)
- `LED3_ON` / `LED3_OFF` - Control LED connected to GPIO15 (D8)

## Alert Rules

- **Temperature**:
  - Beep 3x when temperature > 31°C
  - Beep 2x when temperature between 30°C and 31°C
  - Beep 1x when temperature between 29°C and 30°C

- **Humidity**:
  - Beep 3x when humidity ≥ 70%
  - Beep 1x when humidity between 60% and 70%

## Credits

- Created by Alif Naufal Ilyas (G.231.22.0154)
- MQTT Broker provided by Revolusi IT
- Libraries: Chart.js, MQTT.js, Bootstrap, and more

## License

This project is licensed under the MIT License - see the LICENSE file for details.
