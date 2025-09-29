#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_SHT31.h>
#include <Adafruit_BMP085.h>

#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 
#define OLED_I2C_ADDRESS 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_SHT31 sht31 = Adafruit_SHT31(); 
Adafruit_BMP085 bmp; 

void setup() {
  Serial.begin(115200);
  
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN); 
  delay(100);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("ERRO: Falha ao inicializar o display OLED!"));
    for (;;) ;
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();
  
  if (!sht31.begin(0x44)) { 
    Serial.println("ERRO: Falha ao encontrar o sensor SHT31 (0x44)!");
  } else {
    Serial.println("SHT31 OK.");
  }
  
  if (!bmp.begin()) {
    Serial.println("ERRO: Sensor BMP180 nao encontrado!");
  } else {
    Serial.println("BMP180 OK.");
  }
  
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Estacao DevKit V1 OK!");
  display.display();
  delay(1500);
}

void loop() {
  float temperatura = sht31.readTemperature(); 
  float umidade = sht31.readHumidity();       
  float pressaoHPa = bmp.readPressure() / 100.0F; 

  if (isnan(temperatura) || isnan(umidade)) {
    temperatura = -99.9;
    umidade = -99.9;
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temp:");
  display.setTextSize(2);
  display.setCursor(35, 0);
  display.print(temperatura, 1);
  display.print((char)247); 
  display.print("C");

  display.setTextSize(1);
  display.setCursor(0, 24);
  display.print("Umid:");
  display.setTextSize(2);
  display.setCursor(35, 24);
  display.print(umidade, 1);
  display.print(" %");

  display.setTextSize(1);
  display.setCursor(0, 48);
  display.print("Pres:");
  display.setTextSize(2);
  display.setCursor(35, 48);
  if (pressaoHPa > 500) { 
    display.print(pressaoHPa, 0); 
  } else {
    display.print("ERRO");
  }
  display.setTextSize(1);
  display.setCursor(100, 52);
  display.print("hPa");
  
  display.display();

  Serial.print("T: "); Serial.print(temperatura, 1); 
  Serial.print("C | U: "); Serial.print(umidade, 1); 
  Serial.print("% | P: "); Serial.print(pressaoHPa, 2); 
  Serial.println("hPa");

  delay(1000); 
}