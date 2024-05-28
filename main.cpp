
#include "mainwindow.h"
#include <leergps.h>
#include <QtSerialPort/QSerialPort>
#include <QApplication>
#include <QGeoCoordinate>
#include <QNmeaPositionInfoSource>
#include <QDebug>
#include <fcntl.h>
#include <unistd.h>
#include <QTimer>
#include <mqttclient.h>
QSerialPort serial;
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
/*    QNmeaPositionInfoSource nmeaSource(QNmeaPositionInfoSource::RealTimeMode);

    serial.setPortName("/dev/serial0");
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setDataBits(QSerialPort::Data8);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    nmeaSource.setDevice(&serial);

    if(!serial.open(QIODevice::ReadOnly)){
        qDebug() << "Error serie";
        return 1;
    }
    qDebug() << "Leyendo datos del gps";
    QObject::connect(&nmeaSource, &QNmeaPositionInfoSource::positionUpdated, [&](const QGeoPositionInfo &info){
        qDebug() << "Latitud: " << info.coordinate().latitude();
        qDebug() << "Longitud: " << info.coordinate().longitude();
        //longitud = info.coordinate().longitude();
        //qDebug() << "DE la variable longitud " << longitud;
        //write(pipeFIFO, &longitud, sizeof(double));
    });
    nmeaSource.startUpdates();
*/
    leergps ser;
    ser.configurarConexion();
    MainWindow w;
    w.show();
    ser.inicializarSeñalGps();

    QObject::connect(&ser,&leergps::valueChanged, [=](QGeoPositionInfo info) {
       qDebug() << "Latitud: " << info.coordinate().latitude();
       qDebug() << "Logitud: " << info.coordinate().longitude();
       qDebug() << "Speed: " << info.attribute(QGeoPositionInfo::GroundSpeed);
    });

    mqttClient publisher;
    QObject::connect(&ser, &leergps::valueChanged, &publisher, &mqttClient::publishData);

    //qDebug() << "Latitud: " << ser.getLatitud();
    return a.exec();

}
