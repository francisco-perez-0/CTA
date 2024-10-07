// Ecuación de termistor para aplicar en un mcu con ecuación de Steinhart-Hart
// temp =[A] + [B*Ln(R)] + [C*Ln(R)^3]
// Polinomio aproximado por medio de Excel 

#include <math.h>
#include <mcp2515.h>


int pins[6] = {A0, A1, A2, A3, A4, A5};

int i=0;

int xin=0; // Lectura del ADC

int estado=0; // Pin digital

int reads [6];

int read2=0;
int read3=0;
int read4=0;
int read5=0;
int read6=0;
int read7=0;

int arregloLecturas[3];

int temperaturasAux = 0;


///////////////////////////// Valores para la Ecuación///////////////////////
double xread=0; 

double temp = 0;

int led = HIGH;

double A = 0.001112386014;
double B = 0.0002400476586;
double C = 0.0000000393051678;

double logres=9.210340372;

double power = 1; // auxiliar potencia

double logrest=0;

double logtem=0;

///////////////////////// Fin valores para la ecuación///////////////////////
MCP2515 mcp2515(10);
struct can_frame trama1;
struct can_frame canMsg;


double read1=0;

void setup() {
  
  pinMode (A0, INPUT);
  pinMode (A1, INPUT);
  pinMode (A2, INPUT);
  pinMode (A3, INPUT);
  pinMode (A4, INPUT);
  pinMode (A5, INPUT);

  pinMode(3, OUTPUT);
  //analogReference(EXTERNAL);

  Serial.begin(9600);


  //**********************************
  trama1.can_id = 550; 
  trama1.can_dlc = 6;
  trama1.data[0] = 0x1E;
  trama1.data[1] = 0x28;
  trama1.data[2] = 0x32;
  trama1.data[3] = 0x3C;
  trama1.data[4] = 0x46;
  trama1.data[5] = 0x50;



  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_16MHZ);
  //aca decia 125 pero lo cambio a 250 que es la comun
  mcp2515.setNormalMode();

  //**********************************

}

void loop() {

  


///////////////////Inicio de Conversión de valor////////////////////////////
for(int i=0; i<6; i++){
  xin = analogRead(pins[i]);
  reads[i]= (int)steinh(xin);
  Serial.print("| Pin: ");
  Serial.print(pins[i]);
  Serial.print(" valor leido: ");
  Serial.print(reads[i]);
  Serial.print(" | ----- ");
}
/////////////////////Fin de Conversión de valor/////////////////////////////
  trama1.data[0] = reads[0];
  trama1.data[1] = reads[1];
  trama1.data[2] = reads[2];
  trama1.data[3] = reads[3];
  trama1.data[4] = reads[4];
  trama1.data[5] = reads[5];
  //***************************
   if (mcp2515.sendMessage(&trama1) == MCP2515::ERROR_OK) Serial.println("Messages sent");
   else {
    led = !led;
    digitalWrite(3, led);
    Serial.println("Msg1 TX error");
   }
  //**************************

  Serial.println();

/*
Serial.println();
Serial.print("La mediana es: ");
Serial.println(arregloLecturas[1]);
*/
  /*Serial.print("El valor de punto flotante es: ");
  Serial.println(read1);
  Serial.print("El valor entero sin decimales de A0 es: ");
  Serial.println(read2);
  Serial.print("El valor entero sin decimales de A1 es: ");
  Serial.println(read3);
  Serial.print("El valor entero sin decimales de A2 es: ");
  Serial.println(read4);

  Serial.print("El estado del pin 7 es: ");
  Serial.println(estado);*/

  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
      if(canMsg.can_id == 0x123){
        Serial.println("Mensaje recibido");
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
    }
   
  delay (1000);
  
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
