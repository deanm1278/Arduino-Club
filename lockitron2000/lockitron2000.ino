    /****************************************************************
Lockitron_Demo.ino
Shawn Hymel @ SparkFun Electronics
July 7, 2015

Controls the lock and unlock functions of a Lockitron motor with 
2 buttons. Note that on boot, the Arduino will attempt to rotate 
the motor to a "home" position. This might be bad if the 
Lockitron in on a door.

This code is beerware; if you see me (or any other SparkFun 
employee) at the local, and you've found our code helpful, please
buy us a round!

Distributed as-is; no warranty is given.
****************************************************************/

// Debug mode. 0 = Motor functional. 1 = Serial output only.
#define DEBUG 1

const int knockSensor = A0; // the piezo is connected to analog pin 0
const int threshold = 500;  // threshold value to decide when the detected sound is a knock or not

int sensorReading = 0;      // variable to store the value read from the sensor pin

unsigned long k0, k1, k2, ts;
bool allKnocksReceived = false;


// Constants
const uint8_t LOCK_OPEN = 0;
const uint8_t LOCK_CLOSED = 1;
const uint8_t MSG_LOCK = 0x10;
const uint8_t MSG_UNLOCK = 0x11;
const uint8_t MSG_STATE_REQ = 0x12;

// Motor speed and direction definitions
const uint8_t MOTOR_SPEED = 200;
const uint8_t MOTOR_CW = 0;
const uint8_t MOTOR_CCW = 1;

// Pin definitions
/* NOT USED
const uint8_t BTN_UNLOCK_PIN = 2;
const uint8_t BTN_LOCK_PIN = 3;
*/
const uint8_t SW_1A_PIN = 8; //green wire
const uint8_t SW_1B_PIN = 9; //white wire
const uint8_t SW_2A_PIN = 10; //blue wire
const uint8_t SW_2B_PIN = 11; //black wire
const uint8_t AIN1_PIN = 7;
const uint8_t AIN2_PIN = 6;
const uint8_t PWMA_PIN = 5;

/* NOT USED
// Button states
uint8_t btn_lock;
uint8_t btn_unlock;
uint8_t prev_btn_lock;
uint8_t prev_btn_unlock;
*/

// Switch state variables
uint8_t sw_1a;
uint8_t sw_1b;
uint8_t sw_2a;
uint8_t sw_2b;

// Lock state
uint8_t lock_state;
void resetLock()
{
  // Move motor to reset its position
  moveMotor(MOTOR_SPEED, MOTOR_CCW);
  do
  {
    sw_1a = digitalRead(SW_1A_PIN);
    sw_1b = digitalRead(SW_1B_PIN);
    sw_2a = digitalRead(SW_2A_PIN);
    sw_2b = digitalRead(SW_2B_PIN);
  }
  while ( !((sw_2a == 1) && (sw_2b == 1)));
  stopMotor();
}

void lock()
{
  // Move motor to lock the deadbolt
  moveMotor(MOTOR_SPEED, MOTOR_CW);
  do
  {
    sw_1a = digitalRead(SW_1A_PIN);
    sw_1b = digitalRead(SW_1B_PIN);
    sw_2a = digitalRead(SW_2A_PIN);
    sw_2b = digitalRead(SW_2B_PIN);
  }
  while ( !((sw_1a == 0) && (sw_1b == 1) && 
            (sw_2a == 0) && (sw_2b == 1)) );
  stopMotor();
  delay(100);

  // Move motor back to starting position
  moveMotor(MOTOR_SPEED, MOTOR_CCW);
  do
  {
    sw_1a = digitalRead(SW_1A_PIN);
    sw_1b = digitalRead(SW_1B_PIN);
    sw_2a = digitalRead(SW_2A_PIN);
    sw_2b = digitalRead(SW_2B_PIN);
  }
  while ( !((sw_2a == 1) && (sw_2b == 1)) );
  stopMotor();
  lock_state = LOCK_OPEN;
}

void unlock()
{
  // Move motor to lock the deadbolt
  moveMotor(MOTOR_SPEED, MOTOR_CCW);
  do
  {
    sw_1a = digitalRead(SW_1A_PIN);
    sw_1b = digitalRead(SW_1B_PIN);
    sw_2a = digitalRead(SW_2A_PIN);
    sw_2b = digitalRead(SW_2B_PIN);
  }
  while ( !((sw_1a == 1) && (sw_1b == 0) && 
            (sw_2a == 1) && (sw_2b == 0) ));
  stopMotor();
  delay(100);

  // Move motor back to starting position
  moveMotor(MOTOR_SPEED, MOTOR_CW);
  do
  {
    sw_1a = digitalRead(SW_1A_PIN);
    sw_1b = digitalRead(SW_1B_PIN);
    sw_2a = digitalRead(SW_2A_PIN);
    sw_2b = digitalRead(SW_2B_PIN);
  }
  while ( !((sw_2a == 1) && (sw_2b == 1)) );
  stopMotor();
  lock_state = LOCK_CLOSED;
}

void moveMotor(uint8_t spd, uint8_t dir)
{
  boolean ain1;
  boolean ain2;

  // Define direction pins
  if ( dir )
  {
    ain1 = HIGH;
    ain2 = LOW;
  } 
  else
  {
    ain1 = LOW;
    ain2 = HIGH;
  }

  // Set motor to GO!
  digitalWrite(AIN1_PIN, ain1);
  digitalWrite(AIN2_PIN, ain2);
  analogWrite(PWMA_PIN, spd);
}

void stopMotor()
{
  analogWrite(PWMA_PIN, 0);
}

void setup() {

#if DEBUG
  Serial.begin(9600);
#else
  // Set up motor pins
  pinMode(AIN1_PIN, OUTPUT);
  pinMode(AIN2_PIN, OUTPUT);
  pinMode(PWMA_PIN, OUTPUT);
#endif

  // Set up switch and button pins
  /* NOT USED
  pinMode(BTN_UNLOCK_PIN, INPUT_PULLUP);
  pinMode(BTN_LOCK_PIN, INPUT_PULLUP);
  */
  pinMode(SW_1A_PIN, INPUT_PULLUP);
  pinMode(SW_1B_PIN, INPUT_PULLUP);
  pinMode(SW_2A_PIN, INPUT_PULLUP);
  pinMode(SW_2B_PIN, INPUT_PULLUP);
  pinMode(knockSensor, INPUT);

  
  // Reset the lock to unlocked position
  resetLock();
  lock_state = LOCK_OPEN;

  /* NOT USED
  prev_btn_lock = 1;
  prev_btn_unlock = 1;
  */
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
          unlock();
         delay(8000);
         lock();
            
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




