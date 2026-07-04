#include "radarwidget.h"
#include <QPainter>
#include <cmath>

const int MAX_NODES = 360;

RadarWidget::RadarWidget(QWidget *parent) : QWidget(parent)
{
}

void RadarWidget::updateReading(int angle, int distance)
{
    currentAngle = angle;
    currentDistance = distance;

    if (currentAngle > previousAngle){
        sweepingRight = true;
    } else {
        sweepingRight = false;
    }
    //sweepingRight = (angle > previousAngle);

    previousAngle = angle;

    //check************************
    //if (previousDistance == 0)

    if (previousDistance == -1 && currentDistance != -1){
        leadingPoint = true;
    } else if (previousDistance != -1 && currentDistance == -1){
        trailingPoint = true;
    } else if (previousDistance - currentDistance <= 2 && previousDistance - currentDistance >= -2){ //2 cm uncertainty
        samePoint = true;
    }

    previousDistance = distance;

    int centerX = width() / 2;
    //int centerY = height() / 2;

    //if currentDistance != -1, convert (currentAngle, currentDistance) to an (x, y) point relative to center
    if (distance != -1) {
        double angleX = std::cos(angle * M_PI / 180);
        double angleY = std::sin(angle * M_PI / 180);

        Node* newNode = new Node(centerX - (2 * distance * angleX), height() - (2 * distance * angleY)); //2 pixels is 1 cm
        newNode->next = head;
        newNode->prev = nullptr;

        if (newNode->next != nullptr){
            newNode->next->prev = newNode;
        }
        head = newNode;

        if (last == nullptr){
            last = newNode;
        }
        nodeCount++;

        if (nodeCount >= MAX_NODES){
            Node* temp = last;
            last = last->prev;
            last->next = nullptr;
            delete temp;
            nodeCount--;
        }
    }

    update(); // tells Qt to call paintEvent again with the new data
}

void RadarWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int centerX = width() / 2;
    //int centerY = height() / 2;

    //draws a few concentric circles centered on (centerX, centerY) as range rings
    painter.drawEllipse(centerX - 50, height() - 50, 100, 100); //25 cm
    painter.drawEllipse(centerX - 100, height() - 100, 200, 200); //50 cm
    painter.drawEllipse(centerX - 200, height() - 200, 400, 400); //100 cm
    painter.drawEllipse(centerX - 400, height() - 400, 800, 800); //200 cm

    QFont font = painter.font();
    font.setPixelSize(14);
    painter.setFont(font);
    painter.drawText(centerX + 52, height(), "25cm");
    painter.drawText(centerX + 102, height(), "50cm");
    painter.drawText(centerX + 202, height(), "100cm");
    painter.drawText(centerX + 356, height(), "200cm");

    //draws a line from (centerX, centerY) outward at angle currentAngle
    painter.setPen(Qt::red);
    /*float angleX1 = std::cos(currentAngle * (M_PI/180) - 10);
    float angleY1 = std::sin(currentAngle * (M_PI/180) - 10);
    float angleX2 = std::cos(currentAngle * (M_PI/180) + 10);
    float angleY2 = std::sin(currentAngle * (M_PI/180) + 10);

    float angleX3 = std::cos(currentAngle * (M_PI/180) -8);
    float angleY3 = std::sin(currentAngle * (M_PI/180) -8);

    painter.drawLine(centerX, centerY, -400 * angleX1 + centerX, -400 * angleY1 + centerY);
    painter.drawLine(centerX, centerY, -400 * angleX2 + centerX, -400 * angleY2 + centerY);
    painter.drawLine(centerX, centerY, -400 * angleX3 + centerX, -400 * angleY3 + centerY);*/


    for (int i = 0; i <=200; i++){
        if (sweepingRight){
            painter.setOpacity(0 + (i/200.0));
        } else {
            painter.setOpacity(1 - (i/200.0));
        }

        painter.drawLine(centerX, height(), -400 * (std::cos((currentAngle - 10 + (i/10.0)) * (M_PI/180))) + centerX, -400 * (std::sin((currentAngle - 10 + (i/10.0)) * (M_PI/180))) + height());
    }

    //draws stored points
    painter.setOpacity(1);
    painter.setPen(Qt::green);
    Node* tempNode = head;
    while(tempNode != nullptr){
        painter.drawPoint(tempNode->pointX, tempNode->pointY);
        tempNode = tempNode->next;
    }
}