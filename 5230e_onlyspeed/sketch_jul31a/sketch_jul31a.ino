#define TINY_GSM_MODEM_SIM5360
#include  <SoftwareSerial.h>
#include  <TinyGsmClient.h>

#define SerialMon Serial
#define SerialAT Serial1
#define MODEM_BAUD 4800
SoftwareSerial SerialAT(10,11);

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

const char apn[] = "internet.personal.com.ar";
const char user[] = "";
const char pass[] = "";

void setup() {
  Serial.begin(4800);
  delay(100);
  SerialAT.begin(MODEM_BAUD);
  delay(5000);

  Serial.println("Iniciando modem...");
  modem.restart();
  delay(2500);
  // put your setup code here, to run once:
  Serial.println("Conectando a red celular");
  modem.gprsConnect(apn, user, pass);
  if(modem.isGprsConnected()){
    Serial.println("Conectado a la red celular");
  } else {
    Serial.println("Fallo al conectar");
  }

  if(client.connect("httpbin.org", 80)){
    Serial.println("GET /get HTTP/1.1");
    client.println("Host: httpbin.org");
    client.println("Connection close");
    client.println();

  } else {
    Serial.println("Fallo a conectar a internet");
  }
  Serial.println("Esperando respuesta...");
  while(client.connected() || client.available()){
    if(client.available() > 0){
      char c = client.read();
      Serial.print(c);
    }
    client.stop();
    Serial.println("Conexion cerrada");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
