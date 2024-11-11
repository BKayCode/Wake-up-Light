# Wake-up-Light
This is a hack of an Philips Wake-up Light.<br>
I have a old model (HF3520) with no ability to completely turn off the display and would like to automaticly set the Time via NTP.

For now I just reverse engineered the Lamp part and side buttons, wrote a simple script to get the light working.
Iam using a Wemos D1 mini to controll everything.

## Light module
The light part of the Wake-up Light is a separate module connected to 24V and 5V, controlled by I2C.
Please note the I2C Bus is 5V, so when connecting it to a ESP8266/ESP32 use a levelshifter.

Pinout of the connector from light module:

```
+-------+   +------+
|       +---+      |
|   1     2    3   |
|                  |
+------------------+
```

1 = SDA (yellow)<br>
2 = SCR (red)<br>
3 = GND (brown)<br>

I2C part:

Address of the module is 0x51

| command  | I2C packet 1 | I2C packet 2 | 
| ------------- | ------------- | ------------- |
| change brigtness  | `Write0x51 0x00 0x02 REPSTART brightnessLevel[0x00:0x14]`   |  `Write0x51 0x00 0x04 REPSTART 0xAA` |
| rise simulation  | `Write0x51 0x00 0x02 REPSTART riseBrightness[0x01:0x14] riseMinutes[0x01:0x2D]`   |   |
| read brightness  | `Write0x51 0x00 0x05` | `Read0x51` |

There is a quirk where if the lamp is off, you have to send a empty packet to let it receive packets. Maybe some sort of sleep...<br>
Also it really wants repeated start conditions after 2 bytes.<br>
Original I2C clock when connected to the original board is about 16kHz, but it works fine with standard 100kHz.

change brigtness simulates the original hardware buttons on the side for manualy setting the brightness, rise simulation is the fading before the alarm sounds.

## Display

Projector Style backlit LCD Display, which throws the digits against the matt front glass.

- Backlight LED are the grey (-) and white (+) wire, driven with PWM modulated 3V.

- Custom LCD, 4 Rows, 17 Segments. 60Hz modulated Squarewave.<br>
When looking "at the projector" bottom, Pin 1 is to the right.<br><br>
Pin 1-4 = Rows, 5-20 = Segments. Pin pitch 2,00mm, 20 Pins.

- [PCF8562](https://www.nxp.com/docs/en/data-sheet/PCF8562.pdf) or [PCA8561](https://www.nxp.com/docs/en/data-sheet/PCA8561.pdf) are cheap and looking promising.<br>
  may need a TSOP48 breakout board, or something custom. 

## Side buttons

- Side buttons are split up to two Connectors: Left and right buttons. They form a resistor ladder.

Since the ESP boards only have one single ADC pin, I connected them to a CD74HC4067 Multiplexer breakout.
![alt text](https://github.com/BKayCode/Wake-up-Light/blob/main/s-l1600-1.jpg?raw=true)<br>
Connect one pin of the left connector to Y0, right to Y1, connect GND,EN,S1-3 to ESP GND, S0 to D4, VCC to 3,3V, Z to analog pin of the ESP.
The same Pins on Y0 and Y1 need to be connected to 3,3V with a 10k resistor to bias them. The other pin connect to ground.


## Front buttons

- Capacitive touch buttons, conveniently ready to use via I2C Bus. <br>
- Polling frequency 60Hz, I2C speed on original board is up to 250kHz, but takes heavily advantace of clock stretching. Testing if working with standard 100kHz.<br>
- The pleasent clicky sound on touch is generated with a piezzo: 3,3V, 5kHz for 3ms duration.<br>

| command  | I2C packet
| ------------- | ------------- |
| validatePush  | `Write0x75 0x3D REPSTART Read0x75 validTouch[0x00:0x01]` | 
| pushedButton  | `Write0x75 0x31 REPSTART Read0x75 pushedButton[0x00:0x08]`|

The button push is only accepted if validatePush == 1.

pushedButton values:
|   |   |   |  |
| --- | --- | --- | --- |
| Left | Menu	 | Select	 | Right |
| 0x04	| 	0x08	| 	0x01	| 	0x02 |

On the original board on the ribbon is a 1Hz square carried, the I2C polling takes only place on high state...


## Things to do

Get the alarm sounds working, but for the moment ist my bottom priority - still trusting my smartphone more for now. :D<br>
The Amplifier, FM module and I think sound module is located on the power board and seems to be controlled from the main MCU board via the ribbon cable.<br>
On that same board are pads labeled I2C, but they carry no signals. Also the traces go directly to the ribbon connector...
