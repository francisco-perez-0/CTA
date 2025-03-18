const int pinSalida=7;
const int rpm_simulada=1800;
const int imanes_por_vuelta = 1;
void setup() {
  pinMode(pinSalida,OUTPUT);

}

void loop() {
  float period_ms = (60.0 / (rpm_simulada * imanes_por_vuelta)) * 1000;
  digitalWrite(pinSalida, HIGH);
  delayMicroseconds(100);
  digitalWrite(pinSalida, LOW);
  delay(period_ms - 0.1);
}
