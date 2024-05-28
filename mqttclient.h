#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <QObject>
#include <QtMqtt/QMqttClient>
#include <leergps.h>
#include <QGeoPositionInfo>
#include <QNmeaPositionInfoSource>

class mqttClient : public QObject
{
    Q_OBJECT
public:
    explicit mqttClient(QObject *parent = nullptr);
public slots:
    void publishData(QGeoPositionInfo);

private:
    QMqttClient cliente;
private slots:
    void onConnected();
    void onDisconnected();


signals:

};

#endif // MQTTCLIENT_H
