#include <math.h>
#include <LiquidCrystal.h>

int valor=0;
int valor1=0;
int temperatura;
int temperatura1;
int temperatura2;
bool maxc=0;
/////////////// Inicialización Display ////////////////////////////////////
//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int rs = 6, en = 5, d4 = A0, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
////////////////////// Fin Init Display ///////////////////////////////////

///////////////////////////// Valores para la Steinh///////////////////////
double xread=0; 
double temp = 0;
int temp1=0;

double A = 0.001112386014;
double B = 0.0002400476586;
double C = 0.0000000393051678;

double logres=9.210340372;
double power = 1; // auxiliar potencia
double logrest=0;
double logtem=0;

////////////////////// Fin valores para la Steinh //////////////////////////

///////////////////// Función para Med. Termistores ///////////////////////
double steinh (int adc, bool a){
  xread= (1024.0f/adc)-1;
  logrest =-log(xread)+logres;
  power =1;
  for(int i=0;i<3;i++){
    power= power*logrest;
    }
  temp = A + B*logrest + C*power;
  temp = 1.0f/temp;
  temp = temp - 273.15f;
  if (a==1){
    temp1= (int)temp;
    lcd.print (temp1);
    return 0;
    }
    return temp;
}
/////////////////////Fin Función Steinhar-Hart////////////////////////////

///////////////// Función para conversiòn de tensión /////////////////////
/*Esta función incluye visualización en pantalla si se habilita*/
int voltm (int v, int vmax, bool s){
  unsigned long val;
  if (vmax>=100){
    vmax=vmax/10;
    val=v*vmax;
    val=val*10;
    
  }
  if (vmax<100){
  val=v*vmax;
  }
  val=val*10;
  val=val/1010;
  int val1=val/100;
  int val2=val%100;
  int val3=val2%10;
  val2=val2/10;
  
  
  if (s==1){
    lcd.print (val1);
    lcd.print (val2);
    lcd.print (",");
    lcd.print (val3);
    return 0;
    }
    return val;
  }
/////////////////// Fin Conversión de Tensión //////////////////

/////////////////// Conversión de Corriente Hall////////////////////

float corrh (int ci, bool i){
  //float valc=ci*rng;
  float valc=ci*5;
  valc=valc/1023;
  valc=(valc-2.52);
  valc=valc*10;
   
  if (i==1){
    lcd.print (valc);
    return 0;
    }
    return valc;
  }

////////////////// Fin Conversion de Corriente Hall/////////////////

///////////////// Corriente Puente /////////////////////////////

float corr (int ci, bool max, bool i){
  float valc=ci*5;
  valc=valc/1023;
  if (max==0){
    valc=valc/1.14;
  }
  if (max==1){
    valc=valc/0.4;
  }
    
  if (i==1){
    lcd.print (valc);
    return 0;
    }
    return valc;
  }


//////////////// Fin Corriente Puente /////////////////////////////


void setup() {
  
  Serial.begin(9600);
  lcd.begin(16, 2); 
  pinMode(A1,INPUT);
  pinMode(A0, OUTPUT);
  //pinMode(6,INPUT);
  lcd.setCursor(0,0);
}

void loop() {
  

 
  temperatura1= analogRead(A3);
  delay(500);
  temperatura2= analogRead(A3);
  delay(500);
  temperatura=temperatura1+temperatura2;
  temperatura=temperatura/2;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print ("I1= ");
  corrh (temperatura, 1);
  lcd.setCursor(0,1);
  lcd.print ("U1= ");
  valor = analogRead(A2);
  voltm (valor, 33, 1);  
 // valor1 = analogRead(A0);
 // lcd.setCursor(0,1);
 // lcd.print ("I1= ");
  //maxc= digitalRead(6);
 // corrh (valor1, 1);
  delay(100);
}
