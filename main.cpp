#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <iostream>
#include <string>
#include <QSerialPort>
#include <QGeoCoordinate>
#include <QNmeaPositionInfoSource>


QSerialPort serial;
double longitud = 0;
double latitud = 0;


int main(int argc, char *argv[])
{


#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    //QNmeaPositionInfoSource source;
    serial.setPortName("/dev/serial0");
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setDataBits(QSerialPort::Data8);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    if(!serial.open(QIODevice::ReadOnly)){
        qDebug() << "Error serie";
        return 1;
    }
    qDebug() << "Leyendo datos del gps";
    while (serial.waitForReadyRead()) {
            QByteArray data = serial.readAll(); // Lee todos los datos disponibles
            qDebug() << "Datos leídos:" << data;
        }
    serial.close();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
