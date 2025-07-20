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

PCD85063TP RTclock;//define a object of PCD85063TP class
void setup() {
    Serial.begin(9600);
    RTclock.begin();
    /*
        RTclock.stopClock();
        RTclock.fillByYMD(2016,7,5);//Jan 19,2013
        RTclock.fillByHMS(14,10,00);//15:28 30"
        RTclock.fillDayOfWeek(TUE);//Saturday
        RTclock.setTime();//write time to the RTC chip
        RTclock.startClock();

    */
    //clock.setcalibration(1, 32767.2);  // Setting offset by clock frequency
    uint8_t ret = clock.calibratBySeconds(0, -0.000041);
    Serial.print("offset value: ");
    Serial.print("0x");
    Serial.println(ret, HEX);

}
void loop() {
    printTime();
    delay(1000);
}
/*Function: Display time on the serial monitor*/
void printTime() {
    clock.getTime();
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
