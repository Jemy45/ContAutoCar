#include <Servo.h>

#define servoPin 0
Servo myServo; 

void setup() {
  myServo.attach(servoPin); 
  Serial.begin(9600);
}

void loop() {
  
    myServo.write(0); 
    delay(2000); 
    
    myServo.write(90); 
    delay(2000); 
    
    myServo.write(180); 
    delay(2000); 
}
