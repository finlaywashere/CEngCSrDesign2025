#include <AFMotor_R4.h>

#define SENSOR_PIN 11

#define OFF_SPEED 0
#define SLOW_SPEED 200
#define FAST_SPEED 255

#define ON_LINE_VALUE 1

#define ITER_COUNT 4
#define REACQUIRE_TIME 1000

int leftSpeed = 0;
int rightSpeed = 0;

AF_DCMotor left(3);
AF_DCMotor right(4);

bool checkLine(){
  int sensorVal = digitalRead(SENSOR_PIN);
  Serial.print("Sensor value: ");
  Serial.println(sensorVal);
  return sensorVal == ON_LINE_VALUE;
}

void motorsOff(){
  left.setSpeed(OFF_SPEED);
  right.setSpeed(OFF_SPEED);
  left.run(RELEASE);
  right.run(RELEASE);
}

void motorsLeft(){
  left.setSpeed(SLOW_SPEED);
  right.setSpeed(OFF_SPEED);
  left.run(FORWARD);
  right.run(RELEASE);
}
void motorsRight(){
  left.setSpeed(OFF_SPEED);
  right.setSpeed(SLOW_SPEED);
  left.run(RELEASE);
  right.run(FORWARD);
}

void motorsForward(){
  left.setSpeed(FAST_SPEED);
  right.setSpeed(FAST_SPEED);
  left.run(FORWARD);
  right.run(FORWARD);
}
void motorsBackwards(){
  left.setSpeed(FAST_SPEED);
  right.setSpeed(FAST_SPEED);
  left.run(BACKWARD);
  right.run(BACKWARD);
}

void reacquireLine(){
  if(checkLine()){
    return;
  }
  Serial.println("Reacquiring");
  motorsLeft();
  for(int i = 0; i < ITER_COUNT; i++){
    delay(REACQUIRE_TIME/ITER_COUNT);
    if(checkLine()){
      motorsOff();
      Serial.println("Left");
      return;
    }
  }
  motorsRight();
  for(int i = 0; i < ITER_COUNT * 2; i++){
    delay(REACQUIRE_TIME/ITER_COUNT);
    if(checkLine()){
      motorsOff();
      Serial.println("Right");
      return;
    }
  }
  Serial.println("Failed to reacquire");
}

void setup() {
  motorsOff();
  pinMode(SENSOR_PIN, INPUT);
  Serial.begin(115200);
}

void loop() {
  reacquireLine();
  motorsForward();
}
