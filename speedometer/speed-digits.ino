#define RegisterCLK 8
#define Digit0 4
#define Digit1 3
#define Digit2 2
#define ledPin 13
#include <SPI.h>

unsigned int NUMBER[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 3
  0b01100110, // 4
  0b01111111, // 5
  0b01111111,
  0b01111111,
  0b00111000,
  0b01111111,
  0b01101111,
};

unsigned int digit = 0;

unsigned int nombres[3] = {2,3,4};

void setupTimer2(){
  
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
  // put your setup code here, to run once:
  pinMode(RegisterCLK, OUTPUT);
  pinMode(Digit0, OUTPUT);
  pinMode(Digit1, OUTPUT);
  pinMode(Digit2, OUTPUT);

  digitalWrite(RegisterCLK, 0);

  SPI.begin();
  SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));
  
  SPI.transfer(NUMBER[nombres[digit]]); 

  digitalWrite(RegisterCLK, 1);
  digitalWrite(RegisterCLK, 0);

  digitalWrite(Digit0, 1);
  digitalWrite(Digit1, 1);
  digitalWrite(Digit2, 1);

 setupTimer2();
}

ISR(TIMER2_COMPA_vect)          // timer compare interrupt service routine
{
  digit = (digit+1)%3;
  switch (digit){
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
  SPI.transfer(NUMBER[nombres[digit]]); 
  digitalWrite(RegisterCLK, 1);
  digitalWrite(RegisterCLK, 0);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
