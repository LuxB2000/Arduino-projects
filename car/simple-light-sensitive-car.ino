/*
  Basic car, just move then stops. After, it moves its head along 4 positions.
  It Measures the light intensity. It stocks the min and max values and 
  corresponding head position. Finally, the robot moves along the maximal
  light intensity direction.
 */

#include <Servo.h>

int rightMotorPin = 9;
int leftMotorPin = 8;

int headMotorPin = 10;
int INIT_HEAD_POS = 90;
Servo servoHead;

int lightSensorPin = 0;

int go=1;
int initHeadB = 0;

/* -----
 * SETUP
 * -----
 */
void setup() {
  pinMode(rightMotorPin, OUTPUT); // right motor
  pinMode(leftMotorPin, OUTPUT); // left motor
  
  Serial.begin(9600);
  pinMode(lightSensorPin,INPUT); // light sensor
  
  servoHead.attach(headMotorPin);
}

/*
 * MOVE FORWARD for S seconds then stops
 */
void moveForward(float s){
  digitalWrite(rightMotorPin, HIGH);
  digitalWrite(leftMotorPin, HIGH);
  delay( s*1000 );
  stopEngin();
}

/*
 * MOVE RIGHT for s seconds then stops
 */
void moveRight(float s){
  digitalWrite(leftMotorPin,HIGH);
  delay( s*1000);
  stopEngin();
}

/*
 * MOVE LEFT for S seconds then stops
 */
void moveLeft(float s){
  digitalWrite(rightMotorPin,HIGH);
  delay( s*1000 );
  stopEngin();
}

/*
 * STOP both motors
 */
void stopEngin(){
  digitalWrite(rightMotorPin, LOW);
  digitalWrite(leftMotorPin, LOW);
}


/*
 * Move the head in the initial position
 */
void initHead()
{
  servoHead.write( INIT_HEAD_POS );
}

/*
 * Turn head with ANG degree [0,180]
 */
void turnHead(int ang)
{
  servoHead.write(ang);
}
 
/*
 * Choose a direction
 */
int chooseDirection(){
  int maxLight=0, lightMeasurmt=0, ang=0, maxDir=-1, i=0, 
      minLight=1000, minDir=-1;
  
  // turn the captor along 4 directions
  for( i=0; i<3; i++ )
  {
    ang = (i+1) * 45;
    turnHead( ang );
    delay(500);
    lightMeasurmt = analogRead( lightSensorPin );
    Serial.print("Light measured at pos ");
    Serial.print(i);
    Serial.print(" : ");
    Serial.println(lightMeasurmt);
    
    if( lightMeasurmt > maxLight ){
      maxDir = i;
      maxLight = lightMeasurmt;
    }
    
    if( lightMeasurmt < minLight ){
      minDir = i;
      minLight = lightMeasurmt;
    }
      
  }
   // go in the default position
  initHead();
    
  // return the best direction
  Serial.print( ">> Max value: ");
  Serial.print(maxLight);
  Serial.print( " corresponding direction: ");
  Serial.println( ((maxDir+1)*45 ) );
  
  Serial.print( ">> Min value: ");
  Serial.print(minLight);
  Serial.print( " corresponding direction: ");
  Serial.println( ((minDir+1)*45 ) );
  
  Serial.print( ">> Gradient along the head positions: " );
  Serial.println( maxLight-minLight );
  
  //TODO: if the gradient is too smal (espilon) we can decide to stay at the
  //  current location. (we already reach the maximal intensity position );
  
  return (maxDir);
}

/*
 * LOOP
 */
void loop() {
  delay(1000);
  if( initHeadB==0 )
  {
    initHead();
    initHeadB = 1;
  }
  
  int dir = chooseDirection();
  Serial.println(dir);

  
  if( dir == 0)
  {
    // move on the left
    moveRight(1.5);
    moveForward(2);
  }else
  if( dir == 1 ){
    // move forward
    moveForward(3.5);
  }else if(dir==2){
    // move on the right
    moveLeft(1.5);
    moveForward(2);
  }else{
    // stay in place
    Serial.println("No choice can be made.");
  }
}
