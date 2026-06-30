#include "radarwidget.h"
#include <QPainter>
#include <cmath>

RadarWidget::RadarWidget(QWidget *parent) : QWidget(parent)
{
}

void RadarWidget::updateReading(int angle, int distance)
{
    currentAngle = angle;
    currentDistance = distance;

    int centerX = width() / 2;
    int centerY = height() / 2;

    //if currentDistance != -1, convert (currentAngle, currentDistance) to an (x, y) point relative to center
    if (distance != -1) {
        double angleX = std::cos(angle * M_PI / 180);
        double angleY = std::sin(angle * M_PI / 180);
        Node* newNode = new Node(centerX - (distance * angleX), centerY - (distance * angleY));
        newNode->next = head;
        head = newNode;
    }

    update(); // tells Qt to call paintEvent again with the new data
}

void RadarWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int centerX = width() / 2;
    int centerY = height() / 2;

    //draws a few concentric circles centered on (centerX, centerY) as range rings and stored points
    painter.drawEllipse(centerX - 50, centerY - 50, 100, 100);
    painter.drawEllipse(centerX - 100, centerY - 100, 200, 200);
    painter.drawEllipse(centerX - 200, centerY - 200, 400, 400);
    painter.drawEllipse(centerX - 400, centerY - 400, 800, 800);
    painter.setPen(Qt::green);
    Node* tempNode = head;
    while(tempNode != nullptr){
        painter.drawPoint(tempNode->pointX, tempNode->pointY);
        tempNode = tempNode->next;
    }


    //draws a line from (centerX, centerY) outward at angle currentAngle
    int angleX = std::cos(currentAngle * (M_PI/180));
    int angleY = std::sin(currentAngle * (M_PI/180));
    painter.setPen(Qt::red);
    painter.drawLine(centerX, centerY, 400 * angleX + centerX, 400 * angleY + centerY);
}