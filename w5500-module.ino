#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#define ETH_CS_PIN 5 
#define ETH_RST_PIN 2

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

const char* ntpServerName = "time.cloudflare.com"; 

const long timeOffset = -10800; 

unsigned int localPort = 2390;

EthernetUDP Udp;
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  Serial.println("Inicializando Ethernet com W5500...");

  pinMode(ETH_RST_PIN, OUTPUT);
  digitalWrite(ETH_RST_PIN, LOW);
  delay(100);
  digitalWrite(ETH_RST_PIN, HIGH);
  delay(100);

  Ethernet.init(ETH_CS_PIN);
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Falha ao configurar Ethernet usando DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Erro: W5500 nao detectado. Verifique as ligacoes SPI e o pino CS.");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Cabo de rede desconectado.");
    }
  }
  
  Serial.print("Endereço IP: ");
  Serial.println(Ethernet.localIP());

  Udp.begin(localPort);
  Serial.println("UDP iniciado. Solicitando hora NTP...");
}

void loop() {
  static unsigned long lastTimeCheck = 0;
  if (millis() - lastTimeCheck > 10000) {
    sendNTPpacket(ntpServerName);
    lastTimeCheck = millis();
  }

  unsigned long timeReceived = receiveNTPtime();
  if (timeReceived > 0) {
    const unsigned long seventyYears = 2208988800UL; 
    unsigned long epoch = timeReceived - seventyYears;
    
    epoch += timeOffset; 

    unsigned long hours = (epoch  % 86400L) / 3600;
    unsigned long minutes = (epoch % 3600) / 60;
    unsigned long seconds = epoch % 60;

    Serial.print("Hora atual (UTC ajustada): ");
    
    if (hours < 10) Serial.print('0');
    Serial.print(hours);
    Serial.print(':');
    if (minutes < 10) Serial.print('0');
    Serial.print(minutes);
    Serial.print(':');
    if (seconds < 10) Serial.print('0');
    Serial.println(seconds);
  }

  Ethernet.maintain();
}

void sendNTPpacket(const char* address) {
  memset(packetBuffer, 0, NTP_PACKET_SIZE);

  packetBuffer[0] = 0b11100011;

  Udp.beginPacket(address, 123);
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
  Serial.print("Pacote NTP enviado para: ");
  Serial.println(address);
}

unsigned long receiveNTPtime() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Udp.read(packetBuffer, NTP_PACKET_SIZE);
    
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);

    unsigned long secsSince1900 = highWord << 16 | lowWord;
    return secsSince1900;
  }
  return 0;
}
