//Project 2 Code
//Name: Adam Elahmadi
//Professor: Dr. Steckenrider
//Date: 9/21/2023


#include <Servo.h>

//Declare and Initialize Pins
const int rightPin = 7;
const int leftPin = 4;
const int middlePin = 2;
const int greenPin = 12;
const int redPin = 8;
const int oneBallPin = 10;
const int threeBallsPin = 11;
const int buzzerPin = 3;


//Declaring other variables 
bool track = 1;
bool oneBallTracker = 0;
bool threeBallsTracker = 0;
int test = 0;
unsigned int buzzTracker = 0;
unsigned long threshold = 0;
unsigned long thresholdTwo = 0;

//Declaring Servo Objects
Servo oneBall;
Servo threeBalls;



void setup() {
  
  //Setting up LED Pin Types
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  
  //Attaching Servos to Specific Pins
  oneBall.attach(oneBallPin);
  threeBalls.attach(threeBallsPin);
  oneBall.write(0);
  threeBalls.write(0);
  
  Serial.begin(9600);
}

void loop() {
 
  
  //This Block of code calculates the distance in inches that the right ultrasonic sensor reads
  long rightDuration, rightInches;
  
  pinMode(rightPin, OUTPUT);
  digitalWrite(rightPin, LOW);
  delayMicroseconds(2);
  digitalWrite(rightPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(rightPin, LOW);

  pinMode(rightPin, INPUT);
  rightDuration = pulseIn(rightPin, HIGH);
  rightInches = microsecondsToInches(rightDuration);
 


  //This block of code calculates the distance in inches that the left ultrasonic sensor reads
  long leftDuration, leftInches;

  pinMode(leftPin, OUTPUT);
  digitalWrite(leftPin, LOW);
  delayMicroseconds(2);
  digitalWrite(leftPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(leftPin, LOW);

  pinMode(leftPin, INPUT);
  leftDuration = pulseIn(leftPin, HIGH);
  leftInches = microsecondsToInches(leftDuration);
  


  //This block of code calculates the distance that the middle ultrasonic sensor reads
  long middleDuration, middleInches, middleCm;

  pinMode(middlePin, OUTPUT);
  digitalWrite(middlePin, LOW);
  delayMicroseconds(2);
  digitalWrite(middlePin, HIGH);
  delayMicroseconds(5);
  digitalWrite(middlePin, LOW);

  pinMode(middlePin, INPUT);
  middleDuration = pulseIn(middlePin, HIGH);
  middleInches = microsecondsToInches(middleDuration);
  

  //If the middle sensor reads a distance of greater than 20 inches, 
  //then the green light will turn on, and the red light will turn on off
  if (middleInches >= 20)
  {
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
    oneBall.write(0);
    threeBalls.write(0);
    
    //If the one ball has been dropped, then the buzzer will buzz once
    if (oneBallTracker && !threeBallsTracker)
    {
      tone(buzzerPin, 250);
      delay(500);
      noTone(buzzerPin);
      delay(500);
      oneBallTracker = 0;
    }
    
    //If three balls have been dropped, then the buzzer will buzz three times
    if (threeBallsTracker)
    {
      Serial.println(test);
      tone(buzzerPin, 250);
      delay(500);
      noTone(buzzerPin);
      delay(500);
      tone(buzzerPin, 250);
      delay(500);
      noTone(buzzerPin);
      delay(500);
      tone(buzzerPin, 250);
      delay(500);
      noTone(buzzerPin);
      delay(500);
      threeBallsTracker = 0;
    }
   
  }
  
  //If the middle sensor reads a distance of less than 20 inches,
  //then the green light will turn off, and the red light will turn on
  if (middleInches < 20)
  {
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, HIGH);
  
  //either sensor reads one flag, then the one ball servo will be activated
  //and drop 1 ball
 	if ((rightInches < 10 && leftInches > rightInches) || (rightInches > leftInches && leftInches < 10))
    {

      threshold = millis() + 600;
      //while loop to continuously check and see if there is still indeed one flag present since
      //the time it first detected one flag
    	while ((rightInches < 10 && leftInches > rightInches) || (rightInches > leftInches && leftInches < 10))
    	{
      		pinMode(leftPin, OUTPUT);
  	  		digitalWrite(leftPin, LOW);
  	  		delayMicroseconds(2);
  	  		digitalWrite(leftPin, HIGH);
  	  		delayMicroseconds(5);
  	  		digitalWrite(leftPin, LOW);
  	  		pinMode(leftPin, INPUT);
  	  		leftDuration = pulseIn(leftPin, HIGH);
  	  		leftInches = microsecondsToInches(leftDuration);
      
      		pinMode(rightPin, OUTPUT);
 	  		  digitalWrite(rightPin, LOW);
 	  		  delayMicroseconds(2);
  	  		digitalWrite(rightPin, HIGH);
 	  		  delayMicroseconds(5);
 	  		  digitalWrite(rightPin, LOW);
 	  		  pinMode(rightPin, INPUT);
 	  		  rightDuration = pulseIn(rightPin, HIGH);
 	  		  rightInches = microsecondsToInches(rightDuration);

          //If that one flag has been there for a certain amount of time
          //then the one ball servo will be activated and 1 ball will
          //be delivered 
      		if (millis() > threshold)
      		{
        	oneBall.write(100);
      		delay(2000);
      		oneBall.write(0);
     		  delay(4000);
          oneBallTracker = 1;
          break;
      		}
        }
    }

    
    //If both sensors have a flag in front of them,
    //for a total fo 2 flags, then the three balls servo
    //will be initiated and 3 balls will be delivered
    if (rightInches < 10 && leftInches < 10)
    {
      	thresholdTwo = millis() + 600;

        //while loop to continuously check and see if there is still indeed two flags present since
        //the time it first detected two flags
    	  while (rightInches < 10 && leftInches < 10)
    	  {
      		pinMode(leftPin, OUTPUT);
  	  		digitalWrite(leftPin, LOW);
  	  		delayMicroseconds(2);
  	  		digitalWrite(leftPin, HIGH);
  	  		delayMicroseconds(5);
  	  		digitalWrite(leftPin, LOW);
  	  		pinMode(leftPin, INPUT);
  	  		leftDuration = pulseIn(leftPin, HIGH);
  	  		leftInches = microsecondsToInches(leftDuration);
      
      		pinMode(rightPin, OUTPUT);
 	  		  digitalWrite(rightPin, LOW);
 	  		  delayMicroseconds(2);
  	  		digitalWrite(rightPin, HIGH);
 	  		  delayMicroseconds(5);
 	  		  digitalWrite(rightPin, LOW);
 	  		  pinMode(rightPin, INPUT);
 	  		  rightDuration = pulseIn(rightPin, HIGH);
 	  		  rightInches = microsecondsToInches(rightDuration);

          
          //If the two flags have been there for a certain amount of time
          //then the three balls servo will be activated and 3 balls will
          //be delivered 
      		if (millis() > thresholdTwo)
      		{
            Serial.println("moving");
        	  threeBalls.write(100);
            oneBall.write(100);
      		  delay(2000);
            threeBalls.write(0);
      		  oneBall.write(0);
     		    delay(4000);
            threeBallsTracker = 1;
            test = 1;
            break;
      		}
        }
     }
  }

}


//Simple function to convert the measured time 
//the ultrasonic sensor transmitts and recieves a signal
//into a distance (inches)
long microsecondsToInches(long microseconds) {
  
  return microseconds / 74 / 2;
}


