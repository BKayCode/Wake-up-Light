#ifndef LampControl_h
#define LampControl_h
#include <Arduino.h>
#include <Wire.h>

void startRise(byte Risetime, byte RiseBrightness);
void sendBrightness(byte sendBrightness);

#endif
