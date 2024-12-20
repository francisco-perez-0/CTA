#include <math.h>
#include <LiquidCrystal.h>

int valor=0;
int valor1=0;
int lect;
int lecti;
int temperatura1;
int temperatura2;
bool maxc=0;

double volin;
double volout;
double corin;
double corout;
double ief;
double uef;
double efic;

const int rs = 9, en = 8, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//////////////////////////Medicion con termistores//////////////////////////
////////////////////////// Valores para la Steinh///////////////////////////
double xread=0; 
double temp = 0;
int temp1=0;

double A = 0.001112386014;
double B = 0.0002400476586;
double C = 0.0000000393051678;

double logres=9.210340372;  //Tengo que saber de donde saqué este valor!!!
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
////////////////////Fin medicion con termistores//////////////////////////

/////////////////// Conversión de Corriente Hall////////////////////
/*Función para medicion de corriente usando el modulo acs712.
  La variable 'i' define si la función retorna un valor o 
  imprime en display.
*/
float corrh (int ci, bool i){
  float valc=ci*5;
  valc=valc/1023;
  valc=(valc-2.41);
  valc=valc*10;
  valc=valc;//Solo para simulacion
   
  if (i==1){
    lcd.print (valc);
    return 0;
    }
    return valc;
  }
/////////////////// Fin Conversion de Corriente Hall///////////////////////

////////////////// Función para conversión de tensión /////////////////////
/*Esta función incluye visualización en pantalla si se habilita 's'.
  Para usar esta función hay que definir cual es la tensión máxima 'vmax' 
  a medir.
*/
float voltm (int v, int vmax, bool s){
  float val;
  
  if (vmax>=100){
    vmax=vmax/10;
    val=v*vmax;
    val=val*10;
    
  }
  if (vmax<100){
  val=v*vmax;
  val=val/1015;
  }

  if (s==1){
    lcd.print (val);
    return 0;
    }
    return val;
  }
/////////////////// Fin Conversión de Tensión //////////////////

////////////////Filtro exponencial ponderado///////////////////
#define N 5 // Tamaño de la ventana del promedio móvil
#define N1 15 // Tamaño de la ventana del promedio móvil
int valores1[N] = {0}; // Array para almacenar los valores recientes
int indice1 = 0; // Índice actual en el array

int valores2[N] = {0}; // Array para almacenar los valores recientes
int indice2 = 0; // Índice actual en el array

int valores3[N1] = {0}; // Array para almacenar los valores recientes
int indice3 = 0; // Índice actual en el array

int filtrarPromedio1(int nuevoValor1) {
  valores1[indice1] = nuevoValor1;  // Agregar el nuevo valor al array
  indice1 = (indice1 + 1) % N;    // Mover el índice circularmente
  
  // Calcular el promedio
  long suma1 = 0;
  for (int i = 0; i < N; i++) {
    suma1 += valores1[i];
  }
  return suma1 / N; // Retornar el promedio
}

int filtrarPromedio2(int nuevoValor2) {
  valores2[indice2] = nuevoValor2;  // Agregar el nuevo valor al array
  indice2 = (indice2 + 1) % N;    // Mover el índice circularmente
  
  // Calcular el promedio
  long suma2 = 0;
  for (int i = 0; i < N; i++) {
    suma2 += valores2[i];
  }
  return suma2 / N; // Retornar el promedio
}

int filtrarPromedio3(int nuevoValor3) {
  valores3[indice3] = nuevoValor3;  // Agregar el nuevo valor al array
  indice3 = (indice3 + 1) % N1;    // Mover el índice circularmente
  
  // Calcular el promedio
  long suma3 = 0;
  for (int i = 0; i < N1; i++) {
    suma3 += valores3[i];
  }
  return suma3 / N1; // Retornar el promedio
}
///////////////Fin filtro exp pnd/////////////////////////////

void setup() {
  
  ////////Display///////
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  //////////////////////

  /////////Reles////////
  pinMode(A4, OUTPUT);
  pinMode(A4, OUTPUT);
  //////////////////////

  /////Multiplexor//////
  pinMode(A1, INPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  //////////////////////

  lcd.begin(20, 4);
  
}

void loop() {

//////////////X0 (Term2)//////////////
digitalWrite(10,LOW);
digitalWrite(11,LOW);
digitalWrite(12,LOW);
lcd.setCursor(11,0);
lcd.print("T2= ");
lect= analogRead(A1);
steinh(lect,1);
delay(5);
//////////////////////////////////////

//////////////X1 (Corr2)//////////////
digitalWrite(10,HIGH);
digitalWrite(11,LOW);
digitalWrite(12,LOW);
lcd.setCursor(11,1);
lect=analogRead(A1);
lect=filtrarPromedio1(lect);
//lect=lect-1;
lcd.print("I2= ");
corrh(lect,1);
corout=corrh(lect,0);
delay(5);
//////////////////////////////////////

//////////////X2 (Corr1)//////////////
digitalWrite(10,LOW);
digitalWrite(11,HIGH);
digitalWrite(12,LOW);
lcd.setCursor(0,1);
lect=analogRead(A1);
lect=filtrarPromedio2(lect);
lect=lect+5;
lcd.print("I1= ");
corrh(lect,1);
corin=corrh(lect,0);
delay(5);
//////////////////////////////////////

//////////////X3 (Term1)//////////////
digitalWrite(10,HIGH);
digitalWrite(11,HIGH);
digitalWrite(12,LOW);
//lcd.clear();
lcd.setCursor(0,0);
lcd.print("T1= ");
lect= analogRead(A1);
steinh(lect,1);
delay(5);
//////////////////////////////////////

/*//////////////X4 (Volt3)//////////////
digitalWrite(10,LOW);
digitalWrite(11,LOW);
digitalWrite(12,HIGH);
lcd.setCursor(11,3);
lcd.print("U3= ");
lect= analogRead(A1);
voltm(lect,30,1);
delay(10);*/
//////////////////////////////////////

//////////////X5 (Volt0)//////////////
digitalWrite(10,HIGH);
digitalWrite(11,LOW);
digitalWrite(12,HIGH);
lcd.setCursor(0,2);
lcd.print("U0= ");
lect= analogRead(A1);
voltm(lect,30,1);
volin=voltm(lect,30,0);
delay(5);
//////////////////////////////////////

/*/////////////X6 (Volt2)//////////////
digitalWrite(10,LOW);
digitalWrite(11,HIGH);
digitalWrite(12,HIGH);
lcd.setCursor(0,3);
lcd.print("U2= ");
lect= analogRead(A1);
voltm(lect,30,1);
delay(10);*/
//////////////////////////////////////

//////////////X7 (Volt1)//////////////
digitalWrite(10,HIGH);
digitalWrite(11,HIGH);
digitalWrite(12,HIGH);
lcd.setCursor(11,2);
lcd.print("U1= ");
lect= analogRead(A1);
lect=filtrarPromedio3(lect);
voltm(lect,30,1);
volout=voltm(lect,30,0);
delay(5);
//////////////////////////////////////
ief=corout/corin;
uef=volout/volin;
efic=ief*uef;
efic=efic*100;
lcd.setCursor(0, 3);
lcd.print("Eff= ");
lcd.print(efic);
lcd.print(" %");
delay(1000);
digitalWrite(10,LOW);
digitalWrite(11,LOW);
digitalWrite(12,HIGH);
lect=analogRead(A1);  
delay(10);

lcd.clear();

}

///////////////Cartel de Bienvenida///////////////
/*digitalWrite(10,LOW);
digitalWrite(11,LOW);
digitalWrite(12,HIGH);
lect=analogRead(A1);
delay(10);

while(lect>=0 && lect <=5){
digitalWrite(10,LOW);
digitalWrite(11,LOW);
digitalWrite(12,HIGH);
lect=analogRead(A1);  
lcd.setCursor(5, 0);
lcd.print("Bienvenido");
delay(800);
lcd.setCursor(0, 1);
lcd.print("Soy el Banco de");
delay(800);
lcd.setCursor(0, 1);
lcd.print("Ensayos de Gaston");
delay(800);
lcd.setCursor(0, 2);
lcd.print("Solo para uso");
delay(800);
lcd.setCursor(0, 2);
lcd.print("Aeroespacial ");
delay(800);
lcd.setCursor(0, 3);
lcd.print("No me desaproveche");
delay(900);
lcd.clear();
}*/
//while(lect>6){
//}
