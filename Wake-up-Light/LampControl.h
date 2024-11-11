#ifndef lampControl_h
#define lampControl_h
#include <Arduino.h>
#include <Wire.h>

void startRise(byte risetime, byte riseBrightness);
void sendBrightness(byte sendBrightness);

#endif
