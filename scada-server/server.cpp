#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    clients = new QVector<QTcpSocket*>;
    initServer();
}

void Server::initServer() {
    server = new QTcpServer();
    server->setMaxPendingConnections(2);

    connect(server, &QTcpServer::newConnection, this, &Server::newClientConnected);
    if(server->listen(QHostAddress::Any, 8001)) {
        qDebug() << "started to listen at 127.0.0.1:8001";
    }
    else {
        qDebug() << "failed to listen at 127.0.0.1:8001";
    }
}

void Server::newClientConnected() {
    QTcpSocket* client = server->nextPendingConnection();
    QString ipAddress = client->peerAddress().toString();
    int port = client->peerPort();

    connect(client, &QTcpSocket::disconnected, this, &Server::onDisconnection);
    connect(client, &QTcpSocket::readyRead, this, &Server::onReadyRead);

    clients->append(client);

    qDebug() << "accepted connection form " + ipAddress + ":" + QString::number(port);

}

void Server::onDisconnection() {
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString ipAddress = client->peerAddress().toString();
    int port = client->peerPort();

    qDebug() << "client " + ipAddress + ":" + QString::number(port) + " has been disconnected.";


}

void Server::onReadyRead() {
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString message = client->readAll();
    sendMessage(message);
}

void Server::sendMessage(QString message) {
    for(int i = 0; i < clients->size(); i++) {
        if(clients->at(i)->isOpen() && clients->at(i)->isWritable())
        {
            clients->at(i)->write(message.toUtf8());
        }
    }
}





