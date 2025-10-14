# W5500 Ethernet Module with Arduino: NTP Time Example 🌐🕒

This project demonstrates how to use the W5500 Ethernet module with an Arduino to connect to a network, obtain an IP address via DHCP, and request the current time from an NTP server using UDP. 🛠️

## Hardware Required 🔌

- Arduino board (Uno, Mega, etc.)
- W5500 Ethernet module
- Jumper wires

## Wiring 🧩

Connect the W5500 module to the Arduino as follows:

| W5500 Pin | Arduino Pin |
|-----------|-------------|
| VCC       | 3.3V or 5V  |
| GND       | GND         |
| SCK       | SCK (D18 on ESP32, D13 on Uno) |
| MISO      | MISO (D19 on ESP32, D12 on Uno) |
| MOSI      | MOSI (D23 on ESP32, D11 on Uno) |
| CS        | D5          |
| RST       | D2          |

> ⚠️ Note: Pin numbers may vary depending on your Arduino or ESP32 model. Adjust accordingly.

## Installation 🖥️

1. Install the [Ethernet](https://www.arduino.cc/en/Reference/Ethernet) library via Arduino Library Manager.
2. Download or clone this repository.
3. Open `w5500-module.ino` in the Arduino IDE.

## Usage 🚀

1. Wire the W5500 module to your Arduino as described above.
2. Upload the sketch to your Arduino.
3. Open the Serial Monitor at 115200 baud to view network status and the current time.

## Example Output 📟

```
Inicializando Ethernet com W5500...
Endereço IP: 192.168.1.100
UDP iniciado. Solicitando hora NTP...
Pacote NTP enviado para: time.cloudflare.com
Hora atual (UTC ajustada): 12:34:56
```

## Troubleshooting 🛠️

- Ensure all wiring is correct and secure.
- Use a reliable power source for the W5500 module.
- If you see "W5500 nao detectado", check SPI connections and CS pin.
- If you see "Cabo de rede desconectado", check your Ethernet cable and network.
- Make sure your router has DHCP enabled.

## License 📄

This project is licensed under the MIT License.
