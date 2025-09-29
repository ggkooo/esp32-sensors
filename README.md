# 🌡️📡 ESP32 Temperature, Humidity & Pressure Monitor with SHT31, BMP180 and WebServer

This repository contains example code for measuring environmental temperature, humidity, and atmospheric pressure using the SHT31 and BMP180 sensors, and serving the results via a WebServer on the ESP32.

## 🛠️ Components Used
- ESP32
- SHT31 temperature and humidity sensor (I2C)
- BMP180 barometric pressure sensor (I2C)
- Jumper wires

## ✨ Features
- Real-time temperature, humidity, and pressure measurement
- Embedded WebServer for remote data access
- Simple and modular code

## 📦 Required Libraries
Install the following libraries in the Arduino IDE:
- Adafruit SHT31 Library
- Adafruit BMP180 Library
- Wire
- WiFi
- WebServer

## ⚡ Wiring

| SHT31      | ESP32  |
|------------|--------|
| VCC        | 3.3V   |
| GND        | GND    |
| SDA        | GPIO 4 |
| SCL        | GPIO 5 |

| BMP180     | ESP32  |
|------------|--------|
| VCC        | 3.3V   |
| GND        | GND    |
| SDA        | GPIO 4 |
| SCL        | GPIO 5 |

> **Note:** Confirm the pinout according to your ESP32 board and sensor version.

## 🚀 How to Use
1. Install the libraries listed above.
2. Connect the components as shown in the tables.
3. Edit your WiFi credentials in `sht31-bpm180-ws.ino`.
4. Upload the `sht31-bpm180-ws.ino` file to your ESP32.
5. Open the Serial Monitor to see connection status and sensor readings.
6. Access the ESP32's IP address in your browser to view live temperature, humidity, and pressure data.

## 🌐 Example Output on WebServer
```
{
  "temperature": 23.4,
  "humidity": 45.7,
  "pressure": 1013,
  "timestamp": "29-09-25 11:43:30"
}
```

## 📝 Notes
- The code reads sensor data every 1 second and updates the web interface in real time.
- If any sensor is not detected, an error message will be shown in the Serial Monitor and the program will halt.
- The WebServer serves data in JSON format for easy integration with other systems.

## 🤝 Contributing
Feel free to open issues and pull requests to improve this project!

## ⚖️ License
This project is licensed under the MIT License.

---
Developed by Giordano Berwig
