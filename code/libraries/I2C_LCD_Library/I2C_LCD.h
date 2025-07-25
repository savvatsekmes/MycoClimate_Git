/******************************************************************************
  I2C_LCD.h - I2C_LCD library Version 1.22
  Copyright (C), 2016, Sparking Work Space. All right reserved.

 ******************************************************************************
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is build for I2C_LCD12864. Please do not use this library on 
  any other devices, that could cause unpredictable damage to the unknow device.
  See the GNU Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*******************************************************************************/

#ifndef __I2C_LCD_H_
#define __I2C_LCD_H_


#include <Arduino.h>
//#include <avr/pgmspace.h>
//#include <Wire.h>
#include <WSWire.h>

#include <inttypes.h>
#include "Print.h"

#ifndef     TRUE
#define     TRUE            (1)
#endif

#ifndef     FALSE
#define     FALSE           (0)
#endif

/********************************Readme first***************************
If you want to reduce the code size and you do not need full API Library
or 2D Graphic API Library, please set the realted define to "FALSE".     
                                                                         
If you need all API Library or 2D Graphic API Library, please set the    
realted define to "TRUE".                                                
                                                                         
The default setting is use full API library and 2D Graphic API Library.  
*************************************************************************/

#define     SUPPORT_FULL_API_LIB        TRUE
#define     SUPPORT_2D_GRAPHIC_LIB      TRUE

/********************************End of readme***************************/


/**************************************************************
      Defines for hardware parameters
***************************************************************/

#define     I2C_LCD_X_SIZE_MAX      (128)
#define     I2C_LCD_Y_SIZE_MAX      (64)

#define     I2C_LCD_NUM_OF_FONT      (7)

#define     I2C_LCD_TRANS_ONCE_BYTE_MAX      (6)


/**************************************************************
      Defines for I2C driver APIs.
      NOTE: 
      1. If you want to use I2C_LCD on other MCUs or platforms, please change the I2C driver APIs show below.
      2. You also can redefine these five functions at the begin of I2C_LCD.c.
         Init()
         ReadByteFromReg()
         WriteByteToReg()
         ReadSeriesFromReg()
         WriteSeriesToReg()  
***************************************************************/
#define     I2C_BUS_Init()                               Wire.begin()                       /*Init I2C IOs or hardware I2C register*/
#define     I2C_BUS_BeginTransmission(addr)              Wire.beginTransmission(addr)       /*Send I2C start signal*/
#define     I2C_BUS_EndTransmission(addr)                Wire.endTransmission(addr)         /*Send I2C stop signal*/
#define     I2C_BUS_WriteByte(data)                      Wire.write(data)                   /*Write one byte to slaver*/
#define     I2C_BUS_ReadByte(data)                       Wire.read(data)                    /*Read one byte from slaver*/
#define     I2C_BUS_RequestFrom(addr, len)               Wire.requestFrom(addr, len)        /*Request bytes from slaver*/
#define     I2C_BUS_Available()                          Wire.available()                   /*Get the num of received bytes in buffer, return 0 means no data received*/


/**************************************************************
      Defines for graphic constant
***************************************************************/
#define	________	0x0
#define	_______X	0x1
#define	______X_	0x2
#define	______XX	0x3
#define	_____X__	0x4
#define	_____X_X	0x5
#define	_____XX_	0x6
#define	_____XXX	0x7
#define	____X___	0x8
#define	____X__X	0x9
#define	____X_X_	0xa
#define	____X_XX	0xb
#define	____XX__	0xc
#define	____XX_X	0xd
#define	____XXX_	0xe
#define	____XXXX	0xf
#define	___X____	0x10
#define	___X___X	0x11
#define	___X__X_	0x12
#define	___X__XX	0x13
#define	___X_X__	0x14
#define	___X_X_X	0x15
#define	___X_XX_	0x16
#define	___X_XXX	0x17
#define	___XX___	0x18
#define	___XX__X	0x19
#define	___XX_X_	0x1a
#define	___XX_XX	0x1b
#define	___XXX__	0x1c
#define	___XXX_X	0x1d
#define	___XXXX_	0x1e
#define	___XXXXX	0x1f
#define	__X_____	0x20
#define	__X____X	0x21
#define	__X___X_	0x22
#define	__X___XX	0x23
#define	__X__X__	0x24
#define	__X__X_X	0x25
#define	__X__XX_	0x26
#define	__X__XXX	0x27
#define	__X_X___	0x28
#define	__X_X__X	0x29
#define	__X_X_X_	0x2a
#define	__X_X_XX	0x2b
#define	__X_XX__	0x2c
#define	__X_XX_X	0x2d
#define	__X_XXX_	0x2e
#define	__X_XXXX	0x2f
#define	__XX____	0x30
#define	__XX___X	0x31
#define	__XX__X_	0x32
#define	__XX__XX	0x33
#define	__XX_X__	0x34
#define	__XX_X_X	0x35
#define	__XX_XX_	0x36
#define	__XX_XXX	0x37
#define	__XXX___	0x38
#define	__XXX__X	0x39
#define	__XXX_X_	0x3a
#define	__XXX_XX	0x3b
#define	__XXXX__	0x3c
#define	__XXXX_X	0x3d
#define	__XXXXX_	0x3e
#define	__XXXXXX	0x3f
#define	_X______	0x40
#define	_X_____X	0x41
#define	_X____X_	0x42
#define	_X____XX	0x43
#define	_X___X__	0x44
#define	_X___X_X	0x45
#define	_X___XX_	0x46
#define	_X___XXX	0x47
#define	_X__X___	0x48
#define	_X__X__X	0x49
#define	_X__X_X_	0x4a
#define	_X__X_XX	0x4b
#define	_X__XX__	0x4c
#define	_X__XX_X	0x4d
#define	_X__XXX_	0x4e
#define	_X__XXXX	0x4f
#define	_X_X____	0x50
#define	_X_X___X	0x51
#define	_X_X__X_	0x52
#define	_X_X__XX	0x53
#define	_X_X_X__	0x54
#define	_X_X_X_X	0x55
#define	_X_X_XX_	0x56
#define	_X_X_XXX	0x57
#define	_X_XX___	0x58
#define	_X_XX__X	0x59
#define	_X_XX_X_	0x5a
#define	_X_XX_XX	0x5b
#define	_X_XXX__	0x5c
#define	_X_XXX_X	0x5d
#define	_X_XXXX_	0x5e
#define	_X_XXXXX	0x5f
#define	_XX_____	0x60
#define	_XX____X	0x61
#define	_XX___X_	0x62
#define	_XX___XX	0x63
#define	_XX__X__	0x64
#define	_XX__X_X	0x65
#define	_XX__XX_	0x66
#define	_XX__XXX	0x67
#define	_XX_X___	0x68
#define	_XX_X__X	0x69
#define	_XX_X_X_	0x6a
#define	_XX_X_XX	0x6b
#define	_XX_XX__	0x6c
#define	_XX_XX_X	0x6d
#define	_XX_XXX_	0x6e
#define	_XX_XXXX	0x6f
#define	_XXX____	0x70
#define	_XXX___X	0x71
#define	_XXX__X_	0x72
#define	_XXX__XX	0x73
#define	_XXX_X__	0x74
#define	_XXX_X_X	0x75
#define	_XXX_XX_	0x76
#define	_XXX_XXX	0x77
#define	_XXXX___	0x78
#define	_XXXX__X	0x79
#define	_XXXX_X_	0x7a
#define	_XXXX_XX	0x7b
#define	_XXXXX__	0x7c
#define	_XXXXX_X	0x7d
#define	_XXXXXX_	0x7e
#define	_XXXXXXX	0x7f
#define	X_______	0x80
#define	X______X	0x81
#define	X_____X_	0x82
#define	X_____XX	0x83
#define	X____X__	0x84
#define	X____X_X	0x85
#define	X____XX_	0x86
#define	X____XXX	0x87
#define	X___X___	0x88
#define	X___X__X	0x89
#define	X___X_X_	0x8a
#define	X___X_XX	0x8b
#define	X___XX__	0x8c
#define	X___XX_X	0x8d
#define	X___XXX_	0x8e
#define	X___XXXX	0x8f
#define	X__X____	0x90
#define	X__X___X	0x91
#define	X__X__X_	0x92
#define	X__X__XX	0x93
#define	X__X_X__	0x94
#define	X__X_X_X	0x95
#define	X__X_XX_	0x96
#define	X__X_XXX	0x97
#define	X__XX___	0x98
#define	X__XX__X	0x99
#define	X__XX_X_	0x9a
#define X__XX_XX	0x9b
#define X__XXX__	0x9c
#define X__XXX_X	0x9d
#define	X__XXXX_	0x9e
#define	X__XXXXX	0x9f
#define	X_X_____	0xa0
#define	X_X____X	0xa1
#define	X_X___X_	0xa2
#define	X_X___XX	0xa3
#define	X_X__X__	0xa4
#define	X_X__X_X	0xa5
#define	X_X__XX_	0xa6
#define	X_X__XXX	0xa7
#define	X_X_X___	0xa8
#define	X_X_X__X	0xa9
#define	X_X_X_X_	0xaa
#define	X_X_X_XX	0xab
#define	X_X_XX__	0xac
#define	X_X_XX_X	0xad
#define	X_X_XXX_	0xae
#define	X_X_XXXX	0xaf
#define	X_XX____	0xb0
#define X_XX___X	0xb1
#define	X_XX__X_	0xb2
#define	X_XX__XX	0xb3
#define	X_XX_X__	0xb4
#define	X_XX_X_X	0xb5
#define	X_XX_XX_	0xb6
#define	X_XX_XXX	0xb7
#define	X_XXX___	0xb8
#define	X_XXX__X	0xb9
#define	X_XXX_X_	0xba
#define	X_XXX_XX	0xbb
#define	X_XXXX__	0xbc
#define	X_XXXX_X	0xbd
#define	X_XXXXX_	0xbe
#define	X_XXXXXX	0xbf
#define	XX______	0xc0
#define	XX_____X	0xc1
#define	XX____X_	0xc2
#define	XX____XX	0xc3
#define	XX___X__	0xc4
#define	XX___X_X	0xc5
#define	XX___XX_	0xc6
#define	XX___XXX	0xc7
#define	XX__X___	0xc8
#define	XX__X__X	0xc9
#define	XX__X_X_	0xca
#define	XX__X_XX	0xcb
#define	XX__XX__	0xcc
#define	XX__XX_X	0xcd
#define	XX__XXX_	0xce
#define XX__XXXX	0xcf
#define	XX_X____	0xd0
#define	XX_X___X	0xd1
#define	XX_X__X_	0xd2
#define	XX_X__XX	0xd3
#define	XX_X_X__	0xd4
#define	XX_X_X_X	0xd5
#define	XX_X_XX_	0xd6
#define	XX_X_XXX	0xd7
#define	XX_XX___	0xd8
#define	XX_XX__X	0xd9
#define	XX_XX_X_	0xda
#define	XX_XX_XX	0xdb
#define	XX_XXX__	0xdc
#define	XX_XXX_X	0xdd
#define	XX_XXXX_	0xde
#define	XX_XXXXX	0xdf
#define	XXX_____	0xe0
#define	XXX____X	0xe1
#define	XXX___X_	0xe2
#define	XXX___XX	0xe3
#define	XXX__X__	0xe4
#define	XXX__X_X	0xe5
#define	XXX__XX_	0xe6
#define	XXX__XXX	0xe7
#define	XXX_X___	0xe8
#define	XXX_X__X	0xe9
#define	XXX_X_X_	0xea
#define	XXX_X_XX	0xeb
#define	XXX_XX__	0xec
#define	XXX_XX_X	0xed
#define	XXX_XXX_	0xee
#define	XXX_XXXX	0xef
#define	XXXX____	0xf0
#define	XXXX___X	0xf1
#define	XXXX__X_	0xf2
#define	XXXX__XX	0xf3
#define	XXXX_X__	0xf4
#define	XXXX_X_X	0xf5
#define	XXXX_XX_	0xf6
#define	XXXX_XXX	0xf7
#define	XXXXX___	0xf8
#define	XXXXX__X	0xf9
#define	XXXXX_X_	0xfa
#define	XXXXX_XX	0xfb
#define	XXXXXX__	0xfc
#define	XXXXXX_X	0xfd
#define	XXXXXXX_	0xfe
#define	XXXXXXXX	0xff



typedef struct {
  uint8_t XSize;
  uint8_t YSize;
  uint8_t BytesPerLine;
  uint8_t BitsPerPixel;
  const unsigned char *pData;
} GUI_Bitmap_t;


/*typedef struct {
  uint8_t YSize;
  uint8_t offset;
} Font_Info_t;*/


#define    ArrowLeft        0x1c
#define    ArrowRight       0x1d
#define    ArrowUp          0x1e
#define    ArrowDown        0x1f




enum LCD_RegAddress
{
    FontModeRegAddr=1,
    CharXPosRegAddr=2,
    CharYPosRegAddr=3,

    CursorConfigRegAddr=16,
    CursorXPosRegAddr=17,
    CursorYPosRegAddr=18,
    CursorWidthRegAddr=19,
    CursorHeightRegAddr=20, 	//8

    DisRAMAddr=32,				//9
    ReadRAM_XPosRegAddr=33,
    ReadRAM_YPosRegAddr=34,
    WriteRAM_XPosRegAddr=35,
    WriteRAM_YPosRegAddr=36,
    

    DrawDotXPosRegAddr=64, 		//14
    DrawDotYPosRegAddr=65,

    DrawLineStartXRegAddr=66,
    DrawLineEndXRegAddr=67,
    DrawLineStartYRegAddr=68,
    DrawLineEndYRegAddr=69,

    DrawRectangleXPosRegAddr=70,	//20
    DrawRectangleYPosRegAddr=71,
    DrawRectangleWidthRegAddr=72,
    DrawRectangleHeightRegAddr=73,
    DrawRectangleModeRegAddr=74,

    DrawCircleXPosRegAddr=75,
    DrawCircleYPosRegAddr=76,
    DrawCircleRRegAddr=77,
    DrawCircleModeRegAddr=78,

    DrawBitmapXPosRegAddr=79,
    DrawBitmapYPosRegAddr=80,
    DrawBitmapWidthRegAddr=81,
    DrawBitmapHeightRegAddr=82, 	//31
    

    DisplayConfigRegAddr=128,		//32
    WorkingModeRegAddr=129,
    BackLightConfigRegAddr=130,
    ContrastConfigRegAddr=131,
    DeviceAddressRegAddr=132
};

#ifdef  SUPPORT_FULL_API_LIB
#if  SUPPORT_FULL_API_LIB == TRUE

enum LCD_ColorSort
{
    WHITE=0x00,
    BLACK=0xff
};

enum LCD_CharMode
{
    WHITE_BAC=0x00,
    WHITE_NO_BAC=0x40,
    BLACK_BAC=0x80,
    BLACK_NO_BAC=0xc0
};

enum LCD_DrawMode
{
    WHITE_NO_FILL=0x00,
    WHITE_FILL=0x01,
    BLACK_NO_FILL=0x02,
    BLACK_FILL=0x03
};

enum LCD_DisplayMode
{
    AllREV=0x00,
    AllNOR=0x80
};

enum LCD_SwitchState
{
    OFF=0x00,
    ON=0x01
};


enum LCD_FontSort
{
    Font_6x8=0x00,
    Font_6x12=0x01,    
    Font_8x16_1=0x02,
    Font_8x16_2=0x03,
    Font_10x20=0x04,
    Font_12x24=0x05,
    Font_16x32=0x06
};

enum LCD_FontMode
{
    FM_ANL_AAA=0x00,	//FM_AutoNewLine_AutoAddrAdd
    FM_ANL_MAA=0x10,	//FM_AanualNewLine_ManualAddrAdd
    FM_MNL_MAA=0x30,	//FM_ManualNewLine_ManualAddrAdd
    FM_MNL_AAA=0x20 	//FM_ManualNewLine_AutoAddrAdd
};

enum LCD_WorkingMode
{
    WM_CharMode=0x00,	   //Common mode, put char or dot
    WM_BitmapMode=0x01,
    WM_RamMode=0x02	 //8Bit deal mode, deal with 8-bit RAM directly
};

enum LCD_SettingMode
{
    LOAD_TO_RAM=0x00,
    LOAD_TO_EEPROM=0x80,
};

#endif
#endif


class I2C_LCD: public Print
{
public:
    I2C_LCD(void);
    void Init(void);
    uint8_t ReadByteFromReg(enum LCD_RegAddress regAddr);
    void WriteByteToReg(enum LCD_RegAddress regAddr, uint8_t buf);
    void ReadSeriesFromReg(enum LCD_RegAddress regAddr, uint8_t *buf, int8_t length);
    void WriteSeriesToReg(enum LCD_RegAddress regAddr, const uint8_t *buf, uint8_t length);

#ifdef  SUPPORT_FULL_API_LIB
#if  SUPPORT_FULL_API_LIB == TRUE

    void FontModeConf(enum LCD_FontSort font, enum LCD_FontMode mode, enum LCD_CharMode cMode);
    void CharGotoXY(uint8_t x, uint8_t y);
    void DispCharAt(char buf, uint8_t x, uint8_t y);
    void DispStringAt(char *buf, uint8_t x, uint8_t y);

    virtual size_t write(uint8_t);
    using Print::write;
    
    void CursorConf(enum LCD_SwitchState swi, uint8_t freq);
    void CursorGotoXY(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
    
#ifdef  SUPPORT_2D_GRAPHIC_LIB
#if  SUPPORT_2D_GRAPHIC_LIB == TRUE

    void DrawDotAt(uint8_t x, uint8_t y, enum LCD_ColorSort color);
    void DrawHLineAt(uint8_t startX, uint8_t endX, uint8_t y, enum LCD_ColorSort color);
    void DrawVLineAt(uint8_t startY, uint8_t endY, uint8_t x, enum LCD_ColorSort color);
    void DrawLineAt(uint8_t startX, uint8_t endX, uint8_t startY, uint8_t endY, enum LCD_ColorSort color);
    void DrawRectangleAt(uint8_t x, uint8_t y, uint8_t width, uint8_t height, enum LCD_DrawMode mode);
    void DrawCircleAt(int8_t x, int8_t y, uint8_t r, enum LCD_DrawMode mode);
    void DrawScreenAreaAt(GUI_Bitmap_t *bitmap, uint8_t x, uint8_t y);
    void DrawFullScreen(const uint8_t *buf);
    
#endif
#endif
   
    uint8_t ReadByteDispRAM(uint8_t x, uint8_t y);
    void WriteByteDispRAM(uint8_t buf, uint8_t x, uint8_t y);
    void ReadSeriesDispRAM(uint8_t *buf, int8_t length, uint8_t x, uint8_t y);
    void WriteSeriesDispRAM(uint8_t *buf, int8_t length, uint8_t x, uint8_t y);
    //void ReadByteTest(enum LCD_RegAddress regAddr);
    
    
    void DisplayConf(enum LCD_DisplayMode mode);
    void WorkingModeConf(enum LCD_SwitchState logoSwi, enum LCD_SwitchState backLightSwi, enum LCD_WorkingMode mode);
    void BacklightConf(enum LCD_SettingMode mode, uint8_t buf);
    void ContrastConf(enum LCD_SettingMode mode, uint8_t buf);
    void DeviceAddrEdit(uint8_t newAddr);

    void CleanAll(enum LCD_ColorSort color);

private:
    void ReadRAMGotoXY(uint8_t x, uint8_t y);
    void WriteRAMGotoXY(uint8_t x, uint8_t y);
    
    void SendBitmapData(const uint8_t *buf, uint8_t length);

#endif
#endif
};



extern uint8_t I2C_LCD_ADDRESS;

#endif



