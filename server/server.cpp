#include "server.h"

#include <QTcpSocket>
#include <QDataStream>
#include <QTime>

SERVER::SERVER(){
    this->listen(QHostAddress::Any,2282) ? qDebug()<<"Server have started!": qDebug()<<"Error to start the server!";
    block_size = 0;
}

void SERVER::SendToClient(QString &message){
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);


    out << quint16(0) << message<<QTime::currentTime();
    out.device()->seek(0);
    out<<quint16(data.size() - sizeof(quint16));

    for(int i = 0; i<clients.size();++i)
        if((QTcpSocket*)sender() != clients[i]) clients[i]->write(data);
}

void SERVER::SlotReadyRead(){
    client = (QTcpSocket *)sender();
    QDataStream in(client);
    in.setVersion(QDataStream::Qt_5_15);

    if(in.status() == QDataStream::Ok){
        qDebug()<<"read...";

        for(;;){
            if (block_size == 0){
                qDebug()<<"block_size = 0";
                if(client->bytesAvailable() < 2){
                    qDebug()<<"data < 2 bytes";
                    break;
                }
                in >> block_size;
                qDebug()<<"block_size = " << block_size;
            }

            if (client->bytesAvailable() < block_size){
                qDebug()<<"data not full";
                break;
            }

            QString message;
            in >> message;
            QTime time;
            in >> time;
            block_size = 0;
            SendToClient(message);
            break;
        }


    }else{
        qDebug()<<&"error reading";
    }
}

void SERVER::incomingConnection(qintptr client_descriptor){
    client = new QTcpSocket;
    client->setSocketDescriptor(client_descriptor);
    connect(client, &QTcpSocket::readyRead, this, &SERVER::SlotReadyRead);
    connect(client, &QTcpSocket::disconnected, client, &QTcpSocket::deleteLater);
    connect(client, &QTcpSocket::disconnected, this, &SERVER::SlotRemoveClient);

    clients.push_back(client);
    qDebug()<<"Client connected: " + QString::number(client_descriptor);
}

void SERVER::SlotRemoveClient(){
    for(int i  = 0; i < clients.size(); ++i){
        if ((QTcpSocket*)sender() == clients[i]){
            qDebug()<<"Client has disconnected: " << clients[i]->socketDescriptor();
            clients.remove(i);
        }
    }
}


