#include "sideButtons.h"
#include "lampControl.h"

byte readPin;
byte pushedL;
byte pushedR;
byte debouncedButton;
bool released = true;
byte currentState2;

void doButtons(){

  readButtons();

  switch (debouncedButton) {

    case 0:
      break;
    
    case 1:       // Button IncreaseBrightness
      if (currentState2 < 20){
        currentState2++;
        sendBrightness(currentState2);
      }
      break;

    case 2:       // Button BrightnessOnOff
      if (currentState2 == 0){
        sendBrightness(1);
        currentState2 = 1;
      }
      else {
       sendBrightness(0);
       currentState2 = 0;
      }
      break;

    case 3:
      if (currentState2 > 1){
        currentState2--;
        sendBrightness(currentState2);
      }
      break;

    case 4:
      break;

    case 5:
      break;

    case 6:
      break;

    case 7:
      break;

    case 8:
      break;

    case 9:
      break;



  }

}

void readButtons(){

  for (readPin = 0; readPin < 2; readPin++){

   digitalWrite(D4, readPin);

   int sensorValue = analogRead(A0);

    switch (readPin) {

      // left buttons
      case 0:

        if (sensorValue >  797){                                  // No button pressed
          pushedL = 0;
          break;
        }
        else if((sensorValue < 797) && (sensorValue > 481)){     // Button IncreaseBrightness
          pushedL = 1;
          break;
        }
        else if((sensorValue < 481) && (sensorValue > 281)){     // Button BrightnessOnOff
          pushedL = 2;
          break;
        }
        else if((sensorValue < 281) && (sensorValue > 115)){     // Button LowerBrightness
          pushedL = 3;
          break;
        }
        else if((sensorValue < 115)){                             // Button Alarm 1
          pushedL = 4;
          break;
        }
    
    // right buttons
    case 1:

      if (sensorValue >  896){                                  // No button pressed
        pushedR = 0;
        break;
      }
      else if((sensorValue < 896) && (sensorValue > 676)){     // Button decreaseVolume
        pushedR = 1;
        break;
      }
      else if((sensorValue < 676) && (sensorValue > 481)){     // Button FM
        pushedR = 2;
        break;
      }
      else if((sensorValue < 481) && (sensorValue > 281)){     // Button increaseVolume
        pushedR = 3;
        break;
      }
      else if((sensorValue < 281) && (sensorValue > 115)){     // Button Alarm 2
        pushedR = 4;
        break;
      }
      else if((sensorValue < 115)){                            // Button Snooze
        pushedR = 5;
        break;
      }

    }

  }

    if( (pushedL == 0) && (pushedR == 0) ){            // make it to execute stuff only once, while buttons are pushed...
      released = true;
      debouncedButton = 0;
    }
    else if( (pushedL > 0) && (pushedR == 0) && (released == true) ){
        debouncedButton = pushedL;
        released = false;
    }
    else if( (pushedR > 0) && (pushedL == 0) && (released == true) ){
        debouncedButton = pushedR;
        released = false;
    }
    else if( ( (pushedL > 0) || (pushedR > 0) ) && (released == false) ){
      debouncedButton = 0;
    }

}
