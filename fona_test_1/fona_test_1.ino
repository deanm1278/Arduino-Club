/*
 * This demonstrates the bare minimum needed to send a text message with FONA.
 * All this sketch does it boot up, send a text to the number you specify, and then wait
 * in an infinite loop.
 */

#include "Adafruit_FONA.h"

//define the pins the arduino pins we will use to communicate with the fona
#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4

//we need the software serial to communicate via UART
#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

// Use this for FONA 800 and 808s
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

void setup() {
  while (!Serial);

  //begin serial communication with computer
  Serial.begin(115200);

  //begin serial communication with fona
  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }

  Serial.println(F("fona connected"));
}

void loop() {

  char sendto[] = "YOUR_NUMBER_HERE";
  char message[] = "YOUR_MESSAGE_HERE";

  //try to send message
  if (!fona.sendSMS(sendto, message)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("Sent!"));
  }

  //wait forever in an infinite loop
  while(1);
}
