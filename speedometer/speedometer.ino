#define RegisterCLK 8
#define Digit0 2
#define Digit1 3
#define Digit2 4
#define ledPin 13
#define SERIAL_BUFFER_SIZE 3

#include <SPI.h>
#include <Servo.h>

Servo myservo;


unsigned char Servo_pos = 0;  // variable to store the servo position
unsigned char serial_rpm=10;
unsigned int serial_speed=10;
unsigned int max_rpm = 100;

unsigned int NUMBER_TO_LED[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111 //9
};

unsigned int CURRENT_DIGIT = 0;

unsigned int SPEED[3] = {0,0,1};


void setSPEED_BCD(unsigned int nb){
  SPEED[2] = nb/100;
  SPEED[1] = (nb%100)/10;
  SPEED[0] = (nb%10);
}

void setupTimer2(){
  // setup the Timer 2 for a 300 Hz clock to switch into the next digit
  noInterrupts();           // disable all interrupts
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;

  OCR2A = 208;            // compare match register 16MHz/(256*300Hz)
  TCCR2B |= (1 << WGM22);   // CTC mode
  TCCR2B |= (1 << CS22);    
  TCCR2B |= (1 << CS21);    
  TCCR2B |= (0 << CS20);    // 256 prescaler 
  TIMSK2 |= (1 << OCIE2A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

void setup() {
  // Setup Register CLK
  pinMode(RegisterCLK, OUTPUT);
  digitalWrite(RegisterCLK, 0);

  // Setup Digits pins
  pinMode(Digit0, OUTPUT);
  pinMode(Digit1, OUTPUT);
  pinMode(Digit2, OUTPUT);

  digitalWrite(Digit0, 1);
  digitalWrite(Digit1, 1);
  digitalWrite(Digit2, 1);

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(2000000);


  SPI.begin();
  SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));
 
  setupTimer2();
}

ISR(TIMER2_COMPA_vect)          // interrupt being called at 300hz 
{
  CURRENT_DIGIT = (CURRENT_DIGIT+1)%3; // light up the right digit in the 4 digit 8 segment display
  switch (CURRENT_DIGIT){
    case 0:
    digitalWrite(Digit2, 1);
    digitalWrite(Digit0, 0);
    break;
    
    case 1:
    digitalWrite(Digit0, 1);
    digitalWrite(Digit1, 0);
    break;
    
    case 2:
    digitalWrite(Digit1, 1);
    digitalWrite(Digit2, 0);
    break;
  }
  
  SPI.transfer(NUMBER_TO_LED[SPEED[CURRENT_DIGIT]]);  // send the new bytes to the serial in parallel out
  digitalWrite(RegisterCLK, 1); // register the new command into the  serial in parallel out shifter
  digitalWrite(RegisterCLK, 0);

  
}
unsigned char serial_buffer[SERIAL_BUFFER_SIZE];
unsigned char read_byte;
unsigned char serial_buffer_index;
unsigned char valide_start=0;

void loop() {
  if (Serial.available() > 0) {
    read_byte = Serial.read();
    if(read_byte==255){
      if(valide_start == 0){
        //start packet read
        serial_buffer_index=0;
        valide_start=1;
      }else{
        //error stop packet read
        valide_start=0;
      }
    }
    else if(valide_start==1){
      if(read_byte==254){ //check if stop byte
        if(serial_buffer_index==3){ //check if all packed as been read and applyes the changes
          serial_speed= (serial_buffer[0]<<5) + serial_buffer[1]; 
          setSPEED_BCD(serial_speed);

          serial_rpm = serial_buffer[2];
          myservo.write(((unsigned int)serial_rpm * 150) / max_rpm);
        }
        valide_start=0;
      }else{
        serial_buffer[serial_buffer_index]=read_byte;
        serial_buffer_index++;
      }
    }
  }
}