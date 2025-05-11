#include <Adafruit_AHTX0.h>

#include "statusHandling.h"
#include "temperature.h"
#include "display.h"

Adafruit_AHTX0 aht;
bool gTempSensorInitialized=false;
void temperatureSetup() {
  gTempSensorInitialized =aht.begin();
  if (! gTempSensorInitialized) {
    Serial.println("Could not find AHT? Check wiring");
    displayMessage("no temp");
  }
  Serial.println("AHT10 or AHT20 found");
}

void temperatureLoop() {
  if(!gTempSensorInitialized) {
    return;
}
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  gBattery.setTemperature(temp.temperature);
  gBattery.setHumidity(humidity.relative_humidity);

  
}