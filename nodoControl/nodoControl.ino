#include <mcp2515.h>
#include <EEPROM.h>

#define RELAY1 7
#define RELAY2 8
#define RELAY3 9

#define pinCorriente A0
#define pinTension A1

#define sensibilidad 0.0125
#define Vref 5.0
#define Vcero 2.5


const unsigned long interval = 1000; 
unsigned long previousMillis = 0;    

MCP2515 mcp2515(10);
struct can_frame trama1;
struct can_frame tramaCorriente;
struct can_frame tramaCorrienteN;
struct can_frame tramaTension;
struct can_frame canMsg;

int i=0;
int j=0;


int estado=0; // Pin digital

int led = HIGH;

int read2=0;
int read3=0;
int read4=0;

double read1=0;

long tension;
int parteEnteraT;
int parteFlotanteT;
float convTension = 0;


long corriente;
int parteEnteraC;
int parteFlotanteC;
float convCorriente = 0;


int flag = 0;
int flagNegativo=0;
int val;

int estadoRelayUno;
int estadoRelayDos;
int estadoRelayTres;


void setup() {

  pinMode (pinCorriente, INPUT);
  pinMode (pinTension, INPUT);
  // put your setup code here, to run once:
  estadoRelayUno = EEPROM.read(estadoRelayUno);
  estadoRelayDos = EEPROM.read(estadoRelayDos);
  estadoRelayTres = EEPROM.read(estadoRelayTres);
  pinMode(3, OUTPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  Serial.begin(9600);
  //digitalWrite(7, LOW);
  //digitalWrite(8, LOW);
  //digitalWrite(9, LOW);

  tramaCorriente.can_id = 880; 
  tramaCorriente.can_dlc = 2;
  tramaCorriente.data[0] = 0x00;
  tramaCorriente.data[1] = 0x00;

  tramaCorrienteN.can_id = 879; 
  tramaCorrienteN.can_dlc = 2;
  tramaCorrienteN.data[0] = 0x00;
  tramaCorrienteN.data[1] = 0x00;

  tramaTension.can_id = 890; 
  tramaTension.can_dlc = 2;
  tramaTension.data[0] = 0x00;
  tramaTension.data[1] = 0x00;


  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);
  //aca decia 125 pero lo cambio a 250 que es la comun
  mcp2515.setNormalMode();
}

void loop() {
  uint32_t t = millis();
  if(flag){
    led = !led;
    digitalWrite(3, led);
    /*
    Serial.print("Estado pin 4 - ");
    val = digitalRead(4);
    Serial.println(val);
    Serial.print("Estado pin 5 - ");
    val = digitalRead(5);
    Serial.println(val);
    Serial.print("Estado pin 6 - ");
    val = digitalRead(6);
    Serial.println(val);
    
    Serial.print("Conversion corriente ");
    Serial.print(parteEnteraC);
    Serial.print(" , ");
    Serial.print(parteFlotanteC);
    Serial.println();*/
    flag=0;
  }
  // ------------------- Corriente -------------------
  corriente = analogRead(A0);
  float corrienteAux = ((corriente*Vref)/1023); 
  convCorriente = funcionConversionCorriente(corrienteAux);
  if(convCorriente < 0){
    flagNegativo = 1;
    convCorriente*=-1;
  }
  parteEnteraC = (int)convCorriente;
  parteFlotanteC = (int)((convCorriente - parteEnteraC) * 100); 
  //convCorriente = mapFloat(corriente, 0, 1023, 0, 50);
  
  // ------------------- Tension -------------------
  tension = analogRead(A1); 
  //convTension = mapFloat(tension, 0, 1023, 0, 100);
  float ope = tension*100;
  float ope1 = ope/1023;
  parteEnteraT = (int)ope1;
  parteFlotanteC = (int)((ope1 - parteEnteraT)*100);

  if(flagNegativo == 0){
    //Envio trama de corriente
    tramaCorriente.data[0] = parteEnteraC;
    tramaCorriente.data[1] = parteFlotanteC;
    //***************************
    if (mcp2515.sendMessage(&tramaCorriente) == MCP2515::ERROR_OK);
    else {
      Serial.println("MsgCorriente TX error");
      flag=1;
      }
    //**************************
  } else {
    //Envio trama de corriente
    tramaCorrienteN.data[0] = parteEnteraC;
    tramaCorrienteN.data[1] = parteFlotanteC;
    //***************************
    if (mcp2515.sendMessage(&tramaCorrienteN) == MCP2515::ERROR_OK);
    else{
      Serial.println("MsgCorrienteN TX error");
      flag=1;
    } 
    //**************************
  }

   //Envio trama de tension
  tramaTension.data[0] = parteEnteraT;
  tramaTension.data[1] = parteFlotanteT;
  //***************************
  if (mcp2515.sendMessage(&tramaTension) == MCP2515::ERROR_OK);
  else {
    flag=1;
    Serial.println("MsgTension TX error");
  }
  //**************************

  //Recibo de tramas de control 
  if (mcp2515.readMessage(&trama1) == MCP2515::ERROR_OK) {
    if(trama1.can_id == 0x123){
      Serial.println("Mensaje recibido");
      Serial.print(trama1.can_id, HEX); // print ID
      Serial.print(" "); 
      Serial.print(trama1.can_dlc, HEX); // print DLC
      Serial.print(" ");
      switch (trama1.data[0]){
        case 0x00:
          digitalWrite(RELAY1, LOW);
          EEPROM.write(estadoRelayUno, 0);
          break;
        case 0x11:
          digitalWrite(RELAY1, HIGH);
          EEPROM.write(estadoRelayUno, 1);
          break;
        case 0x22:
          digitalWrite(RELAY2 , LOW);
          EEPROM.write(estadoRelayDos, 0);
          break;
        case 0x33:
          digitalWrite(RELAY2, HIGH);
          EEPROM.write(estadoRelayDos, 1);
          break;
        
      }
      if(trama1.data[0] == 0x44){
        digitalWrite(RELAY3, LOW);
        EEPROM.write(estadoRelayTres, 0);
      } else if(trama1.data[0] == 0x55){
        digitalWrite(RELAY3, HIGH);
        EEPROM.write(estadoRelayTres, 1);
      }
      for (int i = 0; i<trama1.can_dlc; i++)  {  // print the data
        Serial.print(trama1.data[i],HEX);
        Serial.print(" ");
      }
      Serial.println();      
    }
  }

  flagNegativo = 0;
  delay(5);
}


float funcionConversionCorriente(float entrada){
  return ((entrada - 2.5)/0.0125);
}

float mapFloat(int input, int min, int max, int minW, int maxW){
  float res = (minW + (input / 1023.0)*(maxW - minW));
  return res;
}
