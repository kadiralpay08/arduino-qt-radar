#ifndef RADARWIDGET_H
#define RADARWIDGET_H

#include <QWidget>

class Node {
public:
    int pointX;
    int pointY;
    Node* next;

    Node(int X, int Y){
        pointX = X;
        pointY = Y;
        next = nullptr;
    }
};

class RadarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RadarWidget(QWidget *parent = nullptr);

    void updateReading(int angle, int distance); //call this whenever new serial data arrives

    ~RadarWidget()
    {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int currentAngle = 0;
    int currentDistance = -1;
    Node* head = nullptr;
};

#endif // RADARWIDGET_H