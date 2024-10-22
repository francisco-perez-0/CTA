

#include  <SoftwareSerial.h>
SoftwareSerial sim5320(3,4);
#include <mcp2515.h>

// CAN
MCP2515 mcp2515(10);
struct can_frame trama1;

String lon = "";
String speed = "";

float metrosSegundos=0.0;
int parteEntera;
int parteDecimal;


#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];


String sentence = "";
int commaCount = 0;
bool latOrlong = true;
bool trash = true;
bool finish = false;

int i1;
int i2;
int i3;
int i4;
int i5;
int i6;
int i7;
int i8;
int i9;

void setup() {
  // put  your setup code here, to run once:
  //Serial.begin(4800);
  sim5320.begin(4800);

  trama1.can_id = 910; 
  trama1.can_dlc = 2;
  trama1.data[0] = 0x00;

  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);
  //aca decia 125 pero lo cambio a 250 que es la comun
  mcp2515.setNormalMode();

  delay(5000);
  sim5320.write("AT+CGPSINFO=1\r\n");
}

int flag = 1;

void  loop() {
  /*if(flag) {
    Serial.println("Escriba los comando AT que desee: ");
    flag=0;
  }
  static String lat = "";
  if (Serial.available()) {
    String atCommand = Serial.readStringUntil('\n');
    sim5320.println(atCommand);
    flag=1;
  }*/
  if (sim5320.available()) {
    while (sim5320.available()) {
      char c = sim5320.read();
      //Serial.print(c);
      sentence.concat(c);
  }
  i1 = sentence.indexOf(':');
  i2 = sentence.indexOf(',');
  i3 = sentence.indexOf(',', i2+1);
  i4 = sentence.indexOf(',', i3+1);
  i5 = sentence.indexOf(',', i4+1);
  i6 = sentence.indexOf(',', i5+1);
  i7 = sentence.indexOf(',', i6+1);
  i8 = sentence.indexOf(',', i7+1);
  i9 = sentence.indexOf(',', i8+1);
  //                                                             |         |
  //                                                             v = i7+1  v = i8
  // [<lat>],[<N/S>],[<log>],[<E/W>],[<date>],[<UTCtime>],[<alt>],[<speed>],[<course>]  
  speed = sentence.substring(i7+1, i8);
  //lat = sentence.substring(i1+1, i2);
  metrosSegundos = speed.toFloat();
  metrosSegundos/=1.944;
  parteEntera = metrosSegundos;
  parteDecimal = (int)((metrosSegundos - parteEntera)*100);

  trama1.data[0] = parteEntera;
  trama1.data[1] = parteDecimal;
   //***************************
  if (mcp2515.sendMessage(&trama1) == MCP2515::ERROR_OK);

  /**************************
  Serial.println();
  Serial.print("Velocidad en m/s: ");
  Serial.print(parteEntera);
  Serial.print(",");
  Serial.println(parteDecimal); 
   //Serial.println(sentence.substring(i3+1, i4));
   //sentence.substring(i1+1, i2).toCharArray(msg, MSG_BUFFER_SIZE);
   //Serial.println(msg);*/
   delay(1000);
   sentence = "";
}
}
