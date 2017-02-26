#include <SPI.h> 
#include <Servo.h>
#include "nRF24L01.h"
#include "RF24.h"
Servo myservo;
int pos = 30;

int msg[1];
RF24 radio(9, 10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

int LED1 = 3;

void setup(void) {
  Serial.begin(9600);
  myservo.attach(7);
  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  pinMode(LED1, OUTPUT);
} 

void loop(void) {
  
  if (radio.available()) {
    bool done = false;
    while (!done) {
      done = radio.read(msg, 1);
      Serial.println(msg[0]);
      //Message Received from Bluetooth
      if (msg[0] == 111) {
        //Turn on Servo & LED
        digitalWrite(LED1, LOW);
        turnServoTriggerOn();
        delay(1000);
      }
      else {
        turnServoTriggerOff();
        digitalWrite(LED1, HIGH);
        delay(10);
      }
      delay(10);
    }
  }
  else {
    Serial.println("No radio available");
    digitalWrite(LED1,HIGH);
    turnServoTriggerOff();
    delay(1000);
  }
} 


void turnServoTriggerOn()
{
  for(pos; pos<=140; pos+=1)
  {
    myservo.write(pos);
    delay(5);
  }
}

void turnServoTriggerOff()
{
  
  for(pos; pos>=30; pos-=1)
  {
    myservo.write(pos);
    delay(5);
  }
}

