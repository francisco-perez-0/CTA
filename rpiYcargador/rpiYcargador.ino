
#include <Wire.h>
#include <mcp2515.h>
 
int l1 = 0;
int volt = 0;
int amp = 0;
char voltHexa[4];
char lowVolt[3];
char highVolt[3];
char ampHexa[4];
char lowAmp[3];
char highAmp[3];
bool dato = false;
bool tengoVolt = false;
bool tengoAmp = false;
bool parar = false;

struct can_frame canMsg1;
struct can_frame canMsg;
MCP2515 mcp2515(10);
 
void setup() {
  //0x08 nombre del sclavo, de este arduino
  Wire.begin(0x08);
  Serial.begin(9600);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);
  mcp2515.setNormalMode();
}

void mandarTramaAlCargador(){
  sprintf(voltHexa, "%X", volt*10);
  if (volt*10 < 255){
    highVolt[0] = '0';
    highVolt[1] = '0';
    highVolt[2] = '\0';
    lowVolt[0] = voltHexa[0];
    lowVolt[1] = voltHexa[1];
    lowVolt[2] = '\0';
  }
  
  else {
    highVolt[0] = '0';
    highVolt[1] = voltHexa[0];
    highVolt[2] = '\0';
    lowVolt[0] = voltHexa[1];
    lowVolt[1] = voltHexa[2];
    lowVolt[2] = '\0';

    unsigned char hexValue = (unsigned char)strtol(highVolt, NULL, 16) & 0xFF;
    if (hexValue < 0x10) {
      Serial.println(hexValue);
      hexValue = (hexValue << 4);
    }
    canMsg1.data[0] = hexValue;
  }

  sprintf(ampHexa, "%X", amp*10);
  if (amp*10 < 255){
    highAmp[0] = '0';
    highAmp[1] = '0';
    highAmp[2] = '\0';
    lowAmp[0] = ampHexa[0];
    lowAmp[1] = ampHexa[1];
    lowAmp[2] = '\0';
  }
  else {
    highAmp[0] = '0';
    highAmp[1] = ampHexa[0];
    highAmp[2] = '\0';
    lowAmp[0] = ampHexa[1];
    lowAmp[1] = ampHexa[2];
    lowAmp[2] = '\0';
    unsigned char hexValue = (unsigned char)strtol(highVolt, NULL, 16) & 0xFF;
    if (hexValue < 0x10) {
      Serial.println("entro a menos 10 amp");
      hexValue = (hexValue << 2) | hexValue;
    }
    canMsg1.data[2] = hexValue;
  }

  canMsg1.can_id  = 0x9806E5F4;
  canMsg1.can_dlc = 8;
  //canMsg1.data[0] = strtol(highVolt, NULL, 16) & 0xFF;
  canMsg1.data[1] = strtol(lowVolt, NULL, 16) & 0xFF;
  //canMsg1.data[2] = strtol(highAmp, NULL, 16) & 0xFF;
  canMsg1.data[3] = strtol(lowAmp, NULL, 16) & 0xFF;
  canMsg1.data[4] = 0x00;
  canMsg1.data[5] = 0x00;
  canMsg1.data[6] = 0x00;
  canMsg1.data[7] = 0x00;
  //mcp2515.sendMessage(MCP2515::TXB1, &canMsg1);
  Serial.println(canMsg1.data[0]);
  //Serial.println(canMsg1.data[1]);
  //Serial.println(canMsg1.data[2]);
  //Serial.println(canMsg1.data[3]);
}

void requestEvent(){

}
 
// el b no se usa pero representa los byte que se reciben, es la firma de la funcion tiene que estar
void receiveEvent(int b) {
  while (Wire.available()) {
    int c = Wire.read(); 
    if (not dato){
        if (c == 1){
          //si se leyo 1 el prox dato entonces es el volt
          l1 = 1;
          dato = true;
          parar = false;
          Serial.println("proximo dato a leer volt");
        }
        if (c == 2){
          //si es 2 entoncces el proximo dato son los amp
          l1 = 2;
          dato = true;
          parar = false;
          Serial.println("proximo dato a leer amp");
        }
        if (c == 10){
          Serial.println("dejo de cargar");
          tengoVolt = false;
          tengoAmp = false;
          dato = false;
          parar = true;
        }
    }
    else {
      if (l1 == 1){
        volt = c;
        dato = false;
        tengoVolt = true;
        Serial.println(volt);
      }
      if (l1 == 2){
        amp = c;
        dato = false;
        tengoAmp = true;
        Serial.println(amp);
      }
    }
    
  }
}
void loop() {
  Wire.onReceive(receiveEvent);
  if (tengoVolt && tengoAmp && not parar){
    mandarTramaAlCargador();
  }
  delay(100);
}
