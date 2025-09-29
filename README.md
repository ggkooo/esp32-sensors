# 🌡️ ESP32 Temperature, Humidity & Pressure Monitor with SHT31 and BMP180

This repository contains example code for measuring environmental temperature, humidity, and atmospheric pressure using the SHT31 and BMP180 sensors, and displaying the results on an SSD1306 OLED display, all connected to an ESP32.

## 🛠️ Components Used
- ESP32
- SHT31 temperature and humidity sensor (I2C)
- BMP180 barometric pressure sensor (I2C)
- SSD1306 OLED display (128x64)
- Jumper wires

## ✨ Features
- Real-time temperature, humidity, and pressure measurement
- OLED display visualization
- Simple and modular code

## 📦 Required Libraries
Install the following libraries in the Arduino IDE:
- Adafruit SHT31 Library
- Adafruit BMP180 Library
- Adafruit SSD1306
- Adafruit GFX Library
- Wire

## ⚡ Wiring

| SHT31      | ESP32  |
|------------|--------|
| VCC        | 3.3V   |
| GND        | GND    |
| SDA        | GPIO 4 |
| SCL        | GPIO 5 |

| BMP180     | ESP32   |
|------------|---------|
| VCC        | 3.3V    |
| GND        | GND     |
| SDA        | GPIO 4 |
| SCL        | GPIO 5  |

| OLED SSD1306 | ESP32  |
|--------------|--------|
| VCC          | 3.3V   |
| GND          | GND    |
| SDA          | GPIO 4 |
| SCL          | GPIO 5 |

> **Note:** Confirm the pinout according to your ESP32 board and sensor version.

## 🚀 How to Use
1. Install the libraries listed above.
2. Connect the components as shown in the tables.
3. Upload the `sht31-bmp180.ino` file to your ESP32.
4. Open the Serial Monitor to see temperature, humidity, and pressure readings.
5. The OLED display will show the current temperature (°C), humidity (%), and pressure (hPa).

## 🖥️ Example Output on Display
```
Temp: 23.4°C
Umid: 45.7 %
Press: 1013 hPa
```

## 📝 Notes
- The code reads sensor data every 1 second and updates the display in real time.
- If any sensor or the OLED display is not detected, an error message will be shown in the Serial Monitor and the program will halt.

## 🤝 Contributing
Feel free to open issues and pull requests to improve this project!

## ⚖️ License
This project is licensed under the MIT License.

---
Developed by Giordano Berwig
