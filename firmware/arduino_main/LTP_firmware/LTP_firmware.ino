#include <PacketSerial.h>
#include "lidar.h"
#include "encoder.h"

LIDAR lidar;
Encoder encoder;

int testPin1 = 6;
int testPin2 = 7;
int powerEnablePin = 2;

void setup() {
  lidar.init();
  encoder.init();

  pinMode(testPin1, OUTPUT);
  pinMode(testPin2, OUTPUT);
  pinMode(powerEnablePin, OUTPUT);
  digitalWrite(testPin1, LOW);
  digitalWrite(testPin2, LOW);
  digitalWrite(powerEnablePin, HIGH);
  
  Serial.begin(115200);
  Serial.println("Booted up!");  
}

void loop() {

  digitalWrite(testPin2, HIGH); 
  int packet1 = lidar.getDistance();
  digitalWrite(testPin2, LOW); 

  Serial.print(packet1);
  Serial.print(",");
  Serial.println(encoder.getAngle());

  
  //encoder.getAngle();
  
}

