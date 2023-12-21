#define MOTOR_RIGHT_FORWARD 13 //D8    D5-D6-D7-D8->From left to right
#define MOTOR_RIGHT_BACKWARD 15  // D7
#define MOTOR_LEFT_FORWARD 14  // D6
#define MOTOR_LEFT_BACKWARD 12  // D5
int speed = 150;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode (MOTOR_RIGHT_FORWARD,OUTPUT );
  pinMode (MOTOR_RIGHT_BACKWARD,OUTPUT );
  pinMode (MOTOR_LEFT_FORWARD,OUTPUT );
  pinMode (MOTOR_LEFT_BACKWARD,OUTPUT );

}

void loop() {

moveForward(speed);
delay(5000);
stopMotors();
delay(1000);
moveBackward(speed);
delay(5000);
stopMotors();
delay(1000);
moveRight(speed);
delay(5000);
stopMotors();
delay(1000);
moveLeft(speed);
delay(5000);
stopMotors();
delay(1000);
}
void moveRight(int speed) {
  Serial.println("MoveRight");
  analogWrite(MOTOR_RIGHT_FORWARD , 0);
  analogWrite(MOTOR_RIGHT_BACKWARD , 0);
  analogWrite(MOTOR_LEFT_FORWARD , speed);
  analogWrite(MOTOR_LEFT_BACKWARD ,0);
  
}
void moveLeft(int speed) { 
  Serial.println("MoveLeft");
   analogWrite(MOTOR_RIGHT_FORWARD , speed);
   analogWrite(MOTOR_RIGHT_BACKWARD , 0);
   analogWrite(MOTOR_LEFT_FORWARD , 0);
   analogWrite(MOTOR_LEFT_BACKWARD , 0);
}
void moveForward(int speed){
   Serial.println("MoveForward");
   analogWrite(MOTOR_RIGHT_FORWARD , speed);
   analogWrite(MOTOR_RIGHT_BACKWARD , 0);
   analogWrite(MOTOR_LEFT_FORWARD , speed);
   analogWrite(MOTOR_LEFT_BACKWARD , 0);
}
void moveBackward(int speed){
   Serial.println("MoveBackward");
   analogWrite(MOTOR_RIGHT_FORWARD , 0);
   analogWrite(MOTOR_RIGHT_BACKWARD , speed);
   analogWrite(MOTOR_LEFT_FORWARD , 0);
   analogWrite(MOTOR_LEFT_BACKWARD , speed);
}
void stopMotors() {
   Serial.println("Stopmotors");
   analogWrite(MOTOR_RIGHT_FORWARD , 0);
   analogWrite(MOTOR_RIGHT_BACKWARD , 0);
   analogWrite(MOTOR_LEFT_FORWARD , 0);
   analogWrite(MOTOR_LEFT_BACKWARD , 0);
}