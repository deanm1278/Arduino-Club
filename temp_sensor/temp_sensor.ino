// DERIVED FROM: Example testing sketch for various DHT humidity/temperature sensors
//This reads a temperature from a DHT22 temp sensor with an arduino uno

#include "DHT.h"

#define DHTPIN 3     // what digital pin we're connected to

#define THRESH 70

//we are using this sensor type (PID385)
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

//this function runs once
void setup() {
  //begin communication with the computer
  Serial.begin(9600);
  dht.begin();
}

//this runs over and over
void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  //TODO: do something useful with the data
  if(f < THRESH){
    //email garrett
  }
}
