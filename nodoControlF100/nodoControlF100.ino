#include <mcp2515.h>

#define RELAY1 5
#define RELAY2 6
#define RELAY3 9

MCP2515 mcp2515(10);
struct can_frame tramaCorte;

bool releActivo = true;

void setup() {
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  Serial.begin(9600);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();

  digitalWrite(RELAY1, HIGH); 
}

void loop() {
  if (mcp2515.readMessage(&tramaCorte) == MCP2515::ERROR_OK) {
    if (tramaCorte.can_id == 0x301 && tramaCorte.can_dlc >= 1) {
      int SOC = tramaCorte.data[0]; // SOC en 0-100
      Serial.print("SOC: "); Serial.println(SOC);

      if (SOC >= 95 && releActivo) {
        digitalWrite(RELAY1, LOW);
        releActivo = false;
        Serial.println("Carga cortada");
      }

      if (SOC <= 90 && !releActivo) {
        digitalWrite(RELAY1, HIGH); 
        releActivo = true;
        Serial.println("Carga habilitada");
      }
    }
  }
}
