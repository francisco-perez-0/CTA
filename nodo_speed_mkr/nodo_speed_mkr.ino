#include <SoftwareSerial.h>
#include <mcp2515.h>
#include <movingAvg.h>

// CAN
MCP2515 mcp2515(10);
struct can_frame tramaSpeed;


double metrosSegundos;
int parteEntera;
int parteDecimal;
int ledRojo = 5;
int ledVerde = 6;


// HALL
movingAvg avg(5);
int hallPin = 3;
volatile int contador=0;
double radio=0.3;
double rpm;
double speed;
double promedio;
int intervalSpeed = 500;
int tiempoPrevioSpeed=0;
int counterDelaySpeed = 0;

int errorLed = LOW;



void setup()
{

  avg.begin();
  attachInterrupt(digitalPinToInterrupt(hallPin), Counter, FALLING);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  digitalWrite(ledVerde, HIGH);
  tramaSpeed.can_id = 910; 
  tramaSpeed.can_dlc = 2;
  tramaSpeed.data[0] = 0x00;
  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_16MHZ);
  mcp2515.setNormalMode();
  //aca decia 125 pero lo cambio a 250 que es la

}


void loop()
{  
  
  interrupts();
  if(counterDelaySpeed++ == 500){
    //tiempoPrevioSpeed = tiempoActualSpeed;
    counterDelaySpeed = 0;
    noInterrupts();
    rpm=(contador*60.0)/0.5;
    speed = (rpm*M_PI*radio*2.0)/60.0;
    avg.reading(speed);
    //Serial.print("speed: ");
    //Serial.println(speed);
    promedio = avg.getAvg();
    int entero = promedio;
    int decimal = (promedio-entero)*100;
    //parteDecimal = (int)((metrosSegundos - parteEntera)*100);
    tramaSpeed.data[0] = entero;
    tramaSpeed.data[1] = decimal;
    //***************************
    if (mcp2515.sendMessage(&tramaSpeed) == MCP2515::ERROR_OK){
      digitalWrite(ledRojo, LOW);
    } else {
      digitalWrite(ledRojo, HIGH);
    }
    contador = 0;
  }


  delay(1);
  
}
void Counter() {
  contador++;
}