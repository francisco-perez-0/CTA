/*  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)*/

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int volt = A0; //Lectura tension de batería

#define SOBRECARGA 7
#define BAJATENSION 6

void setup() {
  lcd.begin(16, 2);
  lcd.print("Nivel Bat.:");
  pinMode (SOBRECARGA,OUTPUT);  //Habilitación de Realay de Sobrecarga 
  pinMode (BAJATENSION,OUTPUT);   //Habilitación de Relay de Baja tensión

}

void loop() {
    int i=0;
    int j=0;
  
  unsigned long br = analogRead(volt);

  unsigned long ope = br*1000;

  unsigned long ope1 = ope/1013;

  unsigned int mil = ope1/1000;
  unsigned int res1 = ope1%1000;
  unsigned int cien = res1/100;
  unsigned int res2 = res1%100;
  unsigned int diez = res2/10;
  unsigned int uni = res1%10;
   
  lcd.setCursor(5, 1);  
  lcd.print(mil);
  lcd.print(cien);
  lcd.print(diez);
  lcd.print(",");
  lcd.print(uni);
  lcd.setCursor(15, 1);
  lcd.print("V");

  if (br<1000 && br>850){
    digitalWrite(SOBRECARGA,HIGH);
    digitalWrite(BAJATENSION,LOW);

  } else if (br<=850 && br>800){
    //digitalWrite(7,HIGH);
    digitalWrite(BAJATENSION,LOW);
    
  } else if (br<800 && br>600){
    digitalWrite(SOBRECARGA,LOW);
    digitalWrite(BAJATENSION,LOW);

  } else if (br<=600 && br>500){
    digitalWrite(SOBRECARGA,LOW);
    digitalWrite(BAJATENSION,HIGH);
  }

  }// Fin programa
