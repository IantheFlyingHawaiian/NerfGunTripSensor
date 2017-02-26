#include  <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define trigPin 3
#define echoPin 2
int total = 0;
int avg = 0;
long duration, distance;

int msg[1];
RF24 radio(9, 10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int SW1 = 7;

void setAvgDistance()
{
    //Send out Triggers
    for(int i = 0;i<100;i++)
    {

    digitalWrite(trigPin, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin, HIGH);
  
    //Receive in Echo
    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 29.1;
    total += distance;
    }

    //calculate average distance
    avg = abs(total/100);
    Serial.println(" ");
    Serial.print("Initial Avg: ");
    Serial.println(avg);
    total = 0;
  
}


void setup(void) {
  Serial.begin(9600);
  Serial.print("Start Radio");
  radio.begin();
  radio.openWritingPipe(pipe);

  //Set up Distance Sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  setAvgDistance();
  
} 

void loop(void) {

//With PNG not Working
  //Send out Triggers
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);

  //Receive in Echo
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  
  if (digitalRead(SW1) == HIGH) {
      if (distance >= avg + 40 || distance <= avg - 40) {
        Serial.println("Sensor TRIGGERED");
          msg[0] = 111;
          radio.write(msg, 1);
      }
  }

  //Rest average distance of sensor
  if(digitalRead(SW1) == LOW)
  {
    setAvgDistance();
  }

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("cm");
  Serial.print("Distance AVG: ");
  Serial.print(avg);
  Serial.println(" cm");
}

