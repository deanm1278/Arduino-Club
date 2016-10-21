/* Knock Sensor

   This sketch reads a piezo element to detect a knocking sound.
   It reads an analog pin and compares the result to a set threshold.
   If the result is greater than the threshold, it writes
   "knock" to the serial port, and toggles the LED on pin 13.

   The circuit:
	* + connection of the piezo attached to analog in 0
	* - connection of the piezo attached to ground
	* 1-megohm resistor attached from analog in 0 to ground

   http://www.arduino.cc/en/Tutorial/Knock

   created 25 Mar 2007
   by David Cuartielles <http://www.0j0.org>
   modified 30 Aug 2011
   by Tom Igoe

   This example code is in the public domain.

 */

// these constants won't change:
const int ledPin = 13;      // led connected to digital pin 13
const int knockSensor = A0; // the piezo is connected to analog pin 0
const int threshold = 500;  // threshold value to decide when the detected sound is a knock or not


// these variables will change:
int sensorReading = 0;      // variable to store the value read from the sensor pin
bool ledState = LOW;         // variable used to store the last LED status, to toggle the light

unsigned long k0, k1, k2, ts;
bool allKnocksReceived = false;

void setup() {
  pinMode(ledPin, OUTPUT); // declare the ledPin as as OUTPUT
  Serial.begin(9600);       // use the serial port
}

void reset(){
    k0 = 0;
    k1 = 0;
    k2 = 0;
    allKnocksReceived = false;
}

void loop() {
  unsigned long tNow = millis();
  if(tNow - ts > 3000){
    if(allKnocksReceived){
        //check the pattern
        int diff0 = k1 - k0;
        int diff1 = k2 - k1;
    
        Serial.print(diff0);
        Serial.print(", ");
        Serial.println(diff1);
        Serial.print("ratio is ");
        Serial.print(round((float)diff0 / (float)diff1));
        
        if(round( (float)diff0 / (float)diff1 ) == 2){
          //there is a match
          Serial.println("There is a match!");
        }
    }
    reset();
  }
  
  // read the sensor and store it in the variable sensorReading:
  sensorReading = analogRead(knockSensor);
  
  // if the sensor reading is greater than the threshold:
  if (sensorReading >= threshold) {
    if(k0 == 0){
      ts = tNow; //start the pattern now
      k0 = ts; //the first knock time is the same as the start time
      Serial.print("first knock at ");
      Serial.println(k0);
    }
    else if(k1 == 0){
      k1 = tNow;
      Serial.print("second knock at ");
      Serial.println(k1);
    }
    else if(k2 == 0){
      k2 = tNow;
      Serial.print("thrid knock at ");
      Serial.println(k2);
    }
    else{
      //too many knocks
      Serial.println("too many knocks!");
      reset();
    }
    delay(100);
  }

  if(k0 != 0 && k1 != 0 && k2 != 0){
    allKnocksReceived = true;
  }
  
  delay(5);
}

