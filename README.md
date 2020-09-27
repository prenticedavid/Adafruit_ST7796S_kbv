# Adafruit-style Arduino library for ST7796S SPI displays.

This inherits from Adafruit_GFX and Adafruit_SPITFT libraries.
It should behave exactly like Adafruit's own Adafruit_ILI9341 but for 320x480 ST7796S SPI displays.

Adafruit_ST7796S_kbv inherits all the methods from  
the Adafruit_GFX class: https://learn.adafruit.com/adafruit-gfx-graphics-library/overview  
and Print class: https://www.arduino.cc/en/Serial/Print

Note that the popular Chinese Red SPI displays require 3.3V logic signals.  

5V Uno, Mega, Nano, Leo, ... MUST use level-shifters for CS, RESET, DC, SDI, SCK, LED.  
3V Zero, Due, STM32, ... can connect directly.

Most Red pcbs contain an AMS1117-3.3 regulator.  
None have a pullup resistor on LCD_RESET pin.  
Some have a transistor switch on LED pin.   You can switch backlight LED on or off with a GPIO pin (or PWM).  
The new Red ST7796S boards have an AMS1117-3.3 regulator and an LED transistor.    

Connect VCC = 5V if there is a regulator.  Use VCC = 3.3V if there is no regulator.  
Connect LED to a GPIO if there is a transistor (or 5V with a 1k0 series resistor).  
Otherwise connect LED to 5V with a 39R series resistor.

You MUST use the RST argument in both Software and Hardware constructors.  
Note that many Adafruit examples omit the RST argument (because Adafruit pcbs have a RST pullup resistor)


Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.
MIT license, all text above must be included in any redistribution

To download. click the DOWNLOADS button in the top right corner, rename the uncompressed folder Adafruit_ST7796S_kbv. Check that the Adafruit_ST7796S_kbv folder contains Adafruit_ST7796S_kbv.cpp and Adafruit_ST7796S_kbv.

Place the Adafruit_ST7796S_kbv library folder your arduinosketchfolder/libraries/ folder. You may need to create the libraries subfolder if its your first library. Restart the IDE

Also requires the Adafruit_GFX library for Arduino.
