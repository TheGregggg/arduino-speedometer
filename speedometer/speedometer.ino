#include <Servo.h>

Servo myservo;


unsigned char pos = 0;  // variable to store the servo position
unsigned char serial_speed=10;
unsigned int max_speed = 100;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(2000000);
}

void loop() {
  
  if (Serial.available() > 0) {
    // read the incoming byte:
    serial_speed = (unsigned char)Serial.read();
    
    myservo.write(((unsigned int)serial_speed * 150) / max_speed);
  }
}
