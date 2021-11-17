#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtNetwork/QTcpSocket>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
   void initValues();
   void setValues();
   void initConnectionToServer();
   void connectToServer();

private slots:

    void on_radioButton1_1_clicked(bool checked);

    void on_radioButton1_2_clicked(bool checked);

    void on_radioButton2_1_clicked(bool checked);

    void on_radioButton2_2_clicked(bool checked);

    void on_radioButton3_1_clicked(bool checked);

    void on_radioButton3_2_clicked(bool checked);

    void on_radioButton4_1_clicked(bool checked);

    void on_radioButton4_2_clicked(bool checked);

    void on_activePowerInput_valueChanged(double arg1);

    void on_reactivePowerInput_valueChanged(double arg1);

    void on_voltageInput_valueChanged(double arg1);

    void on_sendButton_clicked();

    void onSocketError();


private:
    Ui::MainWindow *ui;
    bool powersValue[4] { };
    double voltage{ 0.0 };
    double activePower{ 0.0 };
    double reactivePower{ 0.0 };
    QTcpSocket* socket;
    QJsonObject json;
};
#endif // MAINWINDOW_H
