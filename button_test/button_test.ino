int buttonPin = 3; //button on pin 3

/*CIRCUIT:
 * a button connected to pin 3, other side of the button is connected to ground
 */

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //set button as input, make sure it is at 5V while we are not pressing it
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool is_pressed = digitalRead(buttonPin);

  //print if the button is pressed
  if(!is_pressed){
    Serial.println("pressed!");
  }
  delay(50);
}
