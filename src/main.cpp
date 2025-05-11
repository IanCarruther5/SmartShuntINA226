/*
    INA226 Bi-directional Current/Power Monitor. Simple Example.
    Read more:
   http://www.jarzebski.pl/arduino/czujniki-i-sensory/cyfrowy-czujnik-pradu-mocy-ina226.html
    GIT: https://github.com/jarzebski/Arduino-INA226
    Web: http://www.jarzebski.pl
    (c) 2014 by Korneliusz Jarzebski
*/
#include <Arduino.h>
#include <Wire.h>
#include <INA226.h>


#include "common.h"
#include "sensorHandling.h"
#include "webHandling.h"
#include "modbusHandling.h"
#include "victronHandling.h"
#include "temperature.h"
#include "display.h"


void setup() {
      // Start the Serial Monitor at a baud rate of 115200
  Serial.begin(115200);
  
  // Print an initial message to the Serial Monitor
  Serial.println("ESP32 is ready. Please enter a message:");
#if ARDUINO_USB_CDC_ON_BOOT
    SERIAL_VICTRON.begin(19200, SERIAL_8N1, RX, TX);
    // there seems to be a bug in the Arduine core that 
    // prevents RX from working. The next line fixes that....
    SERIAL_VICTRON.setPins(RX, TX, -1, -1);
    SERIAL_DBG.begin(115200);
#else
    SERIAL_VICTRON.begin(19200);
    SERIAL_DBG.begin(19200);
#endif

    wifiSetup();

#if (SOC_UART_NUM > 1)
    SERIAL_MODBUS.begin(9600, SERIAL_8N2);
#endif

displaysetup();
    sensorInit();
    temperatureSetup();
    modbusInit();
    victronInit();
    delay(1000);
    displayMessage("test");
}

void loop() {
//displayMessage("test");
    wifiLoop();
    if (gParamsChanged) {
        modbusInit();
        victronInit();
    }
    sensorLoop();
   
     temperatureLoop();
     //testdrawstyles();
         modbusLoop();
         
    victronLoop();
    gParamsChanged = false;
    displayloop();
}
