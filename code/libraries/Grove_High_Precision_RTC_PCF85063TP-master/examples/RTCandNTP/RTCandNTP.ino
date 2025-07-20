/****************************************************************************/
//  Function: Set time and get the time from RTC chip(PCD85063TP) and display
//            it on the serial monitor.
//  Hardware: Grove - RTC v2.0
//  Arduino IDE: Arduino-1.6.6
//  Author:   lambor
//  Date:    June 14,2016
//  Version: v1.0
//  by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later verscion.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
/****************************************************************************/
#include <Wire.h>
#include "PCF85063TP.h"
#include <WiFi.h>
#include "time.h"

PCD85063TP RTclock;//define a object of PCD85063TP class

const char* ssid       = "SSID";
const char* password   = "password";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000;
const int   daylightOffset_sec = 3600;

void printLocalTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void syncLocalTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    RTclock.stopClock();
    RTclock.fillByYMD(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday); //Jan 19,2013
    RTclock.fillByHMS(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec); //15:28 30"
    RTclock.fillDayOfWeek(timeinfo.tm_wday);//Saturday
    RTclock.setTime();//write time to the RTC chip
    RTclock.startClock();
}


void setup() {
    Serial.begin(115200);
    RTclock.begin();

    RTclock.stopClock();  //setting clock to a fixed default time to
    RTclock.fillByYMD(2018, 1, 1); //Jan 19,2013
    RTclock.fillByHMS(21, 36, 00); //15:28 30"
    RTclock.fillDayOfWeek(WED);//Saturday
    RTclock.setTime();//write time to the RTC chip
    RTclock.startClock();

    //connect to WiFi
    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" CONNECTED");

    //init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); //get time from server
    printLocalTime(); //print NTP time

    //disconnect WiFi as it's no longer needed
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    //clock.setcalibration(1, 32767.2);  // Setting offset by clock frequency
    uint8_t ret = RTclock.calibratBySeconds(0, -0.000041);
    Serial.print("offset value: ");
    Serial.print("0x");
    Serial.println(ret, HEX);
    printTime(); //print internal RTC time before running sync
    syncLocalTime(); //sync RTC with NTP time
}
void loop() {
    printTime();
    delay(1000);
}
/*Function: Display time from RTC on the serial monitor*/
void printTime() {
    RTclock.getTime();
    Serial.print(RTclock.hour, DEC);
    Serial.print(":");
    Serial.print(RTclock.minute, DEC);
    Serial.print(":");
    Serial.print(RTclock.second, DEC);
    Serial.print("  ");
    Serial.print(RTclock.month, DEC);
    Serial.print("/");
    Serial.print(RTclock.dayOfMonth, DEC);
    Serial.print("/");
    Serial.print(RTclock.year + 2000, DEC);
    Serial.print(" ");
    Serial.print(RTclock.dayOfMonth);
    Serial.print("*");
    switch (RTclock.dayOfWeek) { // Friendly printout the weekday
        case MON:
            Serial.print("MON");
            break;
        case TUE:
            Serial.print("TUE");
            break;
        case WED:
            Serial.print("WED");
            break;
        case THU:
            Serial.print("THU");
            break;
        case FRI:
            Serial.print("FRI");
            break;
        case SAT:
            Serial.print("SAT");
            break;
        case SUN:
            Serial.print("SUN");
            break;
    }
    Serial.println(" ");
}
