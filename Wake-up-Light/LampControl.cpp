#include "LampControl.h"
byte error;
byte currentState = 0;

void startRise(byte risetime, byte riseBrightness){

Wire.beginTransmission(0x51);             // Wakeup Packet from dark
Wire.endTransmission();

// set error to arbitrary non zero value to repeat sending until receiver acknowledged
error = 7;

while (error != 0) {

    Wire.beginTransmission(0x51);
    Wire.write(0x00);
    Wire.write(0x02);
    Wire.endTransmission(false);
    Wire.beginTransmission(0x51);
    Wire.write(riseBrightness);
    Wire.write(risetime);
    error = Wire.endTransmission();

  Serial.print("Sending risetime ");
  Serial.print(risetime);
  Serial.print(", riseBrightness");
  Serial.print(riseBrightness);
  Serial.print(", error: ");
  Serial.println(error);

 if (error == 0){
    break;
    currentState = 1;
  }
  else
  {
    delay(1000);
    Wire.beginTransmission(0x51);             // Wakeup Packet from dark
    Wire.endTransmission();
  }
}

}

void sendBrightness(byte sendBrightness){

  // Check if Lamp is off, if so send wakeup Packet
  if(currentState == 0){
  Wire.beginTransmission(0x51);
  Wire.endTransmission();
  }

  error = 7;

while (error != 0) {
    Wire.beginTransmission(0x51);
    Wire.write(0x00);
    Wire.write(0x02);
    Wire.endTransmission(false);
    Wire.beginTransmission(0x51);
    Wire.write(sendBrightness);
    error = Wire.endTransmission();
}

error = 7;

  while (error != 0){
    Wire.beginTransmission(0x51);
    Wire.write(0x00);
    Wire.write(0x04);
    Wire.endTransmission(false);
    Wire.beginTransmission(0x51);
    Wire.write(0xAA);
    error = Wire.endTransmission();
}

  currentState = sendBrightness;

}