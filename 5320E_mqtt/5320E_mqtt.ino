#define TINY_GSM_MODEM_SIM5360
//#define SerialMon Serial
#include <mcp2515.h>
#include <SoftwareSerial.h>

SoftwareSerial SerialAT(3,4); //RX TX

// CAN
MCP2515 mcp2515(10);
struct can_frame trama1;

// APN
const char apn[] = "datos.personal.com";

// MQTT
const char* broker = "test.mosquitto.org";

#define MSG_BUFFER_SIZE (50)
char msg_lon[MSG_BUFFER_SIZE];
char msg_lat[MSG_BUFFER_SIZE];
char msg_vel[MSG_BUFFER_SIZE];

const char* topicGps = "Ecobus/coordinate";
const char* topicGps_lon = "Ecobus/coordinate/lat";
const char* topicGps_lat = "Ecobus/coordinate/lon";
const char* topicDebug = "Ecobus/debug";
const char* topicGps_vel = "Ecobus/coordinate/vel";

#include <TinyGsmClient.h>
#include <PubSubClient.h>

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

String sentence = "";
String lat = "";
String lon = "";
String vel = "";
String gpsData;

float velocidad = 0;

int cant;
int i1;
int i2;
int i3;
int i4;
int i5;
int i6;
int i7;
int i8;

int flagMCP = 0;


uint32_t ultimaReconeccionIntentada = 0;

void mqttCallback(char* topic, byte* payload, unsigned int len){
  //SerialMon.print("Mensaje recibido [");
  //SerialMon.print(topic);
  //SerialMon.print("]: ");
  //SerialMon.write(payload, len);
  //SerialMon.println();
}

boolean mqttConnect(){
  //SerialMon.print("Conectando a ");
  //SerialMon.print(broker);
  //SerialMon.println();

  boolean status = mqtt.connect("Ecobus", "", "");
  if(status == false){
    //SerialMon.print("Fallo en la conexion al topico, estado: ");
    //SerialMon.println(mqtt.state());
    return false; 
  } else {
    //SerialMon.println(" Conexión MQTT Correcta");
    mqtt.publish(topicGps, "Conexion establecida");
    //mqtt.subscribe(topicDebug);
    return mqtt.connected();
  }
}

void setup() {
  trama1.can_id = 910; 
  trama1.can_dlc = 1;
  trama1.data[0] = 0x00;
  
  

  cant = 0;
  //SerialMon.begin(4800); // Incrementar la velocidad de baud
  delay(10);

  //SerialMon.println("Espere la conexion...");
  SerialAT.begin(4800); // Incrementar la velocidad de baud
  delay(6000);

  //SerialMon.println("Inicializando modem GPRS GSM...");
  modem.restart();
  String modemInfo = modem.getModemInfo();
  //SerialMon.print("Informacion sobre el modem: ");
  //SerialMon.println(modemInfo);

  //SerialMon.print("Conectando a ");
  //SerialMon.println(apn);
  if( !modem.gprsConnect(apn, "", "")){
    //SerialMon.println(" Error");
    delay(1000);
    return;
  }
  //SerialMon.println("Hecho");
  if (modem.isGprsConnected()) { }//SerialMon.println("Conexion establecida"); }

  //SerialAT.write("AT+CGPSINFO=1\r\n");

  mqtt.setServer(broker, 1883);
  mqtt.setCallback(mqttCallback);
  mqtt.setKeepAlive(60); // Enviar ping cada 60 segundos
  mqtt.setSocketTimeout(60); // Tiempo de espera de socket de 60 segundos



  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);
  //aca decia 125 pero lo cambio a 250 que es la comun
  mcp2515.setNormalMode();
  delay(1000);

}

void loop() {
  if (!modem.isNetworkConnected()) {
    //SerialMon.println("Red desconectada");
    if (!modem.waitForNetwork(180000L, true)) {
      //SerialMon.println(" fallo");
      delay(10000);
      return;
    }
    if (modem.isNetworkConnected()) {
      //SerialMon.println("Red reconectada");
    }
  }

  if (!mqtt.connected()) {
    //SerialMon.println("=== MQTT NO CONECTADO ===");
    // Reintentar cada 10 segundos
    uint32_t t = millis();
    if (t - ultimaReconeccionIntentada > 10000) {
      cant++;
      ultimaReconeccionIntentada = t;
      if (mqttConnect()) { ultimaReconeccionIntentada = 0; }
    }
    delay(100);
    return;
  }

  /*static unsigned long lastGPSRequest= 0;
  uint32_t requestGPSINFO = millis();
  if( requestGPSINFO - lastGPSRequest > 1000){
    lastGPSRequest = millis();
  }*/
  SerialAT.println("AT+CGPSINFO");
  delay(1000);

  
  
  if (SerialAT.available()) {
    while (SerialAT.available()) {
      char c = SerialAT.read();
      //String gpsData = SerialAT.readString();
      //Serial.write(SerialAT.read());
      //Serial.println(gpsData);
      sentence.concat(c);
    }
  }
  /*
  if (Serial.available()) {
    String atCommand = Serial.readStringUntil('\n');
    SerialAT.println(atCommand);
  }*/
  
  //SerialMon.println(sentence);
  i1 = sentence.indexOf(':');
  i2 = sentence.indexOf(',');
  i3 = sentence.indexOf(',', i2+1);
  i4 = sentence.indexOf(',', i3+1);
  i5 = sentence.indexOf(',', i4+1);
  i6 = sentence.indexOf(',', i5+1);
  i7 = sentence.indexOf(',', i6+1);
  i8 = sentence.indexOf(',', i7+1);
   //8-9

  lat = sentence.substring(i1+1, i2);
  lon = sentence.substring(i3+1, i4);
  vel = sentence.substring(i7+1, i8);
  if (lat.compareTo("") > 0){
    //SerialMon.println(lat);
    lat.toCharArray(msg_lat, MSG_BUFFER_SIZE);
    lon.toCharArray(msg_lon, MSG_BUFFER_SIZE);
    //SerialMon.println(msg_lon);
    velocidad = vel.toFloat();
    velocidad *= 0.514444;
    vel.toCharArray(msg_vel, MSG_BUFFER_SIZE);
    //SerialMon.println(velocidad);
    // Publicar un mensaje cada 10 segundos
    static unsigned long lastPublish = 0;
    if (millis() - lastPublish > 2000) {
      lastPublish = millis();
      //SerialMon.print("Publicando mensaje...");
      if (mqtt.publish(topicGps_lat, msg_lat)) {
        //SerialMon.println(" publicado");
      } else {
        //SerialMon.println(" falló");
      }
      if (mqtt.publish(topicGps_lon, msg_lon)) {
        //SerialMon.println(" publicado");
      } else {
        //SerialMon.println(" falló");
      }
      if (mqtt.publish(topicGps_vel, msg_vel)) {
        //SerialMon.println(" publicado");
      } else {
        //SerialMon.println(" falló");
      }
    }
  }
  
  trama1.data[0] = vel.toInt();
   //***************************
  if (mcp2515.sendMessage(&trama1) == MCP2515::ERROR_OK);

  //**************************
  sentence = "";
  mqtt.loop();
}

void resetArduino() {
  asm volatile ("jmp 0");  // Saltar a la dirección 0, que es el inicio del código
}
