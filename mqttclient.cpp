#include "mqttclient.h"

mqttClient::mqttClient(QObject *parent) : QObject{parent}
{
    cliente.setHostname("test.mosquitto.org");
    cliente.setPort(1883);
    connect(&cliente, &QMqttClient::connected, this, &mqttClient::onConnected);
    connect(&cliente, &QMqttClient::disconnected, this, &mqttClient::onDisconnected);
    cliente.connectToHost();
}

void mqttClient::publishData(QGeoPositionInfo info){
    if(cliente.state() == QMqttClient::Connected){
        QString payload = QString("Latitude: %1, Longitude: %2").arg(info.coordinate().latitude()).arg(info.coordinate().longitude());
         cliente.publish(QMqttTopicName("gpsBus/data"), payload.toUtf8());
    } else{
        qDebug() << "MQTT cliente no esta conectado";
    }
}

void mqttClient::onDisconnected(){
    qDebug() << "Cliente desconectado del broker";
}

void mqttClient::onConnected(){
    qDebug() << "Cliente conectado";
}
