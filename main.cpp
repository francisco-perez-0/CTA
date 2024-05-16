#include "mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QApplication>
#include <QGeoCoordinate>
#include <QNmeaPositionInfoSource>
#include <QDebug>

QSerialPort serial;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QNmeaPositionInfoSource nmeaSource(QNmeaPositionInfoSource::RealTimeMode);

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
    });
    nmeaSource.startUpdates();
/*
    while (serial.waitForReadyRead()) {
            QByteArray data = serial.readAll(); // Lee todos los datos disponibles
            qDebug() << "Datos leídos:" << data;
        }*/
    //serial.close();
    MainWindow w;
    w.show();
    return a.exec();
}
