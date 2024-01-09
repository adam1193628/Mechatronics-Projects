#include <Servo.h>

int depthSensorPin = 7;
int throttlePin = 5;
int steerPin = 6;
int interruptAPin = 2;
int interruptBPin = 3;
int counter = 0;
int firstServoPin = 11;
int secondServoPin = 10;

static volatile byte lastA = 0;
static volatile byte lastB = 0;
bool direction = false;

Servo steer;
Servo throttle;
Servo firstLever;
Servo secondLever;


void setup() {
  
  steer.attach(steerPin);
  throttle.attach(throttlePin);
  firstLever.attach(firstServoPin);
  secondLever.attach(secondServoPin);
  steer.writeMicroseconds(1500);
  throttle.writeMicroseconds(2000);
  pinMode(interruptAPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptAPin), count, CHANGE);
  pinMode(interruptBPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptBPin), count, CHANGE);

  Serial.begin(115200);
}



void loop() {
  throttle.writeMicroseconds(2000);
  while (true){
    //delay(1000);
    //throttle.writeMicroseconds(2000);
    Serial.println(counter);
  }
  //throttle.writeMicroseconds(2000);

  /*
  if (counter % 1200 == 0)
  {
    firstLever.write(45);
    delay(1000);
    firstLever.write(90);
    delay(1000);
    firstLever.write(45);
    delay(1000);

    secondLever.write(180);
    delay(1000);
    secondLever.write(120);
    delay(1000);
    secondLever.write(180);
    delay(1000);

  }
  //else
  //{
    
  //}
  //Serial.println(counter);

  */
}

void count() {
  counter++;
  int value1 = digitalRead(interruptAPin);
  int value2 = digitalRead(interruptBPin);
  Serial.print(value1);
  Serial.print(" ");
  Serial.println(value2);
}
