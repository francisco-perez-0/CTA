  #include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg1;
struct can_frame canMsg;
MCP2515 mcp2515(10);


void setup() {
  canMsg1.can_id  = 0x9806E5F4; //si no anda con eso cambiar el primer 9 por un 1
  canMsg1.can_dlc = 8;
  canMsg1.data[0] = 0x03;//maximo de 83 volt
  canMsg1.data[1] = 0x3E;
  canMsg1.data[2] = 0x00;
  canMsg1.data[3] = 0x96;//carga a 15 ampr
  canMsg1.data[4] = 0x00;
  canMsg1.data[5] = 0x00;
  canMsg1.data[6] = 0x00;
  canMsg1.data[7] = 0x00;
  
  while (!Serial);
  Serial.begin(9600);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);
  mcp2515.setNormalMode();
}

void loop() {
  mcp2515.sendMessage(&canMsg1);
  //mcp2515.sendMessage(MCP2515::TXB1, &canMsg1); //en esta linea se esta aclarando qué buffer
  //de transmision se va a usar
  
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    if (canMsg.can_id == 0x98FF50E5){
          Serial.print(canMsg.can_id, HEX); // print ID
          Serial.print(" "); 
          Serial.print(canMsg.can_dlc, HEX); // print DLC
          Serial.print(" ");
          
          for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
            Serial.print(canMsg.data[i],HEX);
            Serial.print(" ");
          }
        Serial.println();
    }
    if (canMsg.can_id == 0x148){
//        canMsg1.can_id  = 0x9806E5F4;
//        canMsg1.can_dlc = 8;
//        canMsg1.data[0] = canMsg.data[0];
//        canMsg1.data[1] = canMsg.data[1];
//        canMsg1.data[2] = canMsg.data[2];
//        canMsg1.data[3] = canMsg.data[3];
//        canMsg1.data[4] = canMsg.data[4];
//        canMsg1.data[5] = canMsg.data[5];
//        canMsg1.data[6] = canMsg.data[6];
//        canMsg1.data[7] = canMsg.data[7];

        //mcp2515.sendMessage(MCP2515::TXB1, &canMsg1);
        Serial.println("lee el id de la raspberry");
    }
  }
  
  delay(1000);
}
