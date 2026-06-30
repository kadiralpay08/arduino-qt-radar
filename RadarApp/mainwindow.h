#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "radarwidget.h"
#include <QMainWindow>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    RadarWidget *radar;

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
};
#endif // MAINWINDOW_H
