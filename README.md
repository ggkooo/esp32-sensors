# 🏭 ESP32 Air Quality Logger & API

Comprehensive environmental monitoring with ESP32: temperature, humidity, pressure, air quality, and noise sensors, OLED display, local JSON storage, and PHP API integration.

---

## ✨ Features
- Real-time measurement: temperature, humidity, pressure, noise (dB), eCO2, TVOC
- OLED display visualization
- Local JSON storage (LittleFS)
- NTP time synchronization
- Automatic data upload to PHP API
- WiFi setup via WiFiManager
- Visual feedback with LEDs (status, error, upload)

---

## 🧩 Supported Sensors & Displays
- SHT31: temperature and humidity
- INMP441: noise (dB)
- BMP180: atmospheric pressure
- CCS811: eCO2 and TVOC
- SSD1306 OLED: data display

---

## ⚡ Wiring Diagram
| Sensor/Display | ESP32 Pin |
|---------------|-----------|
| SHT31 SDA     | GPIO 21   |
| SHT31 SCL     | GPIO 22   |
| INMP441 WS    | GPIO 25   |
| INMP441 SCK   | GPIO 26   |
| INMP441 SD    | GPIO 35   |
| BMP180 SDA    | GPIO 21   |
| BMP180 SCL    | GPIO 22   |
| CCS811 SDA    | GPIO 21   |
| CCS811 SCL    | GPIO 22   |
| OLED SDA      | GPIO 21   |
| OLED SCL      | GPIO 22   |
| GREEN LED     | GPIO 18   |
| YELLOW LED    | GPIO 19   |
| RED LED       | GPIO 23   |

---

## 📊 Output Examples
### OLED
```
Temp: 23.4°C
Hum: 45.7%
Noise: 56.2dB
---
eCO2: 412 ppm
TVOC: 23 ppb
Pres: 1013 hPa
```

### JSON
```json
{
  "temperature": 23.4,
  "humidity": 45.7,
  "noise": 56.2,
  "pression": 1013.2,
  "eco2": 412,
  "tvoc": 23,
  "timestamp": "2025-10-09 09:20:15"
}
```

---

## 🌐 PHP API
- Endpoint: `/simple-api.php`
- Method: `POST`
- Payload: JSON array of records
- Required fields: `temperature`, `humidity`, `noise`, `pression`, `eco2`, `tvoc`, `timestamp`
- Response: status and error/success message

### Request Example
```bash
curl -X POST http://<server-ip>/simple-api.php \
     -H "Content-Type: application/json" \
     -d '[{"temperature":23.4,"humidity":45.7,"noise":56.2,"pression":1013.2,"eco2":412,"tvoc":23,"timestamp":"2025-10-09 09:20:15"}]'
```

---

## 🚀 Installation & Usage
1. Install the following libraries in Arduino IDE:
   - Adafruit SHT31
   - Adafruit SSD1306
   - Adafruit GFX
   - Adafruit BMP085
   - Adafruit CCS811
   - Wire
   - WiFiManager
   - LittleFS
2. Connect sensors and display as shown above.
3. Open the `.ino` file in Arduino IDE, edit WiFi credentials and API endpoint if needed.
4. Upload to your ESP32.
5. Use Serial Monitor and/or OLED to view data.
6. For web/API integration, set up PHP on your server and access the endpoint.

---

## ⚖️ License
MIT License

---
Developed by Giordano Berwig
