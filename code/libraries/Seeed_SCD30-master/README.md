# Seeed SCD30 Library  [![Build Status](https://travis-ci.com/Seeed-Studio/Seeed_SCD30.svg?branch=master)](https://travis-ci.com/Seeed-Studio/Seeed_SCD30)

This is the Arduino library for Seeed's SCD30 based products.

## Usage
    1.Git clone this resp to your Arduino IDE's libraries directory.
	2.Run the demo "SCD30_Example" on examples directory.

***

## Notice
* The on-board RH/T sensor is influenced by thermal self-heating of SCD30 and other electrical components.   
    Design-in alters the thermal properties of SCD30 such that temperature and humidity offsets may occur when operating the sensor in end-customer devices.  
    Compensation of those effects is achievable by writing the temperature offset found in continuous operation of the device into the sensor.
    User can use "setTemperatureOffset(offset)" to set temperature offset,The param "offset" is type of uint16_t,one tick corresponding to 0.01℃.  
    When user set the temperature offset,the measurement result of temperature won't change immediately,but a slow change process.  
    Take notice that the humidity value will change with the temperature value changes.
* The sensor needs to be calibrated at the first time it is used.  

    When activated for the first time a period of minimum 7 days is needed so that the algorithm can find its initial parameter set for ASC. The sensor has to be exposed to fresh air for at least 1 hour every day.  

    **the sensor may not be disconnected from the power supply,otherwise the procedure to find calibration parameters is aborted and has to be restarted from the beginning**

    User can run the SCD30_auto_calibration.ino to start the calibration.
***




This software is written by Wayen Weng for seeed studio and is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check License.txt for more information.<br>

Contributing to this software is warmly welcomed. You can do this basically by<br>
[forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/articles/using-pull-requests) (follow the links above<br>
for operating guide). Adding change log and your contact into file header is encouraged.<br>
Thanks for your contribution.

Seeed Studio is an open hardware facilitation company based in Shenzhen, China. <br>
Benefiting from local manufacture power and convenient global logistic system, <br>
we integrate resources to serve new era of innovation. Seeed also works with <br>
global distributors and partners to push open hardware movement.<br>


[![Analytics](https://ga-beacon.appspot.com/UA-46589105-3/grove-human-presence-sensor)](https://github.com/igrigorik/ga-beacon)

