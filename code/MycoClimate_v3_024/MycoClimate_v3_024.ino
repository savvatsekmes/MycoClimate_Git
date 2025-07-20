
//---------------------------------------------------
// 1. Libaries
//---------------------------------------------------
#include <WSWire.h> // this is an alternate wire.h library that doesnt freeze the system. 

#include <avr/wdt.h>

#include <EEPROM.h>
 
#include <I2C_LCD.h>
I2C_LCD LCD;
//Declare bitmap data package.
extern GUI_Bitmap_t bmmycoclimate_v3;
extern GUI_Bitmap_t bmcubensis;      
extern GUI_Bitmap_t bmlionsmane;      
extern GUI_Bitmap_t bmcordyceps;      
extern GUI_Bitmap_t bmrieshi;      
extern GUI_Bitmap_t bmenoki;       
extern GUI_Bitmap_t bmmorel;     
extern GUI_Bitmap_t bmoyster;       
extern GUI_Bitmap_t bmshiitake;     
extern GUI_Bitmap_t bmturkeytail;     
extern GUI_Bitmap_t bmmaitake;  

int mode; //

int passFlag = 0; // to not loop the bitmap refresh
int splash = 0; // to not loop the bitmap refresh for splash

uint8_t I2C_LCD_ADDRESS = 0x52; //Device address configuration, the default value is 0x51.

#include "SCD30.h"
int resultC02;
int resultHumi;
int resultTemp;

#include "PCF85063TP.h"
PCD85063TP RTclock;//define a object of PCD85063TP class

//----------------------------------------------------
// 2. Pins
//----------------------------------------------------



///////Draw Closed Sensor//////////
const int magnetSensor = 8;
int mState; // 0 close - 1 open switch


///////Humidity///////

//Humidity Control points
int HSETPOINT;
int HDEADBAND = 1;


//Defining the HUMIDIFIER output pin and on/off states.
#define HUMIDIFIER 7
#define HON false // the realy is sort of backwards. A low/0/false out is actually ON
#define HOFF true // the realy is sort of backwards. A High/1/true out is actually OFF


//Defining the HUMIDIFIER FAN output pin and on/off states.
#define HFAN 6
#define HFON false // the realy is sort of backwards. A low/0/false out is actually ON
#define HFOFF true // the realy is sort of backwards. A High/1/true out is actually OFF



///////CO2///////

//CO2 Control points
int CSETPOINT;
int CDEADBAND = 30;


//Defining the C02 FAN output pin and on/off states.
#define CFAN 5
#define CFON false // the realy is sort of backwards. A low/0/false out is actually ON
#define CFOFF true // the realy is sort of backwards. A High/1/true out is actually OFF



///////Tempreture///////

//Tempreture Control points for Peltier Cooler
int TSETPOINT;
int TDEADBAND = 3;


//Defining the Peltier Cooler output pin and on/off states.
#define PCOOLER 4
#define PCON false // the realy is sort of backwards. A low/0/false out is actually ON
#define PCOFF true // the realy is sort of backwards. A High/1/true out is actually OFF


//Tempreture Control points for Heater pads
int THSETPOINT;
int THDEADBAND = 3;

//Defining the Heater pads output pin and on/off states.
#define HPADS 9
#define HPON false // the realy is sort of backwards. A low/0/false out is actually ON
#define HPOFF true // the realy is sort of backwards. A High/1/true out is actually OFF



/////WATER TANK SENSOR///////

//#define WATER_SENSOR 2
int watersensorCount = 0; // to not loop the bitmap refresh for splash
unsigned long lastWaterSensorReset;


// SEN0369 Water Sensor pins
int inPin = 2;
boolean running = 0;//when running=1, the liquid is detected, print out 1, otherwise, print out 0; running=0, the liquid is detected, print out 0, otherwise, print out 1.
int modePin = 12;




//Defining Solonoid water valve output pin and on/off states.
#define SOLONOID 3
//#define SON false // the realy is sort of backwards. A low/0/false out is actually ON
//#define SOFF true // the realy is sort of backwards. A High/1/true out is actually OFF
int SOLONOIDStatus = 0;

////NEO PIXELS////  NEW LEDS
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN       10
#define NUMPIXELS 80 
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 33 // Time (in milliseconds) to pause between pixels

int DNC; // this is used in the mode settings. allocating if on or off.
int DNCstatus; //this is used for the LCD, to print DN on or DN off. 



////UVC LED////

//Defining the UVC Sterilize LED output pin and on/off states.
#define UVCLED 13
#define UVCON false // the realy is sort of backwards. A low/0/false out is actually ON
#define UVCOFF true // the realy is sort of backwards. A High/1/true out is actually OFF
 
int UVCstatus; //Overall status of the light if it meets both requirements 
int UVCtimer; //this is used for the UVC timer


/////////////////////////MENU/JOYSTICK//////////////////////////////////////////////////////////


const int yPin = A1;
const int xPin = A0;
const int kPin = A0;
//SDA            A4
//SCL            A5


//----------------------------------------------------
// 3. Variables 
//----------------------------------------------------
int tCount1;
bool refresh;//lcd clear On/Off

//leerJoystick
int joyRead;
int joyPos; 
int lastJoyPos;
long lastDebounceTime = 0; 
long debounceDelay = 70;                 //user define

//Control Joystick
bool PQCP;
int mNivel1;  
int mNivel2;
 




//====================================================
// SETUP
//====================================================
void setup() {

   wdt_enable(WDTO_8S);    //watchdog timer with 8 Seconds time out
    
   Serial.begin (9600);

    
    Wire.begin();         //I2C controller initialization.
   
    LCD.WorkingModeConf(OFF, ON, WM_CharMode);
    LCD.CleanAll(BLACK);    //Clean the screen with black or white.
    LCD.FontModeConf(Font_6x8, FM_MNL_AAA, WHITE_BAC);
    

    scd30.initialize();   //Humidity/Temp/C02 sensor initialization.
   // scd30.setMeasurementInterval(2);


///////////EEPROM/MODE////////////////////////////////////////////
//Read Eeprom address 0 (mode)
  mode = EEPROM.read(0);

///////Draw Closed Sensor//////////
pinMode(magnetSensor, INPUT_PULLUP);

///////////////RTC////////////////////////////////////////////////

    
    RTclock.begin();

/*
    // Set the time and date. Use once and comment out.
        RTclock.stopClock();
        RTclock.fillByYMD(2023,9,23);//
        RTclock.fillByHMS(9,51,00);//
        RTclock.fillDayOfWeek(SAT);//
        RTclock.setTime();//write time to the RTC chip
        RTclock.startClock();
 */ 


    RTclock.setcalibration(1, 32767.2);  // Setting offset by clock frequency
    uint8_t ret = RTclock.calibratBySeconds(0, -0.000041);
    /*Serial.print("offset value: ");
    Serial.print("0x");
    Serial.println(ret, HEX);*/
    

///////////////JOYTISCK PINS////////////////////////////////////////////////

  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(kPin, INPUT);

////////////WATER SENSOR//////////////////////////////////////////////////

 // pinMode(WATER_SENSOR, INPUT); //initialize digital pin as an input.

// SEN0369 Water Sensor setup
  pinMode(inPin, INPUT);
  pinMode(modePin, OUTPUT);
  digitalWrite(modePin, running);



///// INITIALIZE NEOPIXELS///////
 pixels.begin();
SetPixelsNone(); // NEO PIXELS OFF



  ////////////RELAY PINS//////////////////////////////////////////////////
 
  //Assign HEATER PADS instead of pin 9
    pinMode(HPADS, OUTPUT);
    digitalWrite(HPADS, HPOFF);

    //Assign HUMIDIFIER instead of pin 7
    pinMode(HUMIDIFIER, OUTPUT);
    digitalWrite(HUMIDIFIER, HOFF);

   //Assign HFAN instead of pin 6
   pinMode(HFAN, OUTPUT);
   digitalWrite(HFAN, HFOFF);

   //Assign CFAN instead of pin 5
   pinMode(CFAN, OUTPUT);
   digitalWrite(CFAN, CFOFF);

   //Assign PCOOLER instead of pin 4
  pinMode(PCOOLER, OUTPUT);
  digitalWrite(PCOOLER, PCOFF);

    //Assign SOLONOID instead of pin 3 - initialize digital pin as an output.
  pinMode(SOLONOID, OUTPUT);
  digitalWrite(SOLONOID, HIGH);

   //Assign DNCLED instead of pin 2//////old LED code
  //pinMode(DNCLED, OUTPUT);
 // digitalWrite(DNCLED, DNCOFF);

  //Assign UVCLED instead of pin 13
  pinMode(UVCLED, OUTPUT);
  digitalWrite(UVCLED, UVCOFF);


}

//====================================================
// LOOP
//====================================================
void loop() {
  modeSettings();
  waterLevelCheck();
  controlJoystick();
  menu();
  wdt_reset();
}




//====================================================
// WATER LEVEL CHECK
//====================================================

void waterLevelCheck() {

//Serial.println(digitalRead(WATER_SENSOR));
Serial.println(digitalRead(inPin));
Serial.println(watersensorCount);

if(digitalRead(inPin) == 1 )//&& digitalRead(UVCLED) == UVCON)
{
  watersensorCount++; }


if(watersensorCount > 5)
{
  digitalWrite(SOLONOID, LOW); 
   SOLONOIDStatus = 1;
   delay(1000);
   watersensorCount = 0; 

  }
else {
digitalWrite(SOLONOID, HIGH);
 SOLONOIDStatus = 0;

}


if (millis() - lastWaterSensorReset > 11000) {
  lastWaterSensorReset += 11000;
  watersensorCount = 0; 
}

}



//====================================================
// Menu
//====================================================
void menu(){
  
/////////Logo Splash Screen - One time using a the counter variable////////
    if (splash == 0) {
    LCD.CleanAll(WHITE);
    LCD.WorkingModeConf(OFF, ON, WM_BitmapMode);
    LCD.DrawScreenAreaAt(&bmmycoclimate_v3, 3, 22);
    LCD.WorkingModeConf(OFF, ON, WM_CharMode);
    delay(3000);
    LCD.CleanAll(BLACK);
    splash++;                     // Increment splash
  }

    
  
  switch (mNivel1){
    case 0:
      menu0();//Home
    break;
    case 1:
         if(mNivel2==1){menu11();
        }else if (mNivel2==2){menu12();
        }else if (mNivel2==3){menu13();
        }else if (mNivel2==4){menu14();
        }else if (mNivel2==5){menu15();
        }else if (mNivel2==6){menu16();
        }else if (mNivel2==7){menu17();
        }else if (mNivel2==8){menu18();
        }else if (mNivel2==9){menu19();
        }else if (mNivel2==10){menu20();}
        else{menu1();}
    break;
    case 2:
       menu2();
    break;
    case 3:
      menu3(); 
    break;
    case 4:
      menu4(); 
    break;
  }//switch
}
//----------------------------------------------------
// Home
//----------------------------------------------------
void menu0(){
  if (refresh){LCD.CleanAll(BLACK);refresh=0;}
  

  LCD.FontModeConf(Font_6x8, FM_MNL_AAA, WHITE_BAC);

  float result[3] = {0}; //SCD30 Results 
  
      

//Get Time from RTC
    RTclock.getTime();


//Refresh Screen every minute - avoid any wierdness 
if (RTclock.second == 30) {
        LCD.CleanAll(BLACK);} 
        
//Print Time and Date
    
    LCD.CharGotoXY(3,53);
     
    LCD.print(RTclock.hour, DEC); 
    LCD.print(":");
    if (RTclock.minute < 10){
    LCD.print("0");
    LCD.print(RTclock.minute, DEC);
    }else{
    LCD.print(RTclock.minute, DEC);
    }
    
    LCD.print("  ");
    LCD.print(RTclock.dayOfMonth, DEC);
    LCD.print("/");
    LCD.print(RTclock.month, DEC);
    LCD.print("/");
    LCD.print(RTclock.year, DEC);
    LCD.print("  ");
    switch (RTclock.dayOfWeek) { // Friendly printout the weekday
        case MON:
            LCD.print("MON");
            break;
        case TUE:
            LCD.print("TUE");
            break;
        case WED:
            LCD.print("WED");
            break;
        case THU:
            LCD.print("THU");
            break;
        case FRI:
            LCD.print("FRI");
            break;
        case SAT:
            LCD.print("SAT");
            break;
        case SUN:
            LCD.print("SUN");
            break; } 

///////////////////////PRINT MODE//////////////////////////////////   
mode = EEPROM.read(0);

 /*  //test out mode settings
    LCD.CharGotoXY(3,43);
    LCD.print (mode);
    LCD.CharGotoXY(20,43);
    LCD.print (CSETPOINT);
    LCD.CharGotoXY(50,43);
    LCD.print (HSETPOINT);
    LCD.CharGotoXY(80,43);
    LCD.print (TSETPOINT); 

    //test joystick values
    int x = analogRead(xPin);
    int y = analogRead(yPin);
    
    LCD.CharGotoXY(3,43);
    LCD.print (x);
    LCD.CharGotoXY(60,43);
    LCD.print (y);*/
   
    LCD.CharGotoXY(3,3);
    if (mode==0){LCD.print("Please select mode!");
    }else if (mode==1){LCD.print("Incubating");
    }else if (mode==2){LCD.print("Fruiting Cubensis");
    }else if (mode==3){LCD.print("Fruiting Lions Mane");
    }else if (mode==4){LCD.print("Fruiting Rieshi");
    }else if (mode==5){LCD.print("Fruiting Cordyceps");
    }else if (mode==6){LCD.print("Fruiting Oyster");
    }else if (mode==7){LCD.print("Fruiting Shiitake");
    }else if (mode==8){LCD.print("Fruiting Enoki");
    }else if (mode==9){LCD.print("Fruiting Maitake");
    }else if (mode==10){LCD.print("Fruiting Turkey Tail");
    }else if (mode==11){LCD.print("Fruiting Morel");
  }
  
/////////////////////PRINT SDC30 RESULTS//////////////////////////////////   

    
    if (scd30.isAvailable()) {
        scd30.getCarbonDioxideConcentration(result);
        LCD.CharGotoXY(3,13);       //Set the start coordinate.
        LCD.print("C02: "); 
        LCD.print(result[0]);
        LCD.CharGotoXY(73,13); 
        LCD.print(" ppm");

        LCD.CharGotoXY(3,23);       //Set the start coordinate.
        LCD.print("Temp: "); 
        LCD.print(result[1]);
        LCD.CharGotoXY(73,23); 
        LCD.print(" °C");  

        LCD.CharGotoXY(3,33);       //Set the start coordinate.
        LCD.print("Humidity: "); 
        LCD.print(result[2]); 
        LCD.print("%");
          }   


/////////////////////PRINT RELAY STATUS/////////////////
LCD.CharGotoXY(3,43);
if(digitalRead(HUMIDIFIER) == HON){LCD.print ("H ");}
  else{LCD.print ("  ");}

if(digitalRead(HFAN) == HFON){LCD.print ("HF ");}
  else{LCD.print ("   ");}

if(digitalRead(CFAN) == CFON){LCD.print ("CF ");}
  else{LCD.print ("   ");}

if(digitalRead(PCOOLER) == PCON){LCD.print ("PC ");}
  else{LCD.print ("   ");}

if(SOLONOIDStatus == 1){LCD.print ("S ");}   
  else{LCD.print ("  ");}

if(digitalRead(HPADS) == HPON){LCD.print ("HP ");}
  else{LCD.print ("   ");}

if(DNCstatus == 1){LCD.print ("L ");}
  else{LCD.print ("  ");}
            
if(UVCstatus == 1){LCD.print ("UV ");}
  else{LCD.print ("   ");}    




///////////////////////////////////////////////////////////////////
          
delay(100);

//Float to integer, avoids the float reading 0 and toggling on/off
if (result[0] > 0) {
resultC02 = (int) result[0];}

if (result[1] > 0) {
resultTemp =  result[1];}

if (result[2] > 0) {
resultHumi = (int) result[2];}



///////////////////////////Now begin humidifier control logic////////////////////////////
  if(digitalRead(HUMIDIFIER) == HON)
  {  
      //If HUMIDIFIER is on, check to see if the humidity is above setpoint 
     if( resultHumi > HSETPOINT )//+ HDEADBAND
     {
      //We've reached the upper limit, so kill the humidifier
      digitalWrite(HUMIDIFIER, HOFF);
      delay(5000);
      
      //Because both humidifier and C02 extraction use the HFAN, must check c02 values before turning off HFAN
        if(resultC02 > CSETPOINT ){ //+ CDEADBAND
           digitalWrite(HFAN, HFON);}
        else{ digitalWrite(HFAN, HFOFF);}
     }
  } else {
  
      if( resultHumi < HSETPOINT ){ //- HDEADBAND
     
      //We've reached the lower limit, so turn on the humidifier
      digitalWrite(HUMIDIFIER, HON);
      digitalWrite(HFAN, HFON);
      
     }
  }



////////////////////////Now begin Co2 Extraction control logic/////////////////////////
  if(digitalRead(CFAN) == CFOFF)
   {  
     if(resultC02 > CSETPOINT ) //+ CDEADBAND
     {
        digitalWrite(CFAN, CFON);
        digitalWrite(HFAN, HFON);

     }
     
 }else {
      if (resultC02 < CSETPOINT ){ //- CDEADBAND
          digitalWrite(CFAN, CFOFF);

          if(digitalRead(HUMIDIFIER) == HON){ // This is new code to leave HFAN on if the HUMIDIFIER is still working its magic. 
            digitalWrite(HFAN, HFON);
          }else{digitalWrite(HFAN, HFOFF);}
           
      }
       
   }





////////////////////////Now begin Peltier Cooler control logic/////////////////////////
  if(digitalRead(PCOOLER) == PCOFF)
   {  
     if(resultTemp > TSETPOINT )//+ TDEADBAND
     {
      digitalWrite(PCOOLER, PCOFF); // Swap PCOFF with PCON in this line if you want to use the perltier cooler
     // digitalWrite(PCFAN, PCFON); 
     }
     
 }else {
      if ( resultTemp < TSETPOINT ){   //- TDEADBAND
       digitalWrite(PCOOLER, PCOFF);
       //digitalWrite(PCFAN, PCFOFF); 

       }
       
   }


////////////////////////Now begin Heater Pads control logic/////////////////////////
  if(digitalRead(HPADS) == HPOFF)
   {  
     if(resultTemp < THSETPOINT )//+ THDEADBAND
     {
      digitalWrite(HPADS, HPON);
     }
     
 }else {
      if ( resultTemp > THSETPOINT ){   //- THDEADBAND
       digitalWrite(HPADS, HPOFF);

       }
       
   }



/////////////////////////////Day/Night time Cycle//////////////////////////////////////////  

if (RTclock.hour > 7 && RTclock.hour < 20 && DNC==1) {
    //digitalWrite(DNCLED, DNCON);
  DNCstatus=1;
  SetPixelsPurple();
  LCD.WorkingModeConf(OFF, ON, WM_CharMode); // new code to turn backlight on in daytime ////////////////////////////////////////////////////////////////////////////////////////////////
// colorWipe(strip.Color(  255, 255,   255), 50); // WHITE
} else {
  //digitalWrite(DNCLED, DNCOFF);
  DNCstatus=0;
  SetPixelsNone(); // NEO PIXELS OFF
  LCD.WorkingModeConf(OFF, OFF, WM_CharMode); // new code to turn backlight off at nighttime/////////////////////////////////////////////////////////////////////////////////////////////
}



/////////////UVC Timer//////////////////
  mState = digitalRead(magnetSensor);

if (RTclock.hour > 15 && RTclock.hour < 16 && mState == LOW) {
  UVCtimer=1;
  digitalWrite(UVCLED, UVCON);
  Serial.println ("on");
  UVCstatus=1;
}else {
  UVCtimer=0;
  digitalWrite(UVCLED, UVCOFF);
  Serial.println ("off");
  UVCstatus=0;
 }

}



/*
////////////////////////Draw Closed Sensor logic for UVC light////////////////////////////////////////////////// Old UVC LED logic
/
  mState = digitalRead(magnetSensor);
  
  if (mState == HIGH && UVCtimer==0){ //New code to turn of uvc off at daytime (less chance of exposure to UV and gives the light a break)//////////////////////////////////////////////////
    digitalWrite(UVCLED, UVCOFF);
    Serial.println ("off");
    UVCstatus=0;
  }
  else{
    digitalWrite(UVCLED, UVCON);
    Serial.println ("on");
    UVCstatus=1;
  }
}
*/





//----------------------------------------------------
// Menu 1
//----------------------------------------------------
void menu1(){
  if (refresh){LCD.CleanAll(BLACK);refresh=0;}
  LCD.FontModeConf(Font_6x8, FM_MNL_AAA, WHITE_BAC);
  digitalWrite(UVCLED, UVCOFF);// UVC light off just in case.
  LCD.CharGotoXY(3,3);
  LCD.print("Fruiting");

    
}
//-------------------------------------------------1.1
void menu11(){
  if (refresh){LCD.CleanAll(WHITE);refresh=0;}
  LCD.FontModeConf(Font_6x8, FM_MNL_AAA, BLACK_BAC);

  LCD.CharGotoXY(3,3);
  LCD.print("Cubensis");

  if (passFlag == 0) {
    LCD.WorkingModeConf(OFF, ON, WM_BitmapMode);
    LCD.DrawScreenAreaAt(&bmcubensis, 80, 15);
    LCD.WorkingModeConf(OFF, ON, WM_CharMode);
    passFlag++;                     // Increment passFlag
  }
  
 
  if(joyPos==1) {
   passFlag--;  
  }
 else if(joyPos==2) {
  passFlag--; 
 }
 
 else if(joyPos==5){
    //mode=2;
    EEPROM.write(0, 2); // write to mode
    passFlag--;  
    LCD.CleanAll(BLACK);
    refresh=0;
    mNivel1=0;
    
  }
}
//-------------------------------------------------1.2
void menu12(){
  if (refresh){LCD.CleanAll(WHITE);refresh=0;}
  
  LCD.CharGotoXY(3,3);
  LCD.print("Lions Mane");

  if (passFlag == 0) {
    LCD.WorkingModeConf(OFF, ON, WM_BitmapMode);
    LCD.DrawScreenAreaAt(&bmlionsmane, 65, 3);
    LCD.WorkingModeConf(OFF, ON, WM_CharMode);
    passFlag++;                     // Increment passFlag
  }
  
 
  if(joyPos==1) {
   passFlag--;  
  }
 else if(joyPos==2) {
  passFlag--; 
 }
 
 else if(joyPos==5){
    //mode=3;
    EEPROM.write(0, 3); // write to mode
    passFlag--;  
    LCD.CleanAll(BLACK);
    refresh=0;
    mNivel1=0;
    }
}
//-------------------------------------------------1.3
void menu13(){
  if (refresh){LCD.CleanAll(WHITE);refresh=0;}
  LCD.CharGotoXY(3,3);
  LCD.print("Rieshi");
 
  if (passFlag == 0) {
    LCD.WorkingModeConf(OFF, ON, WM_BitmapMode);
    LCD.DrawScreenAreaAt(&bmrieshi, 50, 10);
    LCD.WorkingModeConf(OFF, ON, WM_CharMode);
    passFlag++;                     // Increment passFlag
  }
  
 
  if(joyPos==1) {
   passFlag--;  
  }
 else if(joyPos==2) {
  passFlag--; 
 }
 
 else if(joyPos==5){
    //mode=4;
    EEPROM.write(0, 4); // write to mode
    passFlag--;  
    LCD.CleanAll(BLACK);
    refresh=0;
    mNivel1=0;
    }

  
}
//-------------------------------------------------1.4
void menu14(){
  if (refresh){LCD.CleanAll(WHITE);refresh=0;}
  LCD.CharGotoXY(3,3);
  LCD.print("Cordyceps");

  if (passFlag == 0) {
    LCD.WorkingModeConf(OFF, ON, WM_BitmapMode);
    LCD.DrawScreenAreaAt(&bmcordyceps, 80,9);
    LCD.WorkingModeConf(OFF, ON, WM_CharMode);
    passFlag++;                     // Increment passFlag
  }
  
 
  if(joyPos==1) {
   passFlag--;  
  }
 else if(joyPos==2) {
  passFlag--; 
 }
 
 else if(joyPos==5){
    //mode=5;
    EEPROM.write(0, 5); // write to mode
    passFlag--;  
    LCD.CleanAll(BLACK);
    refresh=0;
    mNivel1=0;
    
  }


  
}
//-------------------------------------------------1.5
void menu15(){
  if (refresh){LCD.CleanAll(WHITE);refresh=0;}
  LCD.CharGotoXY(3,3);
  LCD.print("Oyster");
  
   if (passFlag == 0) {
    LCD.WorkingModeConf(OFF, ON, WM_BitmapMode);
    LCD.DrawScreenAreaAt(&bmoyster, 70, 9);
    LCD.WorkingModeConf(OFF, ON, WM_CharMode);
    passFlag++;                     // Increment passFlag
  }
  
 
  if(joyPos==1) {
   passFlag--;  
  }
 else if(joyPos==2) {
  passFlag--; 
 }
 
 else if(joyPos==5){
    //mode=6;
    EEPROM.write(0, 6); // write to mode
    passFlag--;  
    LCD.CleanAll(BLACK);
    refresh=0;
    mNivel1=0;
    }

  
}




//-------------------------------------------------1.6
void menu16(){
  if (refresh){LCD.CleanAll(WHITE);refresh=0;}
  LCD.CharGotoXY(3,3);
  LCD.print("Shiitake");
    
    if (passFlag == 0) {
    LCD.WorkingModeConf(OFF, ON, WM_BitmapMode);
    LCD.DrawScreenAreaAt(&bmshiitake, 80, 15);
    LCD.WorkingModeConf(OFF, ON, WM_CharMode);
    passFlag++;                     // Increment passFlag
  }
  
 
  if(joyPos==1) {
   passFlag--;  
  }
 else if(joyPos==2) {
  passFlag--; 
 }
 
 else if(joyPos==5){
    //mode=7;
    EEPROM.write(0, 7); // write to mode
    passFlag--;  
    LCD.CleanAll(BLACK);
    refresh=0;
    mNivel1=0;
    }
}



//-------------------------------------------------1.7
void menu17(){
  if (refresh){LCD.CleanAll(WHITE);refresh=0;}
  LCD.CharGotoXY(3,3);
  LCD.print("Enoki");
   
   if (passFlag == 0) {
    LCD.WorkingModeConf(OFF, ON, WM_BitmapMode);
    LCD.DrawScreenAreaAt(&bmenoki, 80, 9);
    LCD.WorkingModeConf(OFF, ON, WM_CharMode);
    passFlag++;                     // Increment passFlag
  }
  
 
  if(joyPos==1) {
   passFlag--;  
  }
 else if(joyPos==2) {
  passFlag--; 
 }
 
 else if(joyPos==5){
    //mode=8;
    EEPROM.write(0, 8); // write to mode
    passFlag--;  
    LCD.CleanAll(BLACK);
    refresh=0;
    mNivel1=0;
    }
}

//-------------------------------------------------1.8
void menu18(){
  if (refresh){LCD.CleanAll(WHITE);refresh=0;}
  LCD.CharGotoXY(3,3);
  LCD.print("Maitake");
  if (passFlag == 0) {
    LCD.WorkingModeConf(OFF, ON, WM_BitmapMode);
    LCD.DrawScreenAreaAt(&bmmaitake, 60, 10);
    LCD.WorkingModeConf(OFF, ON, WM_CharMode);
    passFlag++;                     // Increment passFlag
  }
  
 
  if(joyPos==1) {
   passFlag--;  
  }
 else if(joyPos==2) {
  passFlag--; 
 }
 
 else if(joyPos==5){
    //mode=9;
    EEPROM.write(0, 9); // write to mode
    passFlag--;  
    LCD.CleanAll(BLACK);
    refresh=0;
    mNivel1=0;
    }
}

//-------------------------------------------------1.9
void menu19(){
  if (refresh){LCD.CleanAll(WHITE);refresh=0;}
  LCD.CharGotoXY(3,3);
  LCD.print("Turkey Tail");
   
   if (passFlag == 0) {
    LCD.WorkingModeConf(OFF, ON, WM_BitmapMode);
    LCD.DrawScreenAreaAt(&bmturkeytail, 80, 3);
    LCD.WorkingModeConf(OFF, ON, WM_CharMode);
    passFlag++;                     // Increment passFlag
  }
  
 
  if(joyPos==1) {
   passFlag--;  
  }
 else if(joyPos==2) {
  passFlag--; 
 }
 
 else if(joyPos==5){
    //mode=10;
    EEPROM.write(0, 10); // write to mode
    passFlag--;  
    LCD.CleanAll(BLACK);
    refresh=0;
    mNivel1=0;
    }
}

//-------------------------------------------------1.9 + .1
void menu20(){
  if (refresh){LCD.CleanAll(WHITE);refresh=0;}
  LCD.CharGotoXY(3,3);
  LCD.print("Morel");
  
  if (passFlag == 0) {
    LCD.WorkingModeConf(OFF, ON, WM_BitmapMode);
    LCD.DrawScreenAreaAt(&bmmorel, 80, 10);
    LCD.WorkingModeConf(OFF, ON, WM_CharMode);
    passFlag++;                     // Increment passFlag
  }
  
 
  if(joyPos==1) {
   passFlag--;  
  }
 else if(joyPos==2) {
  passFlag--; 
 }
 
 else if(joyPos==5){
    //mode=11;
    EEPROM.write(0, 11); // write to mode
    passFlag--;  
    LCD.CleanAll(BLACK);
    refresh=0;
    mNivel1=0;
    }
}



//----------------------------------------------------
// Menu 2
//----------------------------------------------------
void menu2(){
  if (refresh){LCD.CleanAll(BLACK);refresh=0;}
  LCD.FontModeConf(Font_6x8, FM_MNL_AAA, WHITE_BAC);

  LCD.CharGotoXY(3,3);
  LCD.print("Incubation");
  if(joyPos==5){
    //mode=1;
    EEPROM.write(0, 1); // write to mode
    LCD.CleanAll(BLACK);
    LCD.CharGotoXY(3,3);
    LCD.print("Lower the Fan intake");
    delay(3000);
    LCD.CleanAll(BLACK);
    refresh=0;
    mNivel1=0;

  }
}

//----------------------------------------------------
// Menu 3 - Clear Mode
//----------------------------------------------------
void menu3(){
  if (refresh){LCD.CleanAll(BLACK);refresh=0;}
  LCD.FontModeConf(Font_6x8, FM_MNL_AAA, WHITE_BAC);

  LCD.CharGotoXY(3,3);
  LCD.print("Clear Mode");
  if(joyPos==5){
    //mode=0;
    EEPROM.write(0, 0); // write to mode
    LCD.CleanAll(BLACK);
    refresh=0;
    mNivel1=0;
  }
}



//----------------------------------------------------
// Menu 4 - Show Off
//----------------------------------------------------
void menu4(){
  if (refresh){LCD.CleanAll(BLACK);refresh=0;}
  LCD.FontModeConf(Font_6x8, FM_MNL_AAA, WHITE_BAC);

  LCD.CharGotoXY(3,3);
  LCD.print("Show Off");
  if(joyPos==5){
    
LCD.CharGotoXY(3,30);    
LCD.print("Oooooo Weeeee");


//digitalWrite(DNCLED, DNCON);
digitalWrite(HFAN, HFON);
digitalWrite(HUMIDIFIER, HON);

wdt_disable(); // disable watchdog timer
SetPixelsPurple();

delay(33000);
//digitalWrite(DNCLED, DNCOFF);
SetPixelsNone(); // NEO PIXELS OFF

digitalWrite(HFAN, HFOFF);
digitalWrite(HUMIDIFIER, HOFF);

wdt_enable(WDTO_8S); //Re-enable watchdog timer
wdt_reset(); //Reset watchdog timer

    LCD.CleanAll(BLACK);
    refresh=0;
    mNivel1=0;
  }
}



//----------------------------------------------------
// Mode Settings
//----------------------------------------------------
void modeSettings(){
  
//Cleared Settings - OFF
if (mode==0){
  TSETPOINT=100;
  THSETPOINT=0;
  HSETPOINT=0;
  CSETPOINT=10000;
  DNC=0;
}

//Incubation Settings
else if (mode==1){
  TSETPOINT=27;
  THSETPOINT=21;
  HSETPOINT=40;
  CSETPOINT=10000;
  DNC=0;
}


//Cubensis Settings
else if (mode==2){
  TSETPOINT=33;
  THSETPOINT=0;
  HSETPOINT=81;
  CSETPOINT=990;
  DNC=1;
}


//Lions Mane Settings
else if (mode==3){
  TSETPOINT=23;
  THSETPOINT=9;
  HSETPOINT=75;
  CSETPOINT=990;
  DNC=1;
}


//Rieshi Settings
else if (mode==4){
  TSETPOINT=23;
  THSETPOINT=0;
  HSETPOINT=90;
  CSETPOINT=850;
  DNC=1;
}


//Cordyceps Settings
else if (mode==5){
  TSETPOINT=23;
  THSETPOINT=0;
  HSETPOINT=90;
  CSETPOINT=850;
  DNC=1;
}


//Oyster Settings
else if (mode==6){
  TSETPOINT=33;
  THSETPOINT=0;
  HSETPOINT=75;
  CSETPOINT=990;
  DNC=1;
}


//Shiitake Settings
else if (mode==7){
  TSETPOINT=23;
  THSETPOINT=0;
  HSETPOINT=90;
  CSETPOINT=850;
  DNC=1;
}

//Enoki Settings
else if (mode==8){
  TSETPOINT=23;
  THSETPOINT=0;
  HSETPOINT=90;
  CSETPOINT=850;
  DNC=1;
}


//Maitake Settings
else if (mode==9){
  TSETPOINT=23;
  THSETPOINT=0;
  HSETPOINT=90;
  CSETPOINT=850;
  DNC=1;
}

//Turkey Tail Settings
else if (mode==10){
  TSETPOINT=23;
  THSETPOINT=0;
  HSETPOINT=90;
  CSETPOINT=850;
  DNC=1;
}

//Morel Settings
else if (mode==11){
  TSETPOINT=23;
  THSETPOINT=0;
  HSETPOINT=90;
  CSETPOINT=850;
  DNC=1;
}

}


//====================================================
// Neo Pixels behavior
//====================================================
void SetPixelsPurple(){

for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(128, 0, 128));

    pixels.show();   // Send the updated pixel colors to the hardware.

    //delay(DELAYVAL); // Pause before next pass through loop
  }
}

void SetPixelsNone(){

for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    //delay(DELAYVAL); // Pause before next pass through loop
  }
}














//====================================================
// Control Joystic
//====================================================
void controlJoystick(){
 leeJoystick();
  if(PQCP) {
    PQCP=0;
 
          if (mNivel1<4&&joyPos==3){mNivel1++;    //DOWN
            refresh=1;
            mNivel2=0;}
          if (mNivel1>0&&joyPos==4){mNivel1--;    //UP
            mNivel2=0;
            refresh=1;}
          if (mNivel2<10&&joyPos==1){mNivel2++;   //RIGHT
            refresh=1;}
          if (mNivel2>0&&joyPos==2){mNivel2--;    //LEFT
           refresh=1;}
        
  }
}
int leeJoystick(){
  int x = analogRead(xPin);
  int y = analogRead(yPin);
  int k = x > 900 && x < 1300;   //   x==1020 || x==1021 || x==1022 || x==1023;
    if((k == HIGH)){joyRead=5;  //Button
    }else if(x>520){joyRead=1;  //x+
    }else if(x<500){joyRead=2;  //x-
    }else if(y>510){joyRead=3;  //y+
    }else if(y<500){joyRead=4;  //y-
    }else{joyRead=0;}

  if (joyRead != lastJoyPos){lastDebounceTime = millis();}
  if(((millis() - lastDebounceTime) > debounceDelay)&&(joyRead!=joyPos)){
    joyPos=joyRead;
    if(!PQCP){PQCP=1;}
    }
  if(((millis() - lastDebounceTime) > (5*debounceDelay))&&(joyPos==3||joyPos==4)){
    joyPos=joyRead;                     //repeat time only for UP/DOWN
    if(!PQCP){PQCP=1;}
    }
  lastJoyPos=joyRead;
}
