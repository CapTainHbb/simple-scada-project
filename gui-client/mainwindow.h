#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QVector>
#include <QtNetwork/QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
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
    virtual void paintEvent(QPaintEvent* event);
    void initConnectionToServer();
    void connectToServer();
    void initUiVariables();
    void paintImage();
    void onReadyRead();


public slots:
    void onSocketError();


private:
    Ui::MainWindow *ui;
    QImage image;
    QPainter* painter;
    QPen pen;
    QVector<QColor> powerButtonsColors;
    QVector<QRect> rects;
    QTcpSocket* socket;
    QJsonObject json;
    double kV{ }, MVA{ }, MW{ };
};
#endif // MAINWINDOW_H
