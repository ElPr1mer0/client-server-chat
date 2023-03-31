#include "client.h"
#include <QTcpSocket>
#include <QDataStream>
#include <QTextBrowser>
#include <QLineEdit>
#include <QTime>


CLIENT::CLIENT(QTextBrowser *text_browser, QLineEdit *ld_message){
    socket = new QTcpSocket(this);
    connect(this, &QTcpSocket::readyRead, this, &CLIENT::SlotReadyRead);
    connect(this, &QTcpSocket::disconnected, this, &QTcpSocket::deleteLater);

    this->text_browser = text_browser;
    this->ld_message = ld_message;
    block_size = 0;
}

void CLIENT::SendToServer(QString message){
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);

    QTime time = QTime::currentTime();
    out << quint16(0) << message<<time;
    out.device()->seek(0);  
    out << quint16(data.size() - sizeof(quint16));

    this->write(data);

    text_browser->append("<font color=blue> <strong> " + message + "</strong></font>" + time.toString("hh:mm"));
    text_browser->setAlignment(Qt::AlignRight);
    ld_message->clear();
}

void CLIENT::SlotReadyRead(){
    socket = (QTcpSocket *)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_15);

    if(in.status() == QDataStream::Ok){

        for(;;){
            if (block_size == 0){
                qDebug()<<"block_size = 0";
                if(socket->bytesAvailable() < 2){
                    qDebug()<<"data < 2 bytes";
                    break;
                }
                in >> block_size;
                qDebug()<<"block_size = " << block_size;
            }

            if (socket->bytesAvailable() < block_size){
                qDebug()<<"data not full";
                break;
            }

            QString message;
            in >> message;
            QTime time;
            in >> time;

            text_browser->append(time.toString("hh:mm") + "<font color=red> <strong>" +message + "</strong></font> ");
            text_browser->setAlignment(Qt::AlignLeft);

            block_size = 0;
            break;
        }

    }else{
        text_browser->append("error reading");
    }
}
