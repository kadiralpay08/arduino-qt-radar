#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    radar = new RadarWidget(this);
    setCentralWidget(radar);

    serial = new QSerialPort(this);
    serial->setPortName("COM5"); // your actual port
    serial->setBaudRate(QSerialPort::Baud9600);

    if (!serial->open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open port:" << serial->errorString();
    }

    connect(serial, &QSerialPort::readyRead, this, [=]() {
        while (serial->canReadLine()) {
            QByteArray line = serial->readLine().trimmed();
            QList<QByteArray> parts = line.split(',');
            if (parts.size() == 2) {
                int angle = parts[0].toInt();
                int distance = parts[1].toInt();
                radar->updateReading(angle, distance);
            }
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
