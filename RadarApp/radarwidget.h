#ifndef RADARWIDGET_H
#define RADARWIDGET_H

#include <QWidget>

class Node {
public:
    int pointX;
    int pointY;
    Node* next;
    Node* prev;

    Node(int X, int Y){
        pointX = X;
        pointY = Y;
        next = nullptr;
        prev = nullptr;
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
    Node* last = nullptr;
    int nodeCount = 0;
    int previousAngle = 0;
    int previousDistance = 0;
    bool sweepingRight = true;
    bool leadingPoint = false; //no detection to detection so leading point found
    bool trailingPoint = false; //detection to no detection so trailing point found
    bool samePoint = false; //previousDistance roughly equals currentDistance so same point was detected
};

#endif // RADARWIDGET_H