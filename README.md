# 🌱 ESP32 Air Quality Monitor

This project uses an ESP32 microcontroller to monitor environmental variables: temperature, humidity, equivalent CO₂ (eCO₂), and total volatile organic compounds (TVOC). The data is displayed in real time on an SSD1306 OLED display and sent to the serial monitor for debugging and logging.

## ✨ Features
- Real-time measurement of temperature and humidity (SHT31)
- Real-time measurement of eCO₂ (ppm) and TVOC (ppb) (CCS811)
- OLED display visualization
- Serial output for monitoring and debugging

## 🧩 Components
- **ESP32**
- **SHT31** (temperature/humidity sensor)
- **CCS811** (air quality sensor: eCO₂/TVOC)
- **SSD1306 OLED** (I2C display)

## ⚡ Wiring
All devices use the I2C bus. Connect as follows (default ESP32 pins):

| Component   | ESP32 Pin |
|-------------|-----------|
| SHT31 SDA   | GPIO 21   |
| SHT31 SCL   | GPIO 22   |
| CCS811 SDA  | GPIO 21   |
| CCS811 SCL  | GPIO 22   |
| OLED SDA    | GPIO 21   |
| OLED SCL    | GPIO 22   |

> **Note:** Confirm addresses and pins for your board and modules.

## 📝 How It Works
- Initializes the SHT31, CCS811, and OLED display.
- Reads temperature and humidity from SHT31.
- Updates CCS811 with environmental data for compensation.
- Reads eCO₂ and TVOC from CCS811.
- Displays all values on the OLED and Serial Monitor.
- Updates every 5 seconds.

### Example OLED Output
```
Temp: 23.4°C
Umid: 45.7%
CO2: 412 ppm
VOC: 12 ppb
```

## 🚀 Getting Started
1. Install the required libraries in Arduino IDE:
   - Adafruit SHT31
   - Adafruit CCS811
   - Adafruit SSD1306
   - Adafruit GFX
   - Wire
2. Connect the sensors and display as shown above.
3. Open `sht31_ccs811.ino` in Arduino IDE.
4. Upload to your ESP32.
5. View live data on the Serial Monitor (115200 baud) or OLED display.

## 🤝 Contributing
Feel free to open issues and pull requests to improve this project!

## ⚖️ License
This project is licensed under the MIT License.

---
Developed by Giordano Berwig
