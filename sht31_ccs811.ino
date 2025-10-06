#include <Wire.h>
#include "Adafruit_CCS811.h"
#include "Adafruit_SHT31.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_CCS811 ccs;
Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup() {
  Serial.begin(115200);
  Serial.println("--- Monitor de Qualidade do Ar: CCS811, SHT31 e OLED ---");

  Wire.begin(); 

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("ERRO: Falha ao iniciar o OLED."));
  } else {
    Serial.println("OLED: OK.");
  }
  display.display();
  delay(2000);
  display.clearDisplay(); 
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  if (!sht31.begin(0x44)) {
    Serial.println("ERRO: Falha ao iniciar o sensor SHT31.");
  } else {
    Serial.println("SHT31: OK.");
  }
  
  if (!ccs.begin()) {
    Serial.println("ERRO: Falha ao iniciar o sensor CCS811.");
    while (1);
  }
  Serial.println("CCS811: OK.");

  Serial.print("Aguardando CCS811 ficar pronto...");
  while (!ccs.available()) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Pronto para leituras!");
}

void loop() {
  float temp_c = 0.0;
  float umidade = 0.0;
  
  temp_c = sht31.readTemperature();
  umidade = sht31.readHumidity();
    
  if (!isnan(temp_c) && !isnan(umidade)) {
      ccs.setEnvironmentalData(umidade, temp_c);
  }

  if (ccs.available()) {
    if (!ccs.readData()) {
      
      uint16_t eco2 = ccs.geteCO2();
      uint16_t tvoc = ccs.getTVOC();

      Serial.print("T: "); Serial.print(temp_c, 1); Serial.print("°C | ");
      Serial.print("U: "); Serial.print(umidade, 1); Serial.print("% | ");
      Serial.print("eCO2: "); Serial.print(eco2); Serial.print(" ppm | ");
      Serial.print("TVOC: "); Serial.print(tvoc); Serial.println(" ppb");
      
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);

      display.setCursor(0, 0);
      display.print("Temp: "); 
      display.print(temp_c, 1);
      display.print((char)247); display.println("C");

      display.setCursor(0, 16);
      display.print("Umid: "); 
      display.print(umidade, 1);
      display.println("%");

      display.setCursor(0, 32); 
      display.print("CO2: ");
      display.print(eco2);
      display.println(" ppm");

      display.setCursor(0, 48); 
      display.print("VOC: ");
      display.print(tvoc);
      display.println(" ppb");
      
      display.display();
      
    } else {
      Serial.println("ERRO na leitura do CCS811!");
    }
  }

  delay(5000); 
}