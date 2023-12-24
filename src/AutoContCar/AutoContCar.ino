/*******************************************************************************************************************************************************
* ESP8266 Object Avoidance car by Rodina Ehab, Perihane Hossam, Habiba Hatem, Mostafa Khashan, Ahmed Gamal                                                   *
*                                                                                                                                                      *                      
* Controlled with python code over Access point ESP8266 wifi                                                                                           *
* ESP8266 keyboard controlled and autonomous object avoidance car                                                                                      *
* GUI speed controlling and Ultrasonic data observing                                                                                                  * 
*                                                                                                                                                      *
*------------- IDE Libraries-----------------------                                                                                                    *
* ESP8266 wifi library : https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi                                                          *         
* Servo motor library : https://github.com/arduino-libraries/Servo                                                                                     * 
* Ultrasonic sensor library : https://github.com/gamegine/HCSR04-ultrasonic-sensor-lib                                                                 *                     
*                                                                                                                                                      * 
* Python libraries : Tkinter, Keyboard, Request                                                                                                        *
*---------------Helpful links---------------                                                                                                           *
* https://realpython.com/python-gui-tkinter/                                                                                                           *
* https://thepythoncode.com/article/control-keyboard-python                                                                                            *  
* https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html                                                                             *
* https://realpython.com/python-requests/                                                                                                              *
*https://github.com/Modi1987/My_Arduino_Projects/blob/master/ESP8266/Controlling_Servos/Arduino_IDE/ESP8266_controlServoOverWifi/wifiClient_ServoGUI.py*
*                                                                                                                                                      *  
********************************************************************************************************************************************************/
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
//-------SpeedOfMotors------------
int rightSpeedBackward = 90;
int rightSpeedForward = 90;
int leftSpeedForward = 110;
int leftSpeedBackward =110;
int rotSpeed = 100;

 bool automated_motors =false  ;//Variable checks for automated object avoidance mode
// //-------------Servo_Define------------
 Servo servo_sensor;
 #define servoPin 0 //D3 
// //----------------ULTRASONIC_PINS_DEFINE----------
#define trigPin 5 //D1
#define echoPin 4 //D2
HCSR04 US(trigPin, echoPin); //(trig pin , echo pin)





void setup() {
  Serial.begin(9600); // Initiallize Serial monitor with baud rate 9600

  servo_sensor.attach(servoPin);// Attach servo motor with its connected esp pin
//------Set Motors pins as Output------------
  pinMode (MOTOR_RIGHT_FORWARD,OUTPUT );
  pinMode (MOTOR_RIGHT_BACKWARD,OUTPUT );
  pinMode (MOTOR_LEFT_FORWARD,OUTPUT );
  pinMode (MOTOR_LEFT_BACKWARD,OUTPUT );
 
 //-----------Wi-Fi Setup (SettingESP as access point)-----------
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
/*
  @brief : this function is always listening for incoming data on server of ES8266 and then calling its handle function
  @param (1st): route of data on the server 
  @param (2nd): Transmition method of data either post (receive) or get(send) to ESP
  @param (3rd): Calling handle function that its always called when a new data is come
*/
  server.on("/getData", HTTP_POST, handlePostData);    //(Place,Method,CallingFn)
  server.on("/rightSpeedForward", HTTP_POST, handleRightSpeedForward);
  server.on("/rightSpeedBackward", HTTP_POST, handleRightSpeedBackward);
  server.on("/leftSpeedForward", HTTP_POST, handleLeftSpeedForward);
  server.on("/leftSpeedBackward", HTTP_POST, handleLeftSpeedBackward);
  server.on("/servoAngle", HTTP_POST, handleServoMotor);
  server.on("/rotSpeed", HTTP_POST, handleRotSpeed);
  server.on("/sendData", HTTP_GET,handleGetData);

  server.begin();// This function makes ESP enable to listen for incoming requests

}

void loop() {
   server.handleClient(); // This function handles clients and thier requests continously
   //----------Object Avoidance Part---------------------------
   if(automated_motors==true){
      servo_sensor.write(90);
      delay(50);
      unsigned int dist = US.dist();
      Serial.println("Autonomous Time :)");
      Serial.println(dist);
   moveForward(leftSpeedForward,rightSpeedForward);
   if (dist < 35){
     stopMotors();
     delay(100);
    moveBackward(leftSpeedBackward,rightSpeedBackward);
    delay(500);
    stopMotors();
    delay(100);
     servo_sensor.write(0);
     delay(500);
     unsigned int dist_right = US.dist();
     
     servo_sensor.write(180); 
     delay(500);
     unsigned int dist_left = US.dist();
     
     if ( dist_right > dist_left)
     {
       moveRight(rotSpeed) ;
       delay(300);
       stopMotors();
       delay(100);
     }
     else if (dist_right< dist_left){
       moveLeft(rotSpeed);
       delay(300);
       stopMotors();
       delay(100);
     }
     else if (dist_right<35 && dist_left <35)
     {
       moveLeft(rotSpeed);
       delay(600);
       stopMotors();
       delay(100);
     }
   }
   }
}
/*
@breif handlePostData(): Receives the incoming data and comparing it to get motors state
*/
void handlePostData() {
  String receivedData = server.arg("data");// Function recieves data 
  Serial.println("Received data: " + receivedData);
/*
 @brief: this function is a response from receiver (ESP) to transmitter to ensure that data transmitted successfully
 @param (1st): This is the status of sending data either 200(successfully) or 404(unsuccessfully)
 @param (2nd): State that the received data was text
 @param (3rd): Data sent
*/
  server.send(200, "text/plain","Received data: " + receivedData);

  if(receivedData == "forward" || receivedData == "backward" || receivedData == "right" || receivedData == "left" || receivedData == "stop")
  {
    handleMotors(receivedData);
    automated_motors=false;
  }
  if(receivedData=="auto"){
    automated_motors=true;
  }
  
  }
  //Function that sends data from ESP to console
  void handleGetData(){
    unsigned int distance = US.dist();
    String dataToSend = String(distance); 
    server.send(200, "text/plain", dataToSend);
    
  }
  //--------Functions receives motors speed and servo angle----------
  void handleRightSpeedForward()
  {
  String receivedData = server.arg("data");
  Serial.println("Received data: " + receivedData);
  server.send(200, "text/plain","Received data: " + receivedData);
  rightSpeedForward=receivedData.toInt();
  }
   void handleRightSpeedBackward()
  {
  String receivedData = server.arg("data");
  Serial.println("Received data: " + receivedData);
  server.send(200, "text/plain","Received data: " + receivedData);
  rightSpeedBackward=receivedData.toInt();
  }
   void handleRotSpeed()
  {
  String receivedData = server.arg("data");
  Serial.println("Received data: " + receivedData);
  server.send(200, "text/plain","Received data: " + receivedData);
  rotSpeed=receivedData.toInt();
  }
  void handleLeftSpeedForward()
  {
  String receivedData = server.arg("data");
  Serial.println("Received data: " + receivedData);
  server.send(200, "text/plain","Received data: " + receivedData);
  leftSpeedForward=receivedData.toInt();
  }
    void handleLeftSpeedBackward()
  {
  String receivedData = server.arg("data");
  Serial.println("Received data: " + receivedData);
  server.send(200, "text/plain","Received data: " + receivedData);
  leftSpeedBackward=receivedData.toInt();
  }
  void handleServoMotor()
  {
  String receivedData = server.arg("data");
  Serial.println("Received data: " + receivedData);
  server.send(200, "text/plain","Received data: " + receivedData);
  int servoAngle=receivedData.toInt();
  servo_sensor.write(servoAngle);
  }
//---------------------------------------------------
// Function that switches between motor state according to 1st letter of received data 
void handleMotors(String state)
{
  switch (state[0])
  {
    case 'f':
     moveForward(leftSpeedForward,rightSpeedForward);
      break;

    case 'b':
      moveBackward(leftSpeedBackward,rightSpeedBackward);
      break;

    case 'l':
       moveLeft(rotSpeed);
       break;
    case 'r':
      moveRight(rotSpeed);
      break;
    case 's':
      stopMotors();
      break;
    
}
}

//-----------Controlling functions of Motors--------------------------
void moveRight(int rotSpeed) {
  Serial.print("MoveRight with speed: ");
  Serial.println(rotSpeed);
  analogWrite(MOTOR_RIGHT_FORWARD , 0);
  analogWrite(MOTOR_RIGHT_BACKWARD , 0);
  analogWrite(MOTOR_LEFT_FORWARD , rotSpeed);
  analogWrite(MOTOR_LEFT_BACKWARD ,0);
  
}
void moveLeft(int rotSpeed) { 
  Serial.print("MoveLeft with speed: ");
  Serial.println(rotSpeed);
   analogWrite(MOTOR_RIGHT_FORWARD , rotSpeed);
   analogWrite(MOTOR_RIGHT_BACKWARD , 0);
   analogWrite(MOTOR_LEFT_FORWARD , 0);
   analogWrite(MOTOR_LEFT_BACKWARD , 0);
}
void moveForward(int leftSpeedForward,int rightSpeedForward){
   Serial.print("MoveForward with Left,Right speed: ");
   Serial.print(leftSpeedForward);
   Serial.print(", ");
   Serial.println(rightSpeedForward);
   analogWrite(MOTOR_RIGHT_FORWARD , rightSpeedForward);
   analogWrite(MOTOR_RIGHT_BACKWARD , 0);
   analogWrite(MOTOR_LEFT_FORWARD , leftSpeedForward);
   analogWrite(MOTOR_LEFT_BACKWARD , 0);
}
void moveBackward(int leftSpeedBackward,int rightSpeedBackward){
   Serial.print("MoveBackward with Left,Right speed: ");
   Serial.print(leftSpeedBackward);
   Serial.print(", ");
   Serial.println(rightSpeedBackward);
   analogWrite(MOTOR_RIGHT_FORWARD , 0);
   analogWrite(MOTOR_RIGHT_BACKWARD , rightSpeedBackward);
   analogWrite(MOTOR_LEFT_FORWARD , 0);
   analogWrite(MOTOR_LEFT_BACKWARD , leftSpeedBackward);
}
void stopMotors() {
   Serial.println("Stopmotors");
   analogWrite(MOTOR_RIGHT_FORWARD , 0);
   analogWrite(MOTOR_RIGHT_BACKWARD , 0);
   analogWrite(MOTOR_LEFT_FORWARD , 0);
   analogWrite(MOTOR_LEFT_BACKWARD , 0);
}
                                                                          
                                                                                                             
