#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 4 
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_I2C_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  Serial.begin(115200);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println("ERRO: Display OLED nao encontrado.");
    while (1);
  }
  
  dht.begin();
  
  display.clearDisplay(); 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("ESP32 + DHT22");
  display.println("Iniciando...");
  display.display();
  delay(2000);
}

void loop() {
  delay(2000); 

  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();

  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Falha na leitura do sensor DHT!");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("ERRO DE LEITURA!");
    display.println("DHT22 indisponivel.");
    display.display();
    return;
  }
  
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Monitor de Ambiente");
  display.drawLine(0, 10, 127, 10, WHITE);
  
  display.setTextSize(2);
  display.setCursor(0, 15);
  display.print("UMID:");
  display.print(umidade, 1);
  display.println("%");
  
  display.setCursor(0, 40);
  display.print("TEMP:");
  display.print(temperatura, 1); 
  display.print((char)247);
  display.println("C");

  display.display();
  
  Serial.print("Umidade: "); Serial.print(umidade); Serial.print(" % | Temperatura: "); Serial.print(temperatura); Serial.println(" °C");
}