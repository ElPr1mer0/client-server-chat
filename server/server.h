#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QVector>


class QTcpSocket;

class SERVER: public QTcpServer{
    Q_OBJECT
public:
    QTcpSocket *client = nullptr;

private:
    QVector <QTcpSocket *> clients;
    QByteArray data;
    quint16 block_size;


public:
    SERVER();

private:
    void SendToClient(QString &message);

private slots:
    void SlotReadyRead();
    void incomingConnection(qintptr client_decriptor);
    void SlotRemoveClient();
};

#endif // SERVER_H
