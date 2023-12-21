

//--------LibrariesIncluded----------
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include<Servo.h>
#include <HCSR04.h>
//------------WIFI_INFO-------------
  const char* ssid = "TeamMecha";
  const char* password = "123456789";
  ESP8266WebServer server(80);
  
//-------------MOTOR_PINS_DEFINE-----------------
#define MOTOR_RIGHT_FORWARD 13 //D8    D5-D6-D7-D8->From left to right
#define MOTOR_RIGHT_BACKWARD 15  // D7
#define MOTOR_LEFT_FORWARD 14  // D6
#define MOTOR_LEFT_BACKWARD 12  // D5
 bool automated_motors =false;
// //-------------Servo_Define------------
 Servo servo_sensor;
 #define servoPin 0 //D3 
// //----------------ULTRASONIC_PINS_DEFINE----------
#define trigPin 5 //D1
#define echoPin 4 //D2
HCSR04 US(trigPin, echoPin); //(trig pin , echo pin)

int speed = 150;
int coeff = 1;


void setup() {
  Serial.begin(9600);

  servo_sensor.attach(servoPin);
  pinMode (MOTOR_RIGHT_FORWARD,OUTPUT );
  pinMode (MOTOR_RIGHT_BACKWARD,OUTPUT );
  pinMode (MOTOR_LEFT_FORWARD,OUTPUT );
  pinMode (MOTOR_LEFT_BACKWARD,OUTPUT );
 
 //-----------Wifi controlling --------------
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.on("/getData", HTTP_POST, handlePostData); //(Place,Method,CallingFn)
  server.on("/sendData", HTTP_GET,handleGetData );
  server.begin();

}

void loop() {
   server.handleClient();
   }

void handlePostData() {
  String receivedData = server.arg("data");
  Serial.println("Received data: " + receivedData);
  server.send(200, "text/plain","Received data: " + receivedData);
  if(receivedData == "forward" || receivedData == "backward" || receivedData == "right" || receivedData == "left" || receivedData == "stop")
  {
    handleMotors(receivedData);
  }
  
  }
  void handleGetData()
  { unsigned int dist = US.dist();
    String dataToSend = String(dist); // Data to send to Python script
    server.send(200, "text/plain", dataToSend);
    Serial.println("Automatedone");
  
   Serial.println(dist);
   servo_sensor.write(90);
   delay(50);
   moveForward(speed);
   if (dist < 20){

     stopMotors();
    moveBackward(speed);
    delay(500);
    
     servo_sensor.write(0);
     float dist_right = dist;
     servo_sensor.write(180); 
     float dist_left = dist;
     if ( dist_right > dist_left)
     {
       moveRight(speed) ;
     }
     else {
       moveLeft(speed);
     }
    delay(500);
  }
  }


void handleMotors(String state)
{
  switch (state[0])
  {
    case 'f':
     moveForward(speed);
      break;

    case 'b':
      moveBackward(speed);
      break;

    case 'l':
       moveLeft(speed);
       break;
    case 'r':
      moveRight(speed);
      break;
    case 's':
      stopMotors();
      break;
    
}
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
   analogWrite(MOTOR_LEFT_BACKWARD , speed);
}

