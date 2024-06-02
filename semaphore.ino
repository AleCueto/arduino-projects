#include <IRremote.h>

#define RECV_PIN 10

IRrecv irrecv(RECV_PIN);
decode_results results;

// PEASANT PINS
#define peasant_red 5
#define peasant_green 4

// CAR PINS
#define car_green 11
#define car_yellow 12
#define car_red 13

// crossState can be:
// 0 --> just car can cross
// 1 --> car and peasant can cross
// 2 --> just peasant can cross
int crossState = 0;
bool on = true;

unsigned long previousMillisLeds = 0;
const long interval[] = {5000, 1000, 5000}; // Intervals for each crossState in milliseconds

unsigned long previousMillisIr = 0;
const long intervalIr = 100;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();

  bool on = true;
  crossState = 0;
  updateLeds();

  // Set these pins to OUTPUT mode
  pinMode(peasant_red, OUTPUT);
  pinMode(peasant_green, OUTPUT);
  pinMode(car_green, OUTPUT);
  pinMode(car_yellow, OUTPUT);
  pinMode(car_red, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  // If the system is on, update LEDs based on the crossState
  if (on) {
    if (currentMillis - previousMillisLeds >= interval[crossState]) {
      previousMillisLeds = currentMillis; // Update the last update time

      // Update the crossState
      crossState = (crossState + 1) % 3;
      updateLeds();
    }
  } else {
    poweroff();
  }

  if(currentMillis - previousMillisIr >= intervalIr){
    previousMillisIr = currentMillis;
    
  // Check for IR signals
    if (irrecv.decode(&results)) {
      Serial.println(results.value, HEX);
  
      if (results.value == 0xFFA857 || results.value == 0xFFA25D || results.value == 0xE318261B) {
        on = !on; // Toggle the on/off state
        
        if(on){
          Serial.println("encendido");
          crossState = 0;
          updateLeds();
        } else{
          Serial.println("apagado");
        }
      }
      irrecv.resume(); // Receive the next value
     }
   }
}

void poweroff() {
  digitalWrite(peasant_red, LOW);
  digitalWrite(peasant_green, LOW);
  digitalWrite(car_red, LOW);
  digitalWrite(car_green, LOW);
  digitalWrite(car_yellow, LOW);
}

void updateLeds() {
  switch (crossState) {
    // Just car can cross
    case 0:
      digitalWrite(peasant_red, HIGH);
      digitalWrite(peasant_green, LOW);
      digitalWrite(car_red, LOW);
      digitalWrite(car_green, HIGH);
      digitalWrite(car_yellow, LOW);
      break;

    // Car and peasant can cross
    case 1:
      digitalWrite(peasant_red, LOW);
      digitalWrite(peasant_green, HIGH);
      digitalWrite(car_red, LOW);
      digitalWrite(car_green, LOW);
      digitalWrite(car_yellow, HIGH);
      break;

    // Just peasant can cross
    case 2:
      digitalWrite(peasant_red, LOW);
      digitalWrite(peasant_green, HIGH);
      digitalWrite(car_red, HIGH);
      digitalWrite(car_green, LOW);
      digitalWrite(car_yellow, LOW);
      break;
  }
}
