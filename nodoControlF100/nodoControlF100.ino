#include <mcp2515.h>

#define RELAY1 7
#define RELAY1 8
#define RELAY1 9

#define corteSOC 95

MCP2515 mcp2515(10);
struct can_frame tramaCorte;

void setup() {
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  Serial.begin(9600);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_16MHZ);
  mcp2515.setNormalMode();
}

void loop() {
  
  if(mcp2515.readMessage(&tramaCorte) == MCP2515::ERROR_OK){
    if(tramaCorte.can_id == 0x301){
      Serial.println("Mensaje recibido");
      Serial.print(trama1.can_id, HEX); // print ID
      Serial.print(" "); 
      Serial.print(trama1.can_dlc, HEX); // print DLC
      Serial.print(" ");
      int SOC = (tramaCorte.data[0])/2; 
      if(SOC > 95){
        digitalWrite(RELAY1, LOW);
      }
      if(SOC < 90){
        digitalWrite(RELAY1, HIGH); //Ver despues si es NA o NC
      }
    }
  }

}
