// Name: Adam Elahmadi
// Professor: Dr. Steckenrider
// Project 1
// Date: 9/8/2023



#include <Servo.h>

//Declaring servo objects
Servo throttle;
Servo steer;   

//Declaring & initializing pin variables
int steerServoPin = 11; 
int throttleServoPin = 10; 
int ledOnePin = 6;
int ledTwoPin = 5;
int armButtonPin = 2;
int secondButtonPin = 4;
int potPin = A0;  `

//Declaring & initializing other variables
int potValue;
int armButtonState;
int secondButtonState;
int lastSecondButtonState = LOW;
int selfDriveMode = LOW;
int armMode = LOW;
int lastArmButtonState = LOW;
int currentReadingTwo;

//Declaring & intializing debounce variables
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long lastDebounceTimeTwo = 0;
unsigned long debounceDelayTwo = 50;





void setup() {

  //Assigning pins to their proper servo objects
  throttle.attach(throttleServoPin);
  steer.attach(steerServoPin);

  //Classifying pins as either input or output
  pinMode(secondButtonPin, INPUT);
  pinMode(armButtonPin, INPUT);
  pinMode(ledOnePin, OUTPUT);
  pinMode(ledTwoPin, OUTPUT);
  Serial.begin(9600);   
}





void loop(){

  //Debouncing for arm button (Used Arduino IDE's Debounce Example for this)
  int currentReading = digitalRead(armButtonPin);
  if (currentReading != lastArmButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentReading != armButtonState) {
      armButtonState = currentReading;
      if (armButtonState == HIGH) {
        armMode = !armMode;
      }
    }
  }
  
  //If armMode is LOW, the robot gets disarmed
  if (armMode == 0){
    disarm(); //Calls the disarm() function
  }
  
  //Robot set to armed mode when armed button is pressed
  else if (armMode == 1 && selfDriveMode == 0){

    //Debouncing for search mode button (Used Arduino IDE's Debounce Example for this)
    currentReadingTwo = digitalRead(secondButtonPin);
  	if (currentReadingTwo != lastSecondButtonState) {
    	lastDebounceTimeTwo = millis();
  	}
  	if ((millis() - lastDebounceTimeTwo) > debounceDelayTwo) {
    	if (currentReadingTwo != secondButtonState) {
      	secondButtonState = currentReadingTwo;
      		if (secondButtonState == HIGH) {
        		selfDriveMode = !selfDriveMode;
      		}
    	}
  	}
    
    //If the search mode button is not pressed, then it will still be in armed mode
    if (selfDriveMode == 0){
      
      //Allows user to control throttle with the potentiometer
    	potValue = analogRead(potPin); 
    	potValue = map(potValue, 0, 1023, 1000, 2000); //Sets new lower and upper boundaries for potentiometer value
    	throttle.writeMicroseconds(potValue);
    	delay(15);

      //If potentiometer is below the halfway point, the green LED lights up
    	if (potValue < 1500){
      		digitalWrite(ledTwoPin, LOW);
      		digitalWrite(ledOnePin, HIGH);
    	}
      //If potentiometer is above the halfway point, the red LED lights up
    	if (potValue > 1500){
      		digitalWrite(ledTwoPin, HIGH);
      		digitalWrite(ledOnePin, LOW);
    	}
      //If potentiometer is at the halfway point, none of the LEDs light up
    	if (potValue == 1500){
      		digitalWrite(ledTwoPin, LOW);
      		digitalWrite(ledOnePin, LOW);
    	}
  	}
    
    //If search button is pressed, the robot will execute its search pattern
    else if (selfDriveMode == 1){

      //Turns off both LEDs
      digitalWrite(ledTwoPin, LOW);
    	digitalWrite(ledOnePin, LOW);

      //Straightens out the wheels
    	steer.writeMicroseconds(1500);

      //Throttle speed set to half-full speed for 2 seconds
    	throttle.writeMicroseconds(1750);
    	delay(2000);
      
      //Robot turns half-right with half-full speed for one second
    	throttle.writeMicroseconds(1750);
    	steer.writeMicroseconds(1750);
    	delay(1000);

      //Robot turns half-left with half-full speed for one second 
    	steer.writeMicroseconds(1250);
    	throttle.writeMicroseconds(1750);
    	delay(1000);

      //Calls the disarm() function and disarms the robot
    	disarm();

      //Resets selfDriveMode and armMode conditions
    	selfDriveMode = 0;
    	armMode = 0;
    }
  }
  
  //Updates last button states for debouncing to work correctly
  lastArmButtonState = currentReading;
  lastSecondButtonState = currentReadingTwo;
}



//The disarm function disarms the robot
void disarm(){

  //Stops the robot from moving forward or backwards
	throttle.writeMicroseconds(1500);

  //Makes LEDs blink and alternate
  digitalWrite(ledOnePin, LOW);
  digitalWrite(ledTwoPin, HIGH);
  delay(100);
  digitalWrite(ledOnePin, HIGH);
  digitalWrite(ledTwoPin, LOW);
  delay(100);
}


