#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <string>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("GUI Client");

    initUiVariables();
    initConnectionToServer();
    connectToServer();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    paintImage();

    painter = new QPainter(this);
    painter->drawImage(this->rect(), image, image.rect());
    delete painter;
}

void MainWindow::initConnectionToServer() {
    socket = new QTcpSocket();
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &MainWindow::onSocketError);
}

void MainWindow::connectToServer() {

    socket->connectToHost("127.0.0.1", 8001);

}

void MainWindow::initUiVariables() {
    image = QImage(this->size(), QImage::Format_RGB32);
    image.fill(Qt::darkGray);

    for(int i = 0; i < 4; i++)
        powerButtonsColors.push_back(Qt::darkGray);

    rects.push_back(QRect(260, 100, 30, 30));
    rects.push_back(QRect(260, 320, 30, 30));
    rects.push_back(QRect(165, 430, 30, 30));
    rects.push_back(QRect(335, 430, 30, 30));
}

void MainWindow::paintImage() {

    painter = new QPainter(&image);

    QPen pen(Qt::blue);
    pen.setWidth(5);
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);

    // blue vertical and horizental line
    painter->drawLine(QPoint(200, 50), QPoint(350, 50));
    painter->drawLine(QPoint(275, 50), QPoint(275, 170));
    //

    // Es
    pen.setColor(Qt::darkRed);
    painter->setPen(pen);
    painter->setFont(QFont("Times", 11, QFont::Bold));
    painter->drawText(QPoint(245, 130), "E");
    painter->drawText(QPoint(245, 350), "E");
    painter->drawText(QPoint(150, 460), "E");
    painter->drawText(QPoint(320, 460), "E");

    // draw T1, kV, MVA, MW
    pen.setColor(Qt::white);
    painter->setFont(QFont("Times", 22, QFont::Bold));
    painter->setPen(pen);
    painter->drawText(QPoint(105, 150), "T1");
    painter->setFont(QFont("Times", 16, QFont::Bold));
    painter->drawText(QPoint(30, 190), "kV");
    painter->drawText(QPoint(30, 230), "MVA");
    painter->drawText(QPoint(30, 270), "MW");
    //


    // draw yellow circle
    pen.setColor(Qt::yellow);
    pen.setWidth(5);
    painter->setPen(pen);
    painter->drawEllipse(QPoint(277, 250), 35, 35);
    //

    // draw blue circle
    pen.setColor(Qt::blue);
    painter->setPen(pen);
    painter->drawEllipse(QPoint(277, 205), 35, 35);
    //

    // draw blue arrow
    static const QPointF arrowHead[3] = {
        QPointF(350.0, 165.0),
        QPointF(345.0, 160.0),
        QPointF(350.0, 160.0)
    };
    painter->drawPolygon(arrowHead, 3);
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawLine(QPoint(220, 250), QPoint(345, 165));
    //

    // vertical yellow line
    pen.setColor(Qt::yellow);
    pen.setWidth(5);
    painter->setPen(pen);
    painter->drawLine(QPoint(275, 285), QPoint(275, 405));
    //

    // horizental yellow line
    pen.setWidth(7);
    painter->setPen(pen);
    painter->drawLine(QPoint(150, 405), QPoint(380, 405));

    // first vertical yellow line
    painter->drawLine(QPoint(180, 405), QPoint(180, 475));
    //

    // second vertical yellow line
    painter->drawLine(QPoint(350, 405), QPoint(350, 475));
    //


    // draw rectangles
    pen.setWidth(5);
    for(int i = 0; i < 4; i++) {
        painter->setBrush(powerButtonsColors.at(i));

        if(i == 0)
            pen.setColor(Qt::blue);
        else
            pen.setColor(Qt::yellow);

        painter->setPen(pen);
        painter->drawRect(rects.at(i));
    }
    //

    // three dark box
    pen.setWidth(0);
    pen.setColor(QColor(100, 100, 100));
    painter->setPen(pen);
    painter->setBrush(QColor(100, 100, 100));
    painter->drawRect(QRect(90, 170, 85, 25));
    painter->drawRect(QRect(90, 210, 85, 25));
    painter->drawRect(QRect(90, 250, 85, 25));
    //

    // numbers
    pen.setColor(Qt::white);
    painter->setFont(QFont("Times", 14, QFont::Bold));
    painter->setPen(pen);
    painter->drawText(QPoint(100, 190), QString::number(kV));
    pen.setColor(Qt::darkRed);
    painter->setPen(pen);
    painter->drawText(QPoint(160, 190), "E");
    //

    pen.setColor(Qt::white);
    painter->setFont(QFont("Times", 14, QFont::Bold));
    painter->setPen(pen);
    painter->drawText(QPoint(100, 230), QString::number(MVA));
    pen.setColor(Qt::darkRed);
    painter->setPen(pen);
    painter->drawText(QPoint(160, 230), "E");

    pen.setColor(Qt::white);
    painter->setFont(QFont("Times", 14, QFont::Bold));
    painter->setPen(pen);
    painter->drawText(QPoint(100, 270), QString::number(MW));
    pen.setColor(Qt::darkRed);
    painter->setPen(pen);
    painter->drawText(QPoint(160, 270), "E");
    //

    delete painter;
}

void MainWindow::onReadyRead() {
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString message = client->readAll();

    QJsonDocument jdoc{ QJsonDocument::fromJson(message.toUtf8()) };
    json = jdoc.object();


    powerButtonsColors[0] = (json["power1"].toBool()? Qt::blue: Qt::darkGray);
    powerButtonsColors[1] = (json["power2"].toBool()? Qt::yellow: Qt::darkGray);
    powerButtonsColors[2] = (json["power3"].toBool()? Qt::yellow: Qt::darkGray);
    powerButtonsColors[3] = (json["power4"].toBool()? Qt::yellow: Qt::darkGray);

    kV = json["voltage"].toDouble();
    MVA = json["activePower"].toDouble();
    MW = json["reactivePower"].toDouble();

    this->update();
}

void MainWindow::onSocketError() {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Error in Connection");
    msgBox.setText("Can not connect to server!");
    msgBox.setInformativeText("Press retry or cancel!");
    msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Retry);

    int ret = msgBox.exec();
    if(ret == QMessageBox::Retry) {
        connectToServer();
    }
    else if (ret == QMessageBox::Cancel) {
        exit(0);
    }

}
