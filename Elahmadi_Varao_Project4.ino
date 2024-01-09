#include <Pixy2.h>

 

#include <Servo.h>

 

// Pins to utilize.

 

const int THROTTLE_PIN = 10;

 

const int STEER_PIN = 11;

 

// Parameters for PD functions. They are given values inside the setup.

 

float SteerKp;

 

float SteerKd;

 

float ThrottleKp;

 

float ThrottleKd;

 

// Constants and variables for PD control.

 

const float AREA_SETPOINT = 3300; // 3ft distance from the leader.

 

const float DISTANCE_SETPOINT = 3;

 

const float CENTERING_SETPOINT = 158.0; // The x value when the detected object is in the middle of the figure.

 

double distance_error;

 

double prev_distance_error;

 

double inclination_error;

 

double prev_inclination_error;

 

int throttling_command;

 

int steering_command;

 

long lastMeasurement1;

 

long lastMeasurement2;

 

// Defining the objects to use during the program execution.

 

Pixy2 Cam;

 

Servo Throttle;

 

Servo Steer;

 

// These variables are only used in order to define the largest area block that the camera can recognize.

 

const int samples = 10; // Defines the number of samples to be collected for the moving average.

 

unsigned int largestArea[samples];

 

unsigned int axisPosition[samples];

 

unsigned int averageArea;

 

unsigned int averageAxis;

 

int decidedBlock;

 

 

 

void setup()

{

 // Here are defined the pins for each one of the servo motors.

 Throttle.attach(THROTTLE_PIN);

 Steer.attach(STEER_PIN);

 float coeficients[4] = {3, 4, 10, 11};

 definePD(coeficients);

 Cam.init();

 Serial.begin(115200);

 lastMeasurement1 = millis();

 lastMeasurement2 = millis();

 prev_distance_error = 0;

 prev_inclination_error = 0;

}

 

void loop()

{

 Cam.ccc.getBlocks();

 if(Cam.ccc.numBlocks)

 {

  defineAverageArea();

  defineAverageAxis();

 }

 //correctThrottle();

 correctSteer();

 //dataLog();

}

 

void definePD(float v[4])

{

 SteerKp = v[0];

 SteerKd = v[1];

 ThrottleKp = v[2];

 ThrottleKd = v[3];

}

 

// The following functions have the purpose of defining the block with the largest area that matches the red paper, this is

// the one the Traxxas will need to follow.

unsigned int Area(int block)

{

 return (Cam.ccc.blocks[block].m_width)*(Cam.ccc.blocks[block].m_height);

}

 

void areaDecider()

{

 int i;

 unsigned int area = Area(0);

 // If there's more than one block detected, defines the decided one as the first one and updates the decision due to finding a larger area.

 for(i = 0; i < Cam.ccc.numBlocks; i++)

 {

  if(Area(i) >= area)

  {

   decidedBlock = i;

  }

 }

}

 

// In order to avoid using derivatives over noisy signals, there's a measurement of the moving average for a specified number of samples.

void defineAverageArea()

{

 averageArea = 0;

 for(int i = 0; i < samples ; i++)

 {

  areaDecider();

  largestArea[i] = Area(decidedBlock);

  averageArea = ++largestArea[i];

 }

 averageArea = averageArea/samples;
 //Serial.println(averageArea);
}

 

void defineAverageAxis()

{

 averageAxis = 0;

 for(int i = 0; i < samples ; i++)

 {

  areaDecider();

  axisPosition[i] = Cam.ccc.blocks[decidedBlock].m_x;

  averageAxis = ++axisPosition[i];

 }

 averageAxis = averageAxis/samples;
 //Serial.println(averageAxis);
}

 

// Here are defined the PD commands for the steering and the throttle.

void correctThrottle()

{

 distance_error = DISTANCE_SETPOINT*(1 - sqrt(AREA_SETPOINT/averageArea));

 double error = ThrottleKp*distance_error + ThrottleKd*(distance_error - prev_distance_error)/(millis() - lastMeasurement1);
 //Serial.println(error);

 if (error >= 500)

 {

  error = 500;

 }

 else if (error <= -500)

 {

  error = -500;

 }

 lastMeasurement1 = millis();

 prev_distance_error = distance_error;

 throttling_command = 1500 - (int)error;

 //Serial.println(throttling_command);

 Throttle.writeMicroseconds(throttling_command);

}

 

void correctSteer()

{

 inclination_error = CENTERING_SETPOINT - averageAxis;

 double error = SteerKp*inclination_error + SteerKd*(inclination_error - prev_inclination_error)/(millis() - lastMeasurement2);
 //Serial.println(error);

 if (error >= 500)

 {

  error = 500;

 }

 else if (error <= -500)

 {

  error = -500;

 }

 lastMeasurement2 = millis();

 prev_inclination_error = inclination_error;

 steering_command = 1500 - (int) error;

 Serial.println(steering_command);

 //Steer.writeMicroseconds(steering_command);

}

 

void dataLog()

{

 Serial.print(lastMeasurement2);

 Serial.print(",");

 Serial.print(averageAxis);

 Serial.print(",");

 Serial.print(steering_command);

 Serial.print(",");

 Serial.print(lastMeasurement1);

 Serial.print(",");  

 Serial.print(averageArea);

 Serial.print(",");

 Serial.println(throttling_command);

}