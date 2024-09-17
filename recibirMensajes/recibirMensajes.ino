#include <mcp2515.h>
#define RELAY1 8
#define RELAY2 9
#define analogico A0

MCP2515 mcp2515(10);


struct can_frame trama1;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
   trama1.can_id = 550; 
  trama1.can_dlc = 6;
  trama1.data[0] = 0x1E;
  trama1.data[1] = 0x28;
  trama1.data[2] = 0x32;
  trama1.data[3] = 0x3C;
  trama1.data[4] = 0x46;
  trama1.data[5] = 0x50;

  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (mcp2515.readMessage(&trama1) == MCP2515::ERROR_OK) {
      if(trama1.can_id == 0x123){
        Serial.println("Mensaje recibido");
        Serial.print(trama1.can_id, HEX); // print ID
        Serial.print(" "); 
        Serial.print(trama1.can_dlc, HEX); // print DLC
        Serial.print(" ");
        for (int i = 0; i<trama1.can_dlc; i++)  {  // print the data
          Serial.print(trama1.data[i],HEX);
          Serial.print(" ");
        }
        Serial.println();      
      }
    }

      //***************************
   if (mcp2515.sendMessage(&trama1) == MCP2515::ERROR_OK) Serial.println("Messages sent");
   else Serial.println("Msg1 TX error");
  //**************************

}
