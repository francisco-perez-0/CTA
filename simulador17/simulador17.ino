#define pinTension A1

#define sensibilidad 0.0125
#define Vref 5.0
#define Vcero 2.5

int flagNegativo = 0;


long tension;
int parteEnteraT;
int parteFlotanteT;
float convTension = 0;


long corriente;
int parteEnteraC;
int parteFlotanteC;
int i=0;

float aux= 2.2222;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  corriente = random(20,1023);
  float corrienteAux = ((corriente*Vref)/1023); 
  //Serial.println(corrienteAux,3);
  float convCorriente = funcionConversionCorriente(corrienteAux);
  if(convCorriente < 0){
    flagNegativo = 1;
    convCorriente*=-1;
  }
  parteEnteraC = (int)convCorriente;
  parteFlotanteC = (int)((convCorriente - parteEnteraC) * 100); // Decimal en centésimas

  //convCorriente = mapFloat(corriente, 0, 1023, 0, 50);
  
  // ------------------- Tension -------------------
  tension = random(20,1023);
  //convTension = mapFloat(tension, 0, 1023, 0, 100);
  float ope = tension*100;
  float ope1 = ope/1023;
  parteEnteraT = (int)ope1;
  parteFlotanteT = (int)((ope1 - parteEnteraT)*100);
  
  delay(1);
  if(i++ == 500){
    Serial.print("Analogico tension: ");
    Serial.print(tension);
    Serial.print(" | ");
    Serial.print(" --> ");
    Serial.print(ope1,2);
    Serial.print(" Analogico Corriente: ");
    Serial.print(corriente);
    Serial.print(" | ");
    Serial.print(" --> ");
    Serial.print(parteEnteraC);
    Serial.print(",");
    Serial.print(parteFlotanteC);
    Serial.print("|||||");
    Serial.print(convCorriente);
    Serial.print("  Flag: ");
    Serial.println(flagNegativo);
    i=0;
  }
  flagNegativo = 0;

}
float funcionConversionCorriente(float entrada){
  return ((entrada - 2.5)/0.0125);
}