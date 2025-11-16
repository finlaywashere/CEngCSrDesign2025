#include <AFMotor_R4.h>

#define SENSOR_PIN 11

#define OFF_SPEED 0
#define SLOW_SPEED 200
#define FAST_SPEED 255

#define ON_LINE_VALUE 1

#define ITER_COUNT 40
#define REACQUIRE_TIME 1000

int leftSpeed = 0;
int rightSpeed = 0;

AF_DCMotor left(3);
AF_DCMotor right(4);

bool checkLine(){
  // Check if we are on the line
  int sensorVal = digitalRead(SENSOR_PIN);
  Serial.print("Sensor value: ");
  Serial.println(sensorVal);
  return sensorVal == ON_LINE_VALUE;
}

void motorsOff(){
  // Stop
  left.setSpeed(OFF_SPEED);
  right.setSpeed(OFF_SPEED);
  left.run(RELEASE);
  right.run(RELEASE);
}

void motorsLeft(){
  // Go left
  left.setSpeed(SLOW_SPEED);
  right.setSpeed(OFF_SPEED);
  left.run(FORWARD);
  right.run(RELEASE);
}
void motorsRight(){
  // Go right
  left.setSpeed(OFF_SPEED);
  right.setSpeed(SLOW_SPEED);
  left.run(RELEASE);
  right.run(FORWARD);
}

void motorsForward(){
  // Go forwards
  left.setSpeed(FAST_SPEED);
  right.setSpeed(FAST_SPEED);
  left.run(FORWARD);
  right.run(FORWARD);
}
void motorsBackwards(){
  // Go back
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
  // Turns left and periodically checks if its above the line
  motorsLeft();
  for(int i = 0; i < ITER_COUNT; i++){
    delay(REACQUIRE_TIME/ITER_COUNT);
    if(checkLine()){
      motorsOff();
      Serial.println("Left");
      return;
    }
  }
  // Turns right and periodically checks if it is above the line
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
  // Start with motors off
  motorsOff();
  pinMode(SENSOR_PIN, INPUT);
  Serial.begin(115200);
}

void loop() {
  // Find line and move forwards
  reacquireLine();
  motorsForward();
}
