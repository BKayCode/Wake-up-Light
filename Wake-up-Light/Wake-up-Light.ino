/* 

credit to noiasca for providing the basic structure of the NTP implementation
2021-03-28 https://werner.rothschopf.net/microcontroller/202103_arduino_esp32_ntp_en.htm

*/

#ifndef STASSID
#define STASSID "ssid"          // set your SSID
#define STAPSK "wifiPassword"       // set your wifi password
#endif

/* Configuration of NTP */
// choose the best fitting NTP server pool for your country
#define MY_NTP_SERVER "europe.pool.ntp.org"

// choose your time zone from this list
// https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
#define MY_TZ "CET-1CEST,M3.5.0,M10.5.0/3"

/* Necessary Includes */
#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>                     // we need wifi to get internet access
#endif
#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h> 
#endif
#include <time.h>                    // for time() ctime()

#include <sntp.h>  // sntp_servermode_dhcp()

#include "LampControl.h"

/* Globals */
time_t now;                          // this are the seconds since Epoch (1970) - UTC
tm tm;                             // the structure tm holds time information in a more convenient way *

void showTime() {
  time(&now); // read the current time
  localtime_r(&now, &tm);             // update the structure tm with the current time
  Serial.print("year:");
  Serial.print(tm.tm_year + 1900);    // years since 1900
  Serial.print("\tmonth:");
  Serial.print(tm.tm_mon + 1);        // January = 0 (!)
  Serial.print("\tday:");
  Serial.print(tm.tm_mday);           // day of month
  Serial.print("\thour:");
  Serial.print(tm.tm_hour);           // hours since midnight 0-23
  Serial.print("\tmin:");
  Serial.print(tm.tm_min);            // minutes after the hour 0-59
  Serial.print("\tsec:");
  Serial.print(tm.tm_sec);            // seconds after the minute 0-61*
  Serial.print("\twday");
  Serial.print(tm.tm_wday);           // days since Sunday 0-6
  if (tm.tm_isdst == 1)               // Daylight Saving Time flag
    Serial.print("\tDST");  
  else
    Serial.print("\tstandard");
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nWake-up-Light");

  // sntp_servermode_dhcp(0);      // Do not use Routers NTP

  #ifdef ARDUINO_ARCH_ESP32
  // ESP32 seems to be a little more complex:
  configTime(0, 0, MY_NTP_SERVER);  // 0, 0 because we will use TZ in the next line
  setenv("TZ", MY_TZ, 1);            // Set environment variable with your time zone
  tzset();
  #else
  // ESP8266
  configTime(MY_TZ, MY_NTP_SERVER);    // --> for the ESP8266 only
  #endif

  // start network
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print ( "." );
  }
  Serial.println("\nWiFi connected");

}

// set the desired Time when you want to be woken by the light
byte wakeupHour = 6;
byte wakeupMinute = 40;

// set desired brightnesslevel of the rise simulation [1:20]
byte riseBrightness = 12;
// set the minutes for how long to fade to desired riseBrightness [1:40]
byte risetime = 20;

// set values equal, so the timedifference does not calculate over and over again
byte riseHour = wakeupHour;
byte riseMinute = wakeupMinute;



void loop() {
  
  unsigned long time_now = millis();
  while(millis() < time_now + 1000){
        //wait approx. [period] ms
    }
  showTime();

  // calculate time difference to start rise simulation timely
  if ((riseHour == wakeupHour) && (riseMinute == wakeupMinute)){
    riseHour = (wakeupHour * 60 + wakeupMinute - risetime) / 60;
    riseMinute = (wakeupHour * 60 + wakeupMinute - risetime) % 60;

    Serial.print("riseHour = ");
    Serial.print(riseHour);
    Serial.print(", riseMinute = ");
    Serial.print(riseMinute);
  }

  // send command to slowly brighten the lamp (hardwired to only execute on Mondays to Fridays)
  if ((tm.tm_wday > 0) && (tm.tm_wday < 6) && (tm.tm_hour == riseHour) && (tm.tm_min == riseMinute) && (tm.tm_sec == 0)){
    startRise(risetime, riseBrightness);
  }

  // turn off the light at 6:45 on Mondays to Fridays, since there are no buttons implemented / connected yet...
  if ((tm.tm_wday > 0) && (tm.tm_wday < 6) && (tm.tm_hour == 6) && (tm.tm_min == 45) && (tm.tm_sec == 0)){
    sendBrightness(0);
  }

}
