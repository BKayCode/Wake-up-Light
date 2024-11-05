# Wake-up-Light
This is a hack of an Philips Wake-up Light.<br>
I have a old model (HF3520) with no ability to completely turn off the display and would like to have it connected to NTP.

For now I just reverse engineered the Lamp part and wrote a simple script to get the light working.

Please note the I2C Bus is 5V, so when connecting it to a ESP8266/ESP32 use a levelshifter.

## Light module
The light part of the Wake-up Light is a separate module connected to 24V and 5V, controlled by I2C.

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
| change brigtness  | `0x00 0x02 REPSTART brightnessLevel[0x00:0x20]`   |  `0x00 0x04 REPSTART 0xAA` |
| rise simulation  | `0x00 0x02 REPSTART riseBrightness[0x00:0x20] riseMinutes[0x00:0x2D]`   |   |
| read brightness  | `0x00 0x05` | request 1 byte |

There is a quirk where if the lamp is off, you have to send a empty packet to let it receive packets. Maybe some sort of sleep...<br>
Also it really wants repeated start conditions after 2 bytes.<br>
Original I2C clock when connected to the original board is about 16kHz, but it works fine with standard 100kHz.

change brigtness simulates the original hardware buttons for manualy setting the brightness, rise simulation is the fading before the alarm sounds.

## Side buttons

Side buttons are split up to two Connectors. The buttons connect a resistor between the leads when pushed.<br>
They could be made usable by connecting the cables to analog inputs.<br>
Since the ESP boards only have one single ADC pin, I ordered a CD74HC4067 Multiplexer breakout. Testing contiues...

## Display

Projector Style backlit LCD Display, which throws the digits against the matt front glass.

- Backlight LED are the grey (-) and white (+) wire, driven with PWM modulated 3V.

- Custom LCD, 4 Rows, 17 Segments. 60Hz modulated Squarewave.<br>
When looking "at the projector" bottom, Pin 1 is to the right.<br><br>
Pin 1-4 = Rows, 5-17 = Segments.
- Alternatively for the display I tried a 7-segment module, but when held behind the front it got to much defused...

## Things to do

The capacitive buttons on the front are using a dedicaded processor on its board.<br>
Latter it would be feasable to get working maybe, although the connected ribbon cable ends with a inductor for every wire which might become a nightmare.<br>
Maybe it have to be decoupled for the capacitive buttons to work...<br>
Had a short go with it nevertheless: There ist one squarewave 50% duty cycle and two signals which look like data, although Iam missing a clocksignal...
