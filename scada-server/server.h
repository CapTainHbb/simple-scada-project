#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QVector>
#include <QObject>
#include <QDebug>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void initServer();

private slots:
    void newClientConnected();
    void onReadyRead();
    void onDisconnection();

private:
    QTcpServer* server;
    QVector<QTcpSocket*>* clients;
    void sendMessage(QString);
};

#endif // SERVER_H
