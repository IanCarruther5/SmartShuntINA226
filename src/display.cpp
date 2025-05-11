#include "display.h"  
#include "common.h"
#include "statusHandling.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

 #include <SPI.h>
 #include <Wire.h>
 #include <Adafruit_GFX.h>
 #include <Adafruit_SSD1306.h>
 
 #define SCREEN_WIDTH 128 // OLED display width, in pixels
 #define SCREEN_HEIGHT 64 // OLED display height, in pixels
 
 // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
 // The pins for I2C are defined by the Wire-library. 
 // On an arduino UNO:       A4(SDA), A5(SCL)
 // On an arduino MEGA 2560: 20(SDA), 21(SCL)
 // On an arduino LEONARDO:   2(SDA),  3(SCL), ...
 #define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
 #define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
 Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
 #define LOGO_HEIGHT   16
 #define LOGO_WIDTH    16
int t=0;
bool gDisplayInitialized =false;
 
 void displaysetup() {
  Wire.begin(PIN_SDA,PIN_SCL); 
   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
   if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    SERIAL_DBG.println(F("SSD1306 allocation failed"));
     return;
   }
   gDisplayInitialized=true;
   // Show initial display buffer contents on the screen --
   // the library initializes this with an Adafruit splash screen.
   display.display();
   delay(2000); // Pause for 2 seconds
 
   // Clear the buffer
   display.clearDisplay();
 
   // Draw a single pixel in white
   display.drawPixel(10, 10, SSD1306_WHITE);
 
   // Show the display buffer on the screen. You MUST call display() after
   // drawing commands to make them visible on screen!
   display.display();
   delay(2000);
   // display.display() is NOT necessary after every single drawing command,
   // unless that's what you want...rather, you can batch up a bunch of
   // drawing operations and then update the screen all at once by calling
   // display.display(). These examples demonstrate both approaches...
 

 
   // Invert and restore display, pausing in-between
   display.invertDisplay(true);
   delay(1000);
   display.invertDisplay(false);
   delay(1000);
 displayMessage("init");
   //testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
 }
 
 void displayloop() {
  if(!gDisplayInitialized )
  {
    return;
  }
  t++;
    if(t==200)
    {
      displayPower();
      t=0;
    }
    if(t==100)
    {
      displayTemperature();
    }
 }
 
 void displayMessage(String message) {

  SERIAL_DBG.println("display message: " +message);
  if(!gDisplayInitialized )
  {
    return;
  }
  display.clearDisplay();

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(message.c_str());

  display.display();
}
 void displayPower(void) {
  SERIAL_DBG.println(F("display power values"));
  if(!gDisplayInitialized )
  {
    return;
  }
   display.clearDisplay();
 
   display.setTextSize(2);             // Normal 1:1 pixel scale
   display.setTextColor(SSD1306_WHITE);        // Draw white text
   display.setCursor(0,0);             // Start at top-left corner
String s = "P: " + String(gBattery.voltage()*gBattery.current())+" W";
   display.println(s.c_str());
   s = "V: " + String(gBattery.voltage())+" V";
   display.println(s.c_str());
s="I: " + String(gBattery.current(),3) + " A";
   display.println(s.c_str());
 
   display.display();
 }
 
 
 void displayTemperature(void) {

  SERIAL_DBG.println(F("display power values"));
  if(!gDisplayInitialized )
  {
    return;
  }
   display.clearDisplay();
 
   display.setTextSize(2);             // Normal 1:1 pixel scale
   display.setTextColor(SSD1306_WHITE);        // Draw white text
   display.setCursor(0,0);             // Start at top-left corner
String s = "T: " + String(gBattery.temperature())+" C";
   display.println(s.c_str());
   s = "H: " + String(gBattery.humidity())+" %";
   display.println(s.c_str());
 
   display.display();
 }