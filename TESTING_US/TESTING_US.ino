
#include <HCSR04.h>
#define trigPin 5 //D1
#define echoPin 4 //D2
HCSR04 US(trigPin, echoPin); //(trig pin , echo pin)
void setup() {
Serial.begin(9600);
}

void loop() {
  unsigned int dist = US.dist();
Serial.println(dist);
delay(100);


}
