#include <Wire.h>
#include <Adafruit_SHT31.h>
#include <Adafruit_BMP085.h>
#include <WiFi.h>
#include <WebServer.h>
#include "time.h"

#include "FS.h"
#include "LittleFS.h"

const char* ssid = "VIVOFIBRA-35F1";
const char* password = "#1doutor_berwig";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -10800;
const int daylightOffset_sec = 0;

#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5

const char* logFileName = "/data_log.json";

int lastSecond = -1; 

Adafruit_SHT31 sht31 = Adafruit_SHT31(); 
Adafruit_BMP085 bmp; 
WebServer server(80);

void handleLogFile() {
  File file = LittleFS.open(logFileName, "r");
  if (!file) {
    server.send(404, "text/plain", "ERRO: Arquivo de log nao encontrado. Aguarde a proxima escrita.");
    return;
  }
  
  server.streamFile(file, "application/json");
  file.close();
}

void handleRoot() {
  String html = "<html><head><meta http-equiv='refresh' content='10'></head><body>";
  html += "<h1>Estacao DevKit V1 - Logging</h1>";
  html += "<p><b>IP Atual:</b> " + WiFi.localIP().toString() + "</p>";
  html += "<p>Acesse <a href='/log.json'>/log.json</a> para baixar os dados brutos.</p>";
  html += "<hr>";

  File file = LittleFS.open(logFileName, "r");
  if(file){
    html += "<p>Arquivo de Log (" + String(logFileName) + ") encontrado. Tamanho: <b>" + String(file.size() / 1024.0, 2) + " KB</b></p>";
    file.close();
  } else {
    html += "<p style='color:red;'>Nenhum arquivo de log encontrado. Aguarde o proximo registro.</p>";
  }
  
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();
  float p = bmp.readPressure() / 100.0F;

  html += "<h2>Leitura Atual (Real-Time):</h2>";
  html += "<ul>";
  html += "<li>Temperatura (SHT31): <b>" + String(t, 1) + " °C</b></li>";
  html += "<li>Umidade (SHT31): <b>" + String(h, 1) + " %</b></li>";
  html += "<li>Pressao (BMP180): <b>" + String(p, 2) + " hPa</b></li>";
  html += "</ul>";

  html += "</body></html>";
  server.send(200, "text/html", html);
}

void logSensorData() {
  
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Falha ao obter o tempo do NTP. Nao e possivel registrar.");
    return;
  }

  float temperatura = sht31.readTemperature(); 
  float umidade = sht31.readHumidity();       
  float pressaoHPa = bmp.readPressure() / 100.0F; 
  
  if (isnan(temperatura)) { 
    Serial.println("ERRO: Falha na leitura do SHT31. Nao registrar.");
    return;
  }

  char timestamp_buffer[20];
  strftime(timestamp_buffer, sizeof(timestamp_buffer), "%d-%m-%y %H:%M:%S", &timeinfo);
  String formattedTimestamp = String(timestamp_buffer);

  String jsonString = "{";
  jsonString += "\"temperatura_c\": " + String(temperatura, 1) + ",";
  jsonString += "\"umidade_percent\": " + String(umidade, 1) + ",";
  jsonString += "\"pressao_hpa\": " + String(pressaoHPa, 2)+ ",";
  jsonString += "\"timestamp\": \"" + formattedTimestamp + "\"";
  jsonString += "},\n"; 

  File file = LittleFS.open(logFileName, FILE_APPEND);
  if (!file) {
    Serial.println("Falha ao abrir o arquivo para escrita.");
    return;
  }

  if (file.print(jsonString)) {
    Serial.print("Registro salvo com sucesso: ");
    Serial.println(jsonString);
  } else {
    Serial.println("Falha na escrita do arquivo.");
  }
  file.close();
}


void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando Estacao Web Server ESP32...");

  if (!LittleFS.begin()) {
    Serial.println("Ocorreu um erro ao montar o LittleFS. Formatando...");
    LittleFS.format();
    if(!LittleFS.begin()){
        Serial.println("Falha ao montar LittleFS apos formatacao. PARANDO.");
        while(true);
    }
  }
  Serial.println("LittleFS montado com sucesso.");

  Serial.printf("Conectando a %s\n", ssid);
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nCONECTADO!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFalha na conexao WiFi. Verifique as credenciais.");
  }


  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("NTP configurado.");

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN); 
  delay(100);

  if (!sht31.begin(0x44)) { 
    Serial.println("ERRO: Falha ao encontrar o sensor SHT31!");
  } else {
    Serial.println("SHT31 OK.");
  }
  
  if (!bmp.begin()) {
    Serial.println("ERRO: Sensor BMP180 nao encontrado!");
  } else {
    Serial.println("BMP180 OK.");
  }

  server.on("/", handleRoot);
  server.on("/log.json", handleLogFile);
  server.begin();
  Serial.println("Servidor HTTP iniciado na porta 80.");
}

void loop() {
  server.handleClient();
  
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    if (timeinfo.tm_sec % 5 == 0 && timeinfo.tm_sec != lastSecond) {
      logSensorData();
      lastSecond = timeinfo.tm_sec;
    }
    delay(50);
  } else {
    delay(100); 
  }
}
