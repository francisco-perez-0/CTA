#include <mcp2515.h>

#define RELAY1 6
#define RELAY2 7
#define RELAY3 8

MCP2515 mcp2515(10);
struct can_frame trama1;
struct can_frame canMsg;

int i=0;

int xin=0; // Lectura del ADC

int estado=0; // Pin digital

int read2=0;
int read3=0;
int read4=0;
double read1=0;


int arregloLecturas[3];

int temperaturasAux = 0;
///////////////////////////// Valores para la Ecuación///////////////////////
double xread=0; 

double temp = 0;


double A = 0.001112386014;
double B = 0.0002400476586;
double C = 0.0000000393051678;

double logres=9.210340372;

double power = 1; // auxiliar potencia

double logrest=0;

double logtem=0;

void setup() {

pinMode (A0, INPUT);
pinMode (A1, INPUT);
pinMode (A2, INPUT);
  // put your setup code here, to run once:
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(9, INPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  Serial.begin(9600);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);

  trama1.can_id = 550; 
  trama1.can_dlc = 8;
  trama1.data[0] = 0x1E;
  trama1.data[1] = 0x28;
  trama1.data[2] = 0x32;
  trama1.data[3] = 0x3C;
  trama1.data[4] = 0x46;
  trama1.data[5] = 0x50;
  trama1.data[6] = 0x5A;
  trama1.data[7] = 0x64;

  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);
  //aca decia 125 pero lo cambio a 250 que es la comun
  mcp2515.setNormalMode();
}

void loop() {
  temperaturasAux = 0;
xin=analogRead(A0);
read1 = steinh (xin); 
read2 = (int)read1;

arregloLecturas[0] = read2;

xin=analogRead(A1);
read1 = steinh (xin);
read3 = (int)read1;

arregloLecturas[1] = read3;

xin=analogRead(A4);
read1 = steinh (xin);
read4 = (int)read1;

arregloLecturas[2] = read4;


SortAsc(arregloLecturas);
/*for(i=0;i<3;i++){
    Serial.print(arregloLecturas[i]);
    Serial.print(" - ");
}*/
  int flag = 0;
  int val;
  /*val = analogRead(A0);  
    Serial.print("Estado potenciometro - "); 
    Serial.println(val);
  Serial.print("Estado pin 4 - ");
    val = digitalRead(4);
    Serial.println(val);

    Serial.print("Estado pin 9 - ");
    val = digitalRead(9);
    Serial.println(val);
    delay(1000);*/
  // put your main code here, to run repeatedly:
  if (mcp2515.readMessage(&trama1) == MCP2515::ERROR_OK) {
    if(trama1.can_id == 0x123){
      Serial.println("Mensaje recibido");
      Serial.print(trama1.can_id, HEX); // print ID
      Serial.print(" "); 
      Serial.print(trama1.can_dlc, HEX); // print DLC
      Serial.print(" ");
      switch (trama1.data[0]){
        case 0x00:
          digitalWrite(6, LOW);
          break;
        case 0x11:
          digitalWrite(6, HIGH);
          break;
        case 0x22:
          digitalWrite(7, LOW);
          break;
        case 0x33:
          digitalWrite(7, HIGH);
          break;
        
      }
      if(trama1.data[0] == 0x44){
        digitalWrite(8, LOW);
      } else if(trama1.data[0] == 0x55){
        digitalWrite(8, HIGH);
      }
      for (int i = 0; i<trama1.can_dlc; i++)  {  // print the data
        Serial.print(trama1.data[i],HEX);
        Serial.print(" ");
      }
      Serial.println();      
    }
  }
  trama1.data[0] = 0x0;
  trama1.data[1] = 0x0;
  trama1.data[2] = 0x0;
  trama1.data[3] = 0x0;
  trama1.data[4] = arregloLecturas[1];
  trama1.data[5] = 0x0;
  trama1.data[6] = 0x0;
  trama1.data[7] = 0x0;
  //***************************
   if (mcp2515.sendMessage(&trama1) == MCP2515::ERROR_OK) Serial.println("Messages sent");
   else Serial.println("Msg1 TX error");
  //**************************
  temperaturasAux = 0;
  delay(1000);
}


double steinh (int adc){
  xread= (1024.0f/adc)-1;

  logrest =-log(xread)+logres;
  /////////////////////////Steinhart-Hart//////////////////////////////////////

    power =1;
    for(i=0;i<3;i++){
      power= power*logrest;
    }

  temp = A + B*logrest + C*power;
  temp = 1.0f/temp;
  temp = temp - 273.15f;
   /////////////////////////Fin Steinhar-Hart//////////////////////////////////
  return temp;
}

//////////////////////////Ordenamiento ascendente/////////////////////////////
void SortAsc(int* arr){
  int aux;
    //Comparo si el primer numero es mayor al segundo y luego si el segundo es mayor al tercero
    for(i=0;i<2;i++){
      if(arr[i] > arr[i+1]){
      aux=arr[i+1];
      arr[i+1]=arr[i];
      arr[i] = aux;
    }
  }
  //Comparo si el segundo es menor al segundo en el caso de que haya switcheado arriba
  if(arr[1] < arr[0]){
      aux=arr[0];
      arr[0]=arr[1];
      arr[1] = aux;
  }
}
