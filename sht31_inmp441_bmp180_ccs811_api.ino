#include "driver/i2s.h"
#include <Wire.h>
#include "Adafruit_CCS811.h"
#include "Adafruit_SHT31.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP085.h>
#include <math.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include "time.h"
#include <HTTPClient.h>
#include "LittleFS.h"
#include <ArduinoJson.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define I2S_WS 25
#define I2S_SCK 26
#define I2S_SD 35
#define LED_GREEN 13
#define LED_YELLOW 14
#define LED_RED 12
#define BUZZER_PIN 27
const char* AP_SSID = "Giordano's ESP32";
const char* AP_PASSWORD = "giordanolindo123";
const char* ntpServer1 = "time.google.com";
const char* ntpServer2 = "time.cloudflare.com";
const long gmtOffset_sec = -10800;
const int daylightOffset_sec = 0;
const char* DATA_FILE = "/sensor_data.json";
const char* API_ENDPOINT = ""; 
const char* API_KEY = "";
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_CCS811 ccs;
Adafruit_SHT31 sht31 = Adafruit_SHT31();
Adafruit_BMP085 bmp;
WiFiManager wm;
int16_t sample_buffer[512];
int bytes_read;
long peak_amplitude = 0;
int pagina_atual = 1;
int ultimo_segundo_ntp = -1;
int ultimo_minuto_api = -1;
float temp_c_ultima = 0.0;
float umidade_ultima = 0.0;
float pressaoHPa_ultima = 0.0;
float noise_level_dB_ultima = 0.0;
uint16_t eco2_ultima = 0;
uint16_t tvoc_ultima = 0;
bool wifi_conectado = false;
bool primeiro_minuto_sincronizado = false;
int contador_leituras = 0;
const int ledChannel = 0;
const int resolution = 10;
unsigned long ultimo_bip = 0;
const long intervalo_bip = 2500;
#define NOTE_C5 523
#define NOTE_E5 659
#define NOTE_G5 784
#define NOTE_C6 1047
#define NOTE_PAUSE 0
int melody[] = { NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6 };
int note_durations[] = { 200, 200, 200, 400 };
const int num_notes = sizeof(melody) / sizeof(int); 
int current_note_index = 0;
unsigned long tempo_fim_nota = 0;
String get_laravel_timestamp() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return "0000-00-00 00:00:00";
    }
    char time_output[20];
    strftime(time_output, sizeof(time_output), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return String(time_output);
}
String get_brasilia_time_string() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return "Time Sync...";
    }
    char time_output[30];
    strftime(time_output, sizeof(time_output), "%H:%M:%S", &timeinfo);
    return String(time_output);
}
void desenha_erro(String titulo, String mensagem) {
    digitalWrite(LED_RED, HIGH);
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2); 
    display.setCursor(0, 0);
    display.println(F("API ERROR")); 
    display.drawFastHLine(0, 18, SCREEN_WIDTH, SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 24);
    display.print("Codigo:");
    display.println(titulo);
    display.setCursor(0, 36);
    display.print("Msg:");
    display.println(mensagem); 
    display.setCursor(0, 56);
    display.print("BRT: ");
    display.print(get_brasilia_time_string());
    display.display();
}
void salvar_dados_json(String timestamp) {
    const size_t capacity = JSON_OBJECT_SIZE(7) + 80;
    DynamicJsonDocument doc(capacity);
    doc["temperature"] = temp_c_ultima;
    doc["humidity"] = umidade_ultima;
    doc["noise"] = noise_level_dB_ultima;
    doc["pression"] = pressaoHPa_ultima;
    doc["eco2"] = eco2_ultima;
    doc["tvoc"] = tvoc_ultima;
    doc["timestamp"] = timestamp;
    String json_data;
    serializeJson(doc, json_data);
    File file = LittleFS.open(DATA_FILE, "a");
    if (!file) {
        Serial.println("ERRO: Falha ao abrir o arquivo para escrita.");
        return;
    }
    file.println(json_data);
    file.close();
}
void enviar_dados_lote() {
    Serial.println("\n--- INICIANDO ENVIO DE LOTE ---");
    digitalWrite(LED_RED, LOW);
    ledcWrite(ledChannel, 0); 
    current_note_index = 0; 
    tempo_fim_nota = 0; 
    File file = LittleFS.open(DATA_FILE, "r");
    if (!file) {
        Serial.println("INFO: Arquivo de dados vazio ou não encontrado. Pulando envio.");
        return;
    }
    String json_content = "";
    int record_count = 0;
    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();
        if (line.length() > 0) {
            if (json_content.length() > 0) {
                json_content += ",";
            }
            json_content += line;
            record_count++;
        }
    }
    file.close();
    if (record_count == 0) {
        Serial.println("INFO: Arquivo estava vazio. Nada para enviar.");
        return;
    }
    String payload = "{\"data\": [" + json_content + "]}";
    Serial.printf("Enviando %d registros (Payload size: %d bytes).\n", record_count, payload.length());
    HTTPClient http;
    http.setTimeout(10000);
    http.begin(API_ENDPOINT);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("x-api-key", API_KEY);
    int httpCode = http.POST(payload);
    if (httpCode > 0) {
        Serial.printf("HTTP Code: %d\n", httpCode);
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) { 
            if (LittleFS.remove(DATA_FILE)) {
                Serial.println("SUCESSO: Dados enviados e arquivo local limpo.");
            } else {
                Serial.println("ERRO: Falha ao limpar o arquivo local após o envio.");
            }
            String response = http.getString();
            Serial.print("Resposta da API: ");
            Serial.println(response);
        } else {
            String titulo = String(httpCode);
            String responseBody = http.getString();
            String mensagem = (responseBody.length() > 0) ? responseBody.substring(0, min((int)responseBody.length(), 15)) + "..." : "Sem resposta detalhada";
            desenha_erro(titulo, mensagem);
            Serial.printf("ERRO de API: Servidor retornou código %d.\n", httpCode);
            Serial.printf("Corpo da resposta: %s\n", responseBody.c_str());
        }
    } else {
        String titulo = "HTTP - Sem Conexao";
        String mensagem = http.errorToString(httpCode).c_str();
        desenha_erro(titulo, mensagem);
        Serial.printf("ERRO HTTP: Falha na conexão ou na requisição. Erro: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
}
void desenha_modo_ap(const char* ssid_ap) {
    digitalWrite(LED_RED, LOW);
    ledcWrite(ledChannel, 0); 
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("SETUP WIFI:"));
    display.setCursor(0, 10);
    display.print(F("1. Conecte-se a:"));
    display.setCursor(0, 22);
    display.setTextSize(2);
    display.print(ssid_ap);
    display.setTextSize(1);
    display.setCursor(0, 42);
    display.println(F("2. Acesse 192.168.4.1"));
    display.setCursor(0, 54);
    display.print(F("3. Configure a rede."));
    display.display();
}
void desenha_pagina_1() {
    if (digitalRead(LED_RED) == HIGH) return;
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Temp: ");
    display.print(temp_c_ultima, 1);
    display.print((char)247);
    display.println("C");
    display.setCursor(0, 14);
    display.print("Umid: ");
    display.print(umidade_ultima, 1);
    display.println("%");
    display.setCursor(0, 28);
    display.print("Noise: ");
    display.print(noise_level_dB_ultima, 1);
    display.println("dB");
    display.drawFastHLine(0, 42, SCREEN_WIDTH, SSD1306_WHITE);
    display.setCursor(110, 0);
    display.print("1/2");
    display.setCursor(0, 56);
    display.print("BRT: ");
    display.print(get_brasilia_time_string());
    display.display();
}

void desenha_pagina_2() {
    if (digitalRead(LED_RED) == HIGH) return;
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("eCO2: ");
    display.print(eco2_ultima);
    display.print(" ppm");
    display.setCursor(0, 14);
    display.print("TVOC: ");
    display.print(tvoc_ultima);
    display.print(" ppb");
    display.setCursor(0, 28);
    display.print("Pres: ");
    if (pressaoHPa_ultima > 500) {
        display.print(pressaoHPa_ultima, 0);
        display.print(" hPa");

    } else {
        display.print("ERRO");
    }
    display.drawFastHLine(0, 42, SCREEN_WIDTH, SSD1306_WHITE);
    display.setCursor(110, 0);
    display.print("2/2");
    display.setCursor(0, 56);
    display.print("BRT: ");
    display.print(get_brasilia_time_string());
    display.display();
}

void play_nice_alarm() {
    unsigned long tempo_atual = millis();
    if (tempo_atual >= tempo_fim_nota) {
        if (current_note_index >= num_notes) {
            if (tempo_atual - ultimo_bip >= intervalo_bip) {
                current_note_index = 0;
                ultimo_bip = tempo_atual;
            } else {
                ledcWrite(ledChannel, 0);
                return;
            }
        }
        int frequency = melody[current_note_index];
        int duration_ms = note_durations[current_note_index];
        if (frequency == NOTE_PAUSE) {
            ledcWrite(ledChannel, 0);
        } else {
            ledcWriteTone(ledChannel, frequency);
        }
        tempo_fim_nota = tempo_atual + duration_ms;
        current_note_index++;
    }
}


void setup() {
    Serial.begin(115200);
    Serial.println("--- Monitor de Qualidade do Ar: CCS811, SHT31, BMP180 e OLED ---");
    Wire.begin(32, 33);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    ledcSetup(ledChannel, 2000, resolution); 
    ledcAttachPin(BUZZER_PIN, ledChannel);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
    ledcWrite(ledChannel, 0); 
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("ERRO: Falha ao iniciar o OLED."));
    }
    display.display();
    delay(2000);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    if (!LittleFS.begin(true)) {
        Serial.println("ERRO: Falha ao montar o LittleFS.");
        return;
    }
    Serial.println("LittleFS montado com sucesso.");
    if (LittleFS.remove(DATA_FILE)) {
        Serial.println("INFO: Arquivo de log local limpo na inicialização.");
    } else {
        Serial.println("INFO: Arquivo de log local nao existia ou falhou ao limpar.");
    }
    Serial.println("Iniciando WiFiManager...");
    if (!wm.autoConnect(AP_SSID, AP_PASSWORD)) {
        Serial.println("Falha ao conectar e entrou no modo AP.");
        wifi_conectado = false;
    } else {
        Serial.println("Wi-Fi conectado! IP: " + WiFi.localIP().toString());
        wifi_conectado = true;
    }
    if (wifi_conectado) {
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
        Serial.println("Sincronizando Hora NTP (Brasília -3h)...");
        if (!sht31.begin(0x44)) {
            Serial.println("ERRO: Falha ao iniciar o sensor SHT31.");
        }
        if (!bmp.begin()) {
            Serial.println("ERRO: Sensor BMP180 nao encontrado!");
        }
        if (!ccs.begin()) {
            Serial.println("ERRO: Falha ao iniciar o sensor CCS811.");
            while (1)
                ;
        }
        Serial.print("Aguardando CCS811 ficar pronto...");
        while (!ccs.available()) {
            delay(500);
            Serial.print(".");
        }
        Serial.println(" Pronto.");
        i2s_config_t i2s_config = {
            .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
            .sample_rate = 16000,
            .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
            .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
            .communication_format = I2S_COMM_FORMAT_STAND_I2S,
            .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
            .dma_buf_count = 8,
            .dma_buf_len = 64,
            .use_apll = false
        };
        i2s_pin_config_t pin_config = {
            .bck_io_num = I2S_SCK, .ws_io_num = I2S_WS, .data_out_num = I2S_PIN_NO_CHANGE, .data_in_num = I2S_SD
        };
        i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
        i2s_set_pin(I2S_NUM_0, &pin_config);
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("Wi-Fi OK.");
        display.println("NTP OK. Sensores OK.");
        display.println("API: " + String(API_ENDPOINT));
        display.display();
        delay(3000);
    }
}

void loop() {
    if (!wifi_conectado) {
        wm.process();
        desenha_modo_ap(AP_SSID);
        return;
    }
    if (digitalRead(LED_RED) == HIGH) {
        desenha_erro("Conexao/API", "Verifique o Servidor"); 
        play_nice_alarm();
        delay(10);
        return;
    } 
    else { 
        ledcWrite(ledChannel, 0); 
        current_note_index = 0; 
        tempo_fim_nota = 0; 
        struct tm timeinfo;
        if (getLocalTime(&timeinfo)) {
            int current_second = timeinfo.tm_sec;
            int current_minute = timeinfo.tm_min;
            bool deve_ler = false;
            if (!primeiro_minuto_sincronizado) {
                if (current_second == 0) {
                    primeiro_minuto_sincronizado = true;
                    ultimo_segundo_ntp = -1;
                    Serial.println("SINCRONIZAÇÃO DE INÍCIO CONCLUÍDA. INICIANDO LEITURAS.");
                } else {
                    Serial.printf("AGUARDANDO O MINUTO EXATO (segundo 00)... Atual: %d\n", current_second);
                    delay(500);
                    return;
                }
            }
            if (((current_second % 5 == 0)) && (current_second != ultimo_segundo_ntp)) {
                deve_ler = true;
            }
            if (deve_ler) {
                ultimo_segundo_ntp = current_second;
                digitalWrite(LED_GREEN, HIGH);
                if ((current_minute != ultimo_minuto_api) && (contador_leituras >= 12)) {
                    digitalWrite(LED_YELLOW, HIGH);
                    ultimo_minuto_api = current_minute;
                    Serial.println("\n--- CICLO DE ENVIO INICIADO (Novo Minuto) ---");
                    enviar_dados_lote();
                    contador_leituras = 0;
                    delay(300);
                    digitalWrite(LED_YELLOW, LOW);
                }
                float temp_c = sht31.readTemperature();
                float umidade = sht31.readHumidity();
                float pressaoHPa = bmp.readPressure() / 100.0F;
                i2s_read(I2S_NUM_0, (char*)sample_buffer, sizeof(sample_buffer), (size_t*)&bytes_read, portMAX_DELAY);
                peak_amplitude = 0;
                for (int i = 0; i < bytes_read / 2; i++) {
                    long current_amplitude = abs(sample_buffer[i]);
                    if (current_amplitude > peak_amplitude) {
                        peak_amplitude = current_amplitude;
                    }
                }
                float noise_level_dB = 20 * log10(peak_amplitude / 32768.0) + 94;
                if (!isnan(temp_c) && !isnan(umidade)) {
                    ccs.setEnvironmentalData(umidade, temp_c);
                }
                uint16_t eco2 = eco2_ultima;
                uint16_t tvoc = tvoc_ultima;
                bool ccs_ok = false;
                if (ccs.available()) {
                    if (!ccs.readData()) {
                        eco2 = ccs.geteCO2();
                        tvoc = ccs.getTVOC();
                        ccs_ok = true;
                    } else {
                        Serial.println("ERRO na leitura do CCS811!");
                    }
                }
                if (!isnan(temp_c) && !isnan(umidade)) {
                    temp_c_ultima = temp_c;
                    umidade_ultima = umidade;
                }
                pressaoHPa_ultima = pressaoHPa;
                if (ccs_ok) {
                    eco2_ultima = eco2;
                    tvoc_ultima = tvoc;
                }
                noise_level_dB_ultima = noise_level_dB;
                salvar_dados_json(get_laravel_timestamp());
                contador_leituras++;
                char time_log[30];
                strftime(time_log, sizeof(time_log), "%H:%M:%S", &timeinfo);
                Serial.printf("[ %s BRT] T: %.1f°C | Rec: %s (Contador: %d)\n", time_log, temp_c_ultima, "Dados salvos no LittleFS.", contador_leituras);
                delay(100);
                digitalWrite(LED_GREEN, LOW);
                pagina_atual++;
                if (pagina_atual > 2) {
                    pagina_atual = 1;
                }
            }
        }
        if (pagina_atual == 1) {
            desenha_pagina_1();
        } else {
            desenha_pagina_2();
        }
    }
    delay(10);
}
