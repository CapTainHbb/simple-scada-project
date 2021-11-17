#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Scada Input Client");

    initValues();
    initConnectionToServer();
    connectToServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initValues() {
    for(int i = 0; i < 4; i++)
        powersValue[i] = true;

    json.insert("power1", powersValue[0]);
    json.insert("power2", powersValue[1]);
    json.insert("power3", powersValue[2]);
    json.insert("power4", powersValue[3]);
    json.insert("voltage", voltage);
    json.insert("activePower", activePower);
    json.insert("reactivePower", reactivePower);
}

void MainWindow::setValues() {
    json["power1"] = powersValue[0];
    json["power2"] = powersValue[1];
    json["power3"] = powersValue[2];
    json["power4"] = powersValue[3];
    json["voltage"] = voltage;
    json["activePower"] = activePower;
    json["reactivePower"] = reactivePower;
}

void MainWindow::initConnectionToServer() {
    socket = new QTcpSocket();
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &MainWindow::onSocketError);
}

void MainWindow::connectToServer() {
    socket->connectToHost("127.0.0.1", 8001);
}

void MainWindow::on_radioButton1_1_clicked(bool checked)
{
    if(checked) {
        ui->radioButton1_2->toggle();
        powersValue[0] = true;
    }
    else {
        ui->radioButton1_2->toggle();
        powersValue[0] = false;
    }
}

void MainWindow::on_radioButton1_2_clicked(bool checked)
{
    if(checked) {
        ui->radioButton1_1->toggle();
        powersValue[0] = false;
    }
    else {
        ui->radioButton1_1->toggle();
        powersValue[0] = true;
    }
}

void MainWindow::on_radioButton2_1_clicked(bool checked)
{
    if(checked) {
        ui->radioButton2_2->toggle();
        powersValue[1] = true;
    }
    else {
        ui->radioButton2_2->toggle();
        powersValue[1] = false;
    }
}

void MainWindow::on_radioButton2_2_clicked(bool checked)
{
    if(checked) {
        ui->radioButton2_1->toggle();
        powersValue[1] = false;
    }
    else {
        ui->radioButton2_1->toggle();
        powersValue[1] = true;
    }
}

void MainWindow::on_radioButton3_1_clicked(bool checked)
{
    if(checked) {
        ui->radioButton3_2->toggle();
        powersValue[2] = true;
    }
    else {
        ui->radioButton3_2->toggle();
        powersValue[2] = false;
    }
}

void MainWindow::on_radioButton3_2_clicked(bool checked)
{
    if(checked) {
        ui->radioButton3_1->toggle();
        powersValue[2] = false;
    }
    else {
        ui->radioButton3_1->toggle();
        powersValue[2] = true;
    }
}

void MainWindow::on_radioButton4_1_clicked(bool checked)
{
    if(checked) {
        ui->radioButton4_2->toggle();
        powersValue[3] = true;
    }
    else {
        ui->radioButton4_2->toggle();
        powersValue[3] = false;
    }
}

void MainWindow::on_radioButton4_2_clicked(bool checked)
{
    if(checked) {
        ui->radioButton4_1->toggle();
        powersValue[3] = false;
    }
    else {
        ui->radioButton4_1->toggle();
        powersValue[3] = true;
    }
}

void MainWindow::on_activePowerInput_valueChanged(double arg1)
{
    activePower = arg1;
}

void MainWindow::on_reactivePowerInput_valueChanged(double arg1)
{
    reactivePower = arg1;

}

void MainWindow::on_voltageInput_valueChanged(double arg1)
{
    voltage = arg1;

}

void MainWindow::on_sendButton_clicked()
{
    setValues();
    QJsonDocument jdoc(json);

    if(socket->isOpen() && socket->isWritable())
        socket->write(jdoc.toJson());
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


