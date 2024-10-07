#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <mcp2515.h>
SoftwareSerial ss(3,4);

// CAN
MCP2515 mcp2515(10);
struct can_frame tramaSpeed;

TinyGPSPlus gps;
char dato=' ';

double metrosSegundos;
int parteEntera;
int parteDecimal;


void setup()
{
  Serial.begin(9600);            
  ss.begin(9600); 

  tramaSpeed.can_id = 910; 
  tramaSpeed.can_dlc = 2;
  tramaSpeed.data[0] = 0x00;

  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  //aca decia 125 pero lo cambio a 250 que es la

}


void loop()
{
while (ss.available() > 0) {
    gps.encode(ss.read());
    // Velocidad en km/h
    if (gps.speed.isUpdated()) {
      metrosSegundos = gps.speed.kmph();
      Serial.print("Velocidad (m/s): ");
      metrosSegundos*=0.2778;
      parteEntera = metrosSegundos;
      parteDecimal = (int)((metrosSegundos - parteEntera)*100);
      Serial.print(parteEntera);
      Serial.print(".");
      Serial.println(parteDecimal);
      tramaSpeed.data[0] = parteEntera;
      tramaSpeed.data[1] = parteDecimal;
      //***************************
      if (mcp2515.sendMessage(&tramaSpeed) == MCP2515::ERROR_OK);
      }

  }
}
