# 🌡️ ESP32 Temperature & Humidity Monitor with DHT22

This repository contains example code for measuring environmental temperature and humidity using the DHT22 sensor and displaying the results on an SSD1306 OLED display, both connected to an ESP32.

## 🛠️ Components Used
- ESP32
- DHT22 temperature and humidity sensor
- SSD1306 OLED display (128x64)
- Jumper wires

## ✨ Features
- Real-time temperature and humidity measurement
- OLED display visualization
- Simple and modular code

## 📦 Required Libraries
Install the following libraries in the Arduino IDE:
- Adafruit Unified Sensor
- DHT sensor library
- Adafruit SSD1306
- Adafruit GFX Library
- Wire

## ⚡ Wiring

| DHT22      | ESP32      |
|------------|------------|
| VCC        | 3.3V       |
| GND        | GND        |
| DATA       | GPIO 15    |

| OLED SSD1306 | ESP32      |
|--------------|------------|
| VCC          | 3.3V       |
| GND          | GND        |
| SDA          | GPIO 21    |
| SCL          | GPIO 22    |

> **Note:** Confirm the pinout according to your ESP32 board and sensor version.

## 🚀 How to Use
1. Install the libraries listed above.
2. Connect the components as shown in the tables.
3. Upload the `DHT22.ino` file to your ESP32.
4. Open the Serial Monitor to see temperature and humidity readings.
5. The OLED display will show the current temperature (°C) and humidity (%).

## 🖥️ Example Output on Display
```
Temp: 24.1°C
Hum: 56%
```

## 📝 Notes
- The code reads sensor data every 2 seconds and updates the display in real time.
- If the DHT22 or OLED display is not detected, an error message will be shown in the Serial Monitor and the program will halt.

## 🤝 Contributing
Feel free to open issues and pull requests to improve this project!

## ⚖️ License
This project is licensed under the MIT License.

---
Developed by Giordano Berwig

