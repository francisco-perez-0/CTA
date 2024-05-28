#include "leergps.h"


leergps::leergps(){
    nmeaSource = new QNmeaPositionInfoSource(QNmeaPositionInfoSource::RealTimeMode, this);
    if(nmeaSource){
        qDebug() << "Creacion NMEA correcta";
        //positionUpdated emite una señal cada vez que se recibe nuevos datos. Esta señal si anteriormente se llama startUpdates() o requestUpdate()
        connect(nmeaSource, &QNmeaPositionInfoSource::positionUpdated, this, &leergps::handlerRequest);
    } else{
        qDebug() << "Error NMEA";
    }
}
void leergps::configurarConexion(){
    this->serial.setPortName(portName);
    this->serial.setBaudRate(QSerialPort::Baud9600);
    this->serial.setDataBits(QSerialPort::Data8);
    this->serial.setParity(QSerialPort::NoParity);
    this->serial.setStopBits(QSerialPort::OneStop);
    this->serial.setFlowControl(QSerialPort::NoFlowControl);
    //Source por donde ingresan los datos, en este caso puerto serie.
    nmeaSource->setDevice(&serial);
    if(!serial.open(QIODevice::ReadOnly)){
        qDebug() << "Error serie";
    }
}

void leergps::handlerRequest(QGeoPositionInfo info){
    //qDebug() << "Datos:";
    if(info.isValid()){
        this->datosGps = info;
        groundSpeed = info.attribute(QGeoPositionInfo::GroundSpeed);
        lat = info.coordinate().latitude();
        longitud = info.coordinate().longitude();
        //Emite señal para saber cuando poder leer datos correctos
        emit valueChanged(info);
    }

}
void leergps::inicializarSeñalGps(){
    //qDebug() << "Obteniendo datos...";
    if(nmeaSource){
        nmeaSource->startUpdates();
    }
}

qreal leergps::getGroundSpeed(){
    return groundSpeed;
}

double leergps::getLatitud(){
    //qDebug() << "Latitud: " << datosGps.coordinate().latitude();
    return this->lat;
}

double leergps::getLongitud(){
    return longitud;
}
