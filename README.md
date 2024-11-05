# Wake-up-Light
This is a hack of an Philips Wake-up Light.<br>
I have a old model (HF3520) with no ability to completely turn off the display and would like to have it connected to NTP.

For now I just reverse engineered the Lamp part and wrote a simple script to get the light working.

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