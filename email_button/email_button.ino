/***************************************************
 * DERIVED FROM:
  Adafruit IO Trigger Example
  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino
  Works great with Adafruit's Huzzah ESP board:
  ----> https://www.adafruit.com/product/2471
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Todd Treece for Adafruit Industries.
  MIT license, all text above must be included in any redistribution


  This sketch will read a DHT22 temp sensor and a value to adafruit.io if the temperature goes
  below a set threshold
 ****************************************************/
#include <ESP8266WiFi.h>

#include <EEPROM.h>

#include "Adafruit_IO_Client.h"

#include "DHT.h"

// function prototypes
void button_on();
void wifi_intit();

#define DHTPIN 0     // what digital pin temp controller is connected to
#define THRESH 75

//we are using this sensor type (PID385)
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// wifi credentials
// #define WLAN_SSID       "Network"
// #define WLAN_PASS       "Password"
#define WLAN_SSID       "........."
#define WLAN_PASS       "........."

// configure Adafruit IO access
#define AIO_KEY         "........."

// create an Adafruit IO client instance
WiFiClient client;
Adafruit_IO_Client aio = Adafruit_IO_Client(client, AIO_KEY);

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("HUZZAH Temp sensor");
  dht.begin();
  pinMode(BUTTON, INPUT);
}

void loop() {
    // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  //if f(temp) is under 75 degrees then tap wifi to send to IO
  // if button isn't pressed, we don't need to send anything
  if (f < THRESH) {
    button_on();
  }
  //wait a second before the next read
  delay(1000);
}


// connect to wifi network. .
void wifi_init() {

  // wifi init
  Serial.println("Hittin the WEBB");
  WiFi.begin(WLAN_SSID, WLAN_PASS);

  // wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    Serial.write(';)');
    delay(500);
  }

  // AIO init
  Serial.println("Connecting to Adafruit.io");
  aio.begin();

}

void button_on() {

  // turn on wifi if we aren't connected
  if (WiFi.status() != WL_CONNECTED) {
    wifi_init();
  }

  // grab the door feed
  Adafruit_IO_Feed button = aio.getFeed("button");

  Serial.println("Sending button value to Adafruit.io");
  // send button value to AIO
  button.send(1);
}
