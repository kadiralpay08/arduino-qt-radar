#include <Servo.h>

const int trigPin = 9;
const int echoPin = 10;
const int servoPin = 6;

Servo radarServo;

long getDistanceCM();

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  radarServo.attach(servoPin);
}

void loop() {
  for (int angle = 0; angle <= 180; angle += 2){
    radarServo.write(angle);
    delay(40);
    Serial.print(angle);
    Serial.print(",");
    Serial.println(getDistanceCM());
  }

  for (int angle = 180; angle >= 0; angle -=2){
    radarServo.write(angle);
    delay(40);
    Serial.print(angle);
    Serial.print(",");
    Serial.println(getDistanceCM());
  }
}

long getDistanceCM(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0){
    return -1; //no echo recieved
  }

  long distance = duration * 0.0343 / 2;
  return distance;
}