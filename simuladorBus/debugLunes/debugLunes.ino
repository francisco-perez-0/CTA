#include <mcp2515.h>
#include <movingAvg.h>
#define sensibilidad 0.0125
#define Vref 5.0
#define Vcero 2.5

movingAvg avg(5);
int hallPin = 3;
volatile int contador=0;
double radio=0.3;
double rpm;
double speed;
int promedio;

MCP2515 mcp2515(10);
struct can_frame trama1;
struct can_frame tramaCorriente;
struct can_frame tramaTension;
struct can_frame tramaSpeed;
struct can_frame canMsg;


float metrosSegundos=0.0;
int parteEntera;
int parteDecimal;

long tension;
int parteEnteraT;
int parteFlotanteT;
float convTension = 0;
int flag = 1;
int flagC = 1; 

int i = 0;
int j = 0;
int k = 0;
int h = 0;

long corriente;
int parteEnteraC;
int parteFlotanteC;
float convCorriente = 0;

float ope = 78.0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  avg.begin();
  attachInterrupt(digitalPinToInterrupt(hallPin), Counter, RISING);

  //-------------Trama corriente-------------
  tramaCorriente.can_id = 880;
  tramaCorriente.can_dlc = 1;
  tramaCorriente.data[0] = 0x00;

  //-------------Trama Tension-------------
  tramaTension.can_id = 890;
  tramaTension.can_dlc = 2;
  tramaTension.data[0] = 0x00;
  tramaTension.data[1] = 0x00;
  //-------------Trama speed-------------
  tramaSpeed.can_id = 910;
  tramaSpeed.can_dlc = 2;
  tramaSpeed.data[0] = 0x00;
  tramaSpeed.data[1] = 0x00;

  mcp2515.reset();
  //Tener cuidado porque gise me comento algo de 16MHZ
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);
  //aca decia 125 pero lo cambio a 250 que es la comun
  mcp2515.setNormalMode();

}



void loop() {
  // put your main code here, to run repeatedly:
  // ------------------- Corriente -------------------
  if(flagC){
    corriente = random(14, 15);

  } else { 
    corriente = random(0, 3);
  }
  float corrienteAux = ((corriente*Vref)/1023); 
  convCorriente = (corrienteAux-2.50)*100.0;
  if (convCorriente < 0) {
    //flagNegativo = 1;
    convCorriente *= -1;
  }
  //parteEnteraC = (int)convCorriente;
  //parteFlotanteC = (int)((convCorriente - parteEnteraC) * 100);
  //convCorriente = mapFloat(corriente, 0, 1023, 0, 50);
  


  // ------------------- Tension -------------------
  //tension = random(60,1023);
  //convTension = mapFloat(tension, 0, 1023, 0, 100);
  if (flag) {
    if (k++ == 1000) {
      k = 0;
      ope += 0.1 ;//(tension*100.0)/1023.0;
      parteEnteraT = ope;
      parteFlotanteT = ((ope - parteEnteraT) * 100.0);
      Serial.println(ope,2);
    }
  }
  // ------------------- Tension -------------------
  metrosSegundos = random(0,1);
  parteEntera = metrosSegundos;
  parteDecimal = (metrosSegundos - parteEntera)*100.0; 

  if(h++ == 100){
    h=0;
  //Envio trama de corriente
    tramaSpeed.data[0] = parteEntera;
    tramaSpeed.data[1] = parteDecimal;
    //***************************
    if (mcp2515.sendMessage(&tramaSpeed) == MCP2515::ERROR_OK);
    else {
      Serial.println("MsgCorrienteN TX error");
    }
    //**************************
  }


  //Debug
  /*Serial.print("Conversion tension ");
    Serial.print(parteEnteraT);
    Serial.print(",");
    Serial.print(parteFlotanteT);
    Serial.print(" || ");
    Serial.print("Conversion Corriente ");
    Serial.print(parteEnteraC);
    Serial.print(",");
    Serial.print(parteFlotanteC);
    Serial.println();*/

 /* if (j++ == 5) {
    j = 0;
    //Envio trama de corriente
    tramaCorriente.data[0] = parteEnteraC;
    tramaCorriente.data[1] = parteFlotanteC;
    //***************************
    if (mcp2515.sendMessage(&tramaCorriente) == MCP2515::ERROR_OK);
    else {
      Serial.println("MsgCorrienteN TX error");
    }
    //**************************
  }*/


  if (i++ == 100) {
    i = 0;
    //Envio trama de tension
    tramaTension.data[0] = parteEnteraT;
    tramaTension.data[1] = parteFlotanteT;
    //***************************
    if (mcp2515.sendMessage(&tramaTension) == MCP2515::ERROR_OK);
    else {
      Serial.println("MsgTension TX error");
    }
  }
  if (mcp2515.readMessage(&trama1) == MCP2515::ERROR_OK) {
    if (trama1.can_id == 0x123) {
      Serial.println("Mensaje recibido");
      Serial.print(trama1.can_id, HEX); // print ID
      Serial.print(" ");
      Serial.print(trama1.data[0]);
      Serial.print(" ");
      if (trama1.data[0] == 2) {
        //flag = 0;
        Serial.println("Corto cargador");
        flag=0;
        flagC=0;
        tramaTension.data[0] = ope;
        tramaTension.data[1] = 0;
        if (mcp2515.sendMessage(&tramaTension) == MCP2515::ERROR_OK);
        else {
          Serial.println("MsgTension TX error");
        }
      } else if (trama1.data[0] == 3) {
        flag = 1;
        flagC=1;
        Serial.println("Empiezo a cargar");
      }
    }
        tramaTension.data[0] = ope;
        tramaTension.data[1] = 0;
        if (mcp2515.sendMessage(&tramaTension) == MCP2515::ERROR_OK){
          Serial.println("Mando mensaje");
        }
        else {
          Serial.println("MsgTension TX error");
      

        }
        
  }
        tramaCorriente.data[0] = corriente;
        if (mcp2515.sendMessage(&tramaCorriente) == MCP2515::ERROR_OK){

        }
        delay(1);

        contador = 0;
    interrupts();
    delay(100);
    noInterrupts();
    rpm=(contador*60.0)/0.1;
    speed = (rpm*M_PI*radio*2.0)/60.0;
    avg.reading(speed);
    Serial.print("RPM: ");
    Serial.println(rpm);
    promedio = avg.getAvg();
    tramaSpeed.data[0] = rpm/100;
    if (mcp2515.sendMessage(&tramaSpeed) == MCP2515::ERROR_OK){

        }
}

float funcionConversionCorriente(float entrada) {
  return ((entrada - 2.5) / 0.0125);
}

void Counter() {
  contador++;
}