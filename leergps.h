#ifndef LEERGPS_H
#define LEERGPS_H

#include <QtSerialPort/QSerialPort>
#include <QObject>
#include <QGeoCoordinate>
#include <QNmeaPositionInfoSource>
#include <QtDebug>

class leergps : public QObject
{
    Q_OBJECT

public:
    explicit leergps();
    void configurarConexion();
    void inicializarSeñalGps();
    qreal getGroundSpeed();
    double getLatitud();
    double getLongitud();

private:
    QGeoPositionInfo datosGps;
    const QString portName = "/dev/serial0";
    QSerialPort serial;
    QNmeaPositionInfoSource *nmeaSource;
    double longitud;
    double lat;
    qreal groundSpeed; // metros/s

private slots:
    void handlerRequest(QGeoPositionInfo );

signals:
    //Como parametro QGeoPositionInfo es una clase que contiene posicion, direccion, velocidad, etc
    void valueChanged(QGeoPositionInfo );

};

#endif // LEERGPS_H
