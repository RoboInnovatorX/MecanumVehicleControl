#include "Qt_Server.h"
#include "XmlReadWrite.h"
#include <QHostAddress>
#include <QDebug>
#include <QByteArray>
#include <QStringList>


Qt_Server::Qt_Server(QObject *parent) :  QObject(parent)
{
    //connection = NULL;
    serial.setPortName("/dev/ttyUSB0");
    serial.setBaudRate(115200);
    serial.open(QIODevice::ReadWrite);

    QObject::connect(&server,SIGNAL(newConnection()),this,SLOT(OnConnectionReq()));

    bool ok = server.listen(QHostAddress::Any,52001);
    if (ok){
        qDebug() << "Server is Listening...";
    }
    else
        qDebug() << "Listening fail!!!";

}

void Qt_Server::OnConnectionReq()
{
    connection = server.nextPendingConnection();
    clientVector.append(connection); // Add the new client to the list
    QObject::connect(connection,SIGNAL(readyRead()),this,SLOT(OnDataReceivedFromTcpClient()));
    QObject::connect(connection,SIGNAL(disconnected()),this,SLOT(OnDisconnected()));

    qDebug() << "Client is connected. Waiting for the data!";
}

void Qt_Server::OnDisconnected()
{
//    if (connection==NULL)
//        return;
//    qDebug() << "Client disconnected. Please connect again!!!";

//    connection->close();
//    connection->deleteLater();
//    connection= NULL;

    QTcpSocket* disconnectedClient = qobject_cast<QTcpSocket*>(sender());
    if(disconnectedClient){
        qDebug() << "Client disconnected. Please connect again!!!";
        clientVector.removeAll(disconnectedClient);
        disconnectedClient->deleteLater();
    }
}


void Qt_Server::OnDataReceivedFromTcpClient()
{

    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(sender());
    if(!senderSocket)
        return;

//    if(connection==NULL)
//        return;
//    QByteArray recv=connection->readAll();
    QByteArray recv = senderSocket->readAll();
    QString recvString=recv;
    XmlReadWrite xmlreader;
    qDebug() << "Received" << recvString << "\n";
    bool A = recvString.contains("DirectMode");
    bool B = recvString.contains("MovingMode");
    bool C = recvString.contains("DriveMode");
    bool D = recvString.contains("Reset");
    bool E = recvString.contains("GripperValue");
    bool F = recvString.contains("UpValue");
    bool G = recvString.contains("DownValue");
    bool H = recvString.contains("PositionMode");

    if(A){
        int id = xmlreader.XmlGetInt(recvString,"MotorId"),
                cmd= xmlreader.XmlGetInt(recvString,"Command"),
                type= xmlreader.XmlGetInt(recvString,"Type"),
                value = xmlreader.XmlGetInt(recvString,"Value");
        qDebug() << "Received" << id << cmd <<type <<value << "\n";
        Qt_Server::SendTmclCommand(id, cmd, type, value);
    };

    if(B || C){
        int Lf = xmlreader.XmlGetInt(recvString,"LeftFront"),
                Rf= xmlreader.XmlGetInt(recvString,"RightFront"),
                Lb= xmlreader.XmlGetInt(recvString,"LeftBack"),
                Rb = xmlreader.XmlGetInt(recvString,"RightBack");
        qDebug() << "Received" << Lf << Rf << Lb << Rb << "\n";
        Qt_Server::SendTmclCommand(motor_id_1, 1, 0, Lf);
        Qt_Server::SendTmclCommand(motor_id_2, 1, 0, Rf);
        Qt_Server::SendTmclCommand(motor_id_3, 1, 0, Lb);
        Qt_Server::SendTmclCommand(motor_id_4, 1, 0, Rb);

    };

    if(D){

        Qt_Server::SendTmclCommand(motor_id_1, 1, 0, 0);
        Qt_Server::SendTmclCommand(motor_id_2, 1, 0, 0);
        Qt_Server::SendTmclCommand(motor_id_3, 1, 0, 0);
        Qt_Server::SendTmclCommand(motor_id_4, 1, 0, 0);
        qDebug() << "Reset the value to 0 "<< "\n";
    };
    // send the received data to ESP32 client
    if(E || F ||G){
        sendXmlToEsp32(recv);
        qDebug() << "I am sending this..." << recv;
    };
    if(H){
        int Lf = xmlreader.XmlGetInt(recvString,"LeftFront"),
                Rf= xmlreader.XmlGetInt(recvString,"RightFront"),
                Lb= xmlreader.XmlGetInt(recvString,"LeftBack"),
                Rb = xmlreader.XmlGetInt(recvString,"RightBack");
        qDebug() << "Received" << Lf << Rf << Lb << Rb << "\n";
        Qt_Server::SendTmclCommand(motor_id_1, 4, 0, Lf);
        Qt_Server::SendTmclCommand(motor_id_2, 4, 0, Rf);
        Qt_Server::SendTmclCommand(motor_id_3, 4, 0, Lb);
        Qt_Server::SendTmclCommand(motor_id_4, 4, 0, Rb);
    }
}

void Qt_Server::sendXmlToEsp32(const QByteArray &xmlData)
{
    for (QTcpSocket* client : clientVector){
        client->write(xmlData);
    }
}

QByteArray Qt_Server::SendTmclCommand(uint8_t rs485_id, uint8_t cmd, uint8_t type, int32_t value)
{
    QByteArray send(9,0), receive;
    send[0] = rs485_id;
    send[1] = cmd;
    send[2] = type;
    send[3] = 0;

    uint32_t* uvalue = (uint32_t*) &value;
    send[4] =((*uvalue>>24) & 0xFF);
    send[5] =((*uvalue>>16) & 0xFF);
    send[6] =((*uvalue>>8) & 0xFF);
    send[7] =((*uvalue) & 0xFF);

    uint8_t checksum;
    checksum = send[0];
    for(int i=1; i<8; i++){
        checksum += send[i];
    }
    send[8] = checksum;

    if (serial.write(send)!=9){
        return receive;
    }

    bool read = serial.waitForReadyRead(100);
    if(read)
    {
        receive = serial.read(9);
    }
    return receive;
 }
