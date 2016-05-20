/* We modified this example from the one on the adafruit learning system.
 *  This code will initialize an lcd screen, and set the backlight to blue
 *  
 *  Then it will flash the string 'test 1'
 *  
 *  You can modify this example to print whatever you like on the LCD screen
 */

// include the library code:
#include <LiquidCrystal.h>
#include <Wire.h>

//these digital pins will be used for the backlights
#define REDLITE 3
#define GREENLITE 5
#define BLUELITE 6
 
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
 
void setup() {
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);

  pinMode(REDLITE, OUTPUT);
  pinMode(GREENLITE, OUTPUT);
  pinMode(BLUELITE, OUTPUT);

  //writing one of these low turns on the led backlight of the chosen color
  digitalWrite(BLUELITE, LOW);
  digitalWrite(GREENLITE, HIGH);
  digitalWrite(REDLITE, HIGH);
}
 
 
void loop() {
   // Print a message to the LCD.

  //setCursor takes arguments: column, row
  lcd.setCursor(0,0); //col 0, row 0
  
  lcd.clear(); //remove everything from the display
  delay(1000);
  lcd.print("test 1");
  delay(1000);
}
 
