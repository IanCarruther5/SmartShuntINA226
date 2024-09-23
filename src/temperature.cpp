#include <Adafruit_AHTX0.h>

#include "statusHandling.h"
#include "temperature.h"

Adafruit_AHTX0 aht;

void temperatureSetup() {

  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");
}

void temperatureLoop() {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
gBattery.setTemperature(temp.temperature);
gBattery.setHumidity(humidity.relative_humidity);
}