#include <movingAvg.h>

movingAvg avg(5);

int hallPin = 3;
volatile int contador=0;
double radio=0.3;
double rpm;
double speed;
int promedio;

void setup() {
  //pinMode(interruptPin, INPUT_PULLUP);
  avg.begin();
  attachInterrupt(digitalPinToInterrupt(hallPin), Counter, FALLING);
  Serial.begin(9600);

}

void loop() {
  contador = 0;
  interrupts();
  delay(500);
  noInterrupts();
  rpm=(contador*60.0)/0.5;
  speed = (rpm*M_PI*radio*2.0)/60.0;
  avg.reading(speed);
  Serial.print("RPM: ");
  Serial.println(rpm);
  promedio = avg.getAvg();
  /*Serial.print("Velocidad: ");
  Serial.println(promedio);
*/
}

void Counter() {
  contador++;
}