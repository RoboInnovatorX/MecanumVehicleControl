#ifndef QT_SERVER_H
#define QT_SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QSerialPort>


class Qt_Server : public QObject
{
    Q_OBJECT
private:
    QTcpServer server;
    QTcpSocket* connection;
    QVector<QTcpSocket*> clientVector; // Vector for differnt client
    QSerialPort serial;
    int motor_id_1 =101, motor_id_2 =102, motor_id_3 =103, motor_id_4 =104;

public:
    explicit Qt_Server(QObject *parent = nullptr);

public slots:
    void OnConnectionReq();
    void OnDisconnected();
    void OnDataReceivedFromTcpClient();
    void sendXmlToEsp32(const QByteArray &xmlData);
    QByteArray SendTmclCommand(uint8_t rs485_id, uint8_t cmd, uint8_t type, int32_t value);

};
#endif // QT_SERVER_H
