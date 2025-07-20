/*
    A library for Grove -

    Copyright (c) 2018 seeed technology co., ltd.
    Author      : Wayen Weng
    Create Time : November 2018
    Change Log  :

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef __SENSIRION_SCD30_H__
#define __SENSIRION_SCD30_H__

#include <Arduino.h>
//#include <Wire.h>
#include <WSWire.h>

#define SCD30_I2C_ADDRESS                       0x61

#define SCD30_CONTINUOUS_MEASUREMENT            0x0010
#define SCD30_SET_MEASUREMENT_INTERVAL          0x4600
#define SCD30_GET_DATA_READY                    0x0202
#define SCD30_READ_MEASUREMENT                  0x0300
#define SCD30_STOP_MEASUREMENT                  0x0104
#define SCD30_AUTOMATIC_SELF_CALIBRATION        0x5306
#define SCD30_SET_FORCED_RECALIBRATION_FACTOR   0x5204
#define SCD30_SET_TEMPERATURE_OFFSET            0x5403
#define SCD30_SET_ALTITUDE_COMPENSATION         0x5102
#define SCD30_READ_SERIALNBR                    0xD033

#define SCD30_SET_TEMP_OFFSET                   0x5403


#define SCD30_POLYNOMIAL                        0x31 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001

class SCD30 {
  public:

    SCD30(void);

    void initialize(void);

    bool isAvailable(void);

    void setAutoSelfCalibration(bool enable);
    void setMeasurementInterval(uint16_t interval);

    void startPeriodicMeasurment(void);
    void stopMeasurement(void);
    void setTemperatureOffset(uint16_t offset);

    void getCarbonDioxideConcentration(float* result);
  private:

    uint8_t calculateCrc(uint8_t* data, uint8_t len);

    void writeCommand(uint16_t command);
    void writeCommandWithArguments(uint16_t command, uint16_t arguments);
    uint16_t readRegister(uint16_t address);

    void writeBuffer(uint8_t* data, uint8_t len);
    void readBuffer(uint8_t* data, uint8_t len);

    uint8_t devAddr;

};

extern SCD30 scd30;

#endif
