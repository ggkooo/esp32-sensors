#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP085.h>

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

// --- CONFIGURAÇÃO OLED ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 
#define OLED_I2C_ADDRESS 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(115200);
  
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  if (!bmp.begin()) {
    Serial.println("ERRO: Sensor BMP180 nao encontrado. Verifique as ligacoes.");
    while (1); 
  }
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println("ERRO: Display OLED nao encontrado. Verifique o endereco (0x3C ou 0x3D).");
    while (1);
  }
  
  display.display();
  delay(2000); 

  display.clearDisplay(); // Limpa a tela
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Sistema Pronto!");
  display.println("Lendo dados...");
  display.display();
  delay(1000);
}

void loop() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("--- Estacao BMP180 ---");
  display.drawLine(0, 10, 127, 10, WHITE);
  
  float temperatura = bmp.readTemperature();
  float pressaoHPa = bmp.readPressure() / 100.0F;
  float altitude = bmp.readAltitude(101325); 

  display.setTextSize(2);
  display.setCursor(0, 15);
  display.print("Temp: ");
  display.print(temperatura, 1);
  display.println(" C");
  
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Pressao: ");
  display.print(pressaoHPa, 2);
  display.println(" hPa");

  display.setCursor(0, 50);
  display.print("Altitude: ");
  display.print(altitude, 1);
  display.println(" m");
  
  display.display();
  
  Serial.print("T: "); Serial.print(temperatura); Serial.print(" C | P: "); Serial.print(pressaoHPa); Serial.print(" hPa | A: "); Serial.print(altitude); Serial.println(" m");

  delay(3000);
}