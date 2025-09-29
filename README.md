# 🌡️ ESP32 Temperature & Pressure Monitor with BMP180

This repository contains example code for measuring environmental temperature and atmospheric pressure using the BMP180 sensor and displaying the results on an SSD1306 OLED display, both connected to an ESP32.

## 🛠️ Components Used
- ESP32
- BMP180 barometric pressure sensor (I2C)
- SSD1306 OLED display (128x64)
- Jumper wires

## ✨ Features
- Real-time temperature and pressure measurement
- OLED display visualization
- Simple and modular code

## 📦 Required Libraries
Install the following libraries in the Arduino IDE:
- Adafruit BMP180 Library
- Adafruit SSD1306
- Adafruit GFX Library
- Wire

## ⚡ Wiring

| BMP180      | ESP32      |
|-------------|------------|
| VCC         | 3.3V       |
| GND         | GND        |
| SDA         | GPIO 21    |
| SCL         | GPIO 22    |

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
3. Upload the `bmp180.ino` file to your ESP32.
4. Open the Serial Monitor to see temperature and pressure readings.
5. The OLED display will show the current temperature (°C) and pressure (hPa).

## 🖥️ Example Output on Display
```
Temp: 23.4°C
Press: 1013 hPa
```

## 📝 Notes
- The code reads sensor data every 1 second and updates the display in real time.
- If the BMP180 or OLED display is not detected, an error message will be shown in the Serial Monitor and the program will halt.

## 🤝 Contributing
Feel free to open issues and pull requests to improve this project!

## ⚖️ License
This project is licensed under the MIT License.

---
Developed by Giordano Berwig
