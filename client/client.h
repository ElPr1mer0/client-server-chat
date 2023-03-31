#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>

class QTextBrowser;
class QLineEdit;

class CLIENT: public QTcpSocket{
    Q_OBJECT
public:
    QTcpSocket *socket = nullptr;
private:

    QByteArray data;
    quint16 block_size;

    QTextBrowser *text_browser = nullptr;
    QLineEdit *ld_message = nullptr;

public:
    CLIENT(QTextBrowser *, QLineEdit *);
    void SendToServer(QString message);

public slots:
    void SlotReadyRead();
};

#endif // CLIENT_H
