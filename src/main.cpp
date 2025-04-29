#include "Arduino.h"
#include "LiquidCrystal.h"
#include "Wire.h"

// Shift Register Pins for LEDs
const int latchPin = 3;
const int clockPin = 4;
const int dataPin = 2;

// Middle LED pin
const int ledPin = 7;

// Arcade Button Pins
//const int leftButton = 5;
//const int rightButton = 6;

int buttonStateLeft = 0;
int buttonStateRight = 0;

// Pins to transistor collectors
const int leftSolenoid = 5;
const int rightSolenoid = 6;

// Define pin numbers for piezo and photreisstor
const int popBumper1 = A0; // Pin for 1st piezo
const int popBumper2 = A1; // Pin for 2nd piezo
const int popBumper3 = A3; // Pin for 3rd piezo
const int drainSensor = A4; // Pin for photoresistor

// Define pins for LCD screen
// (RS,Enable,D4,D5,D6,D7)

LiquidCrystal lcd(8,9,10,11,12,13);

// Declare and initialize variables.
int score = 0; // to keep track of score
int totalscore = 0;
int ball = 4; // Track how many balls available to play
byte leds = 0;
bool executed1 = false;
bool executed2 = false;

void setup()
{
pinMode(drainSensor,INPUT);
lcd.begin(20, 4); // Setup the number of the columns and rows of the lcd
lcd.print(“Have Fun”);

// Shift Register 1 Setup
pinMode(latchPin, OUTPUT);
pinMode(dataPin, OUTPUT);
pinMode(clockPin, OUTPUT);

// Button and Solenoid Setup
//pinMode(leftButton, INPUT);
//pinMode(rightButton, INPUT);
pinMode(leftSolenoid, OUTPUT);
pinMode(rightSolenoid, OUTPUT);

// Middle LED
pinMode(ledPin,OUTPUT);

}

void updateShiftRegister()
{
digitalWrite(latchPin, LOW);
shiftOut(dataPin, clockPin, LSBFIRST, leds); //LSBFIRST for clockwise led pattern
digitalWrite(latchPin, HIGH);
}

void loop()
{
/* Buttons became a non use item after solenoids were removed from the flipper assembly
the following includes the button states to detect whether the buttons were pressed
//Detect whether buttons are pressed and activate solenoid
buttonStateLeft = digitalRead(leftButton);
buttonStateRight = digitalRead(rightButton);
*/

//activates solenoid only once if score>=400
if (score >=400&& executed1 ==false)
{
digitalWrite(leftSolenoid, LOW);
executed1 = true; //set true so solenoid doesnt keep activating
}
else
{
digitalWrite(leftSolenoid, HIGH);
}

//activates solenoid only once if score>=800
if (score >=800&& executed2 ==false)
{
digitalWrite(rightSolenoid,LOW);
executed2 = true;
}
else
{
digitalWrite(rightSolenoid,HIGH);
}

// LEDs patterned so they light up clockwise
leds = 0;
updateShiftRegister();
delay(500);
for (int i =0; i <8; i++) { bitSet(leds, i); updateShiftRegister(); delay(250); } // Middle LED will blink digitalWrite(ledPin, HIGH); delay(500); digitalWrite(ledPin, LOW); delay(500); // Points score = millis()/100; if (analogRead(popBumper1) >= 100)
{
score = score + 100;
lcd.setCursor(0, 3);
lcd.print(“Bonus +100″);
delay(500);
lcd.setCursor(0, 3);
lcd.print(” “);
}

if (analogRead(popBumper2) >= 100)
{
score = score + 100;
lcd.setCursor(0, 3);
lcd.print(“Bonus +150″);
delay(500);
lcd.setCursor(0, 3);
lcd.print(” “);
}

if (analogRead(popBumper3) >= 100)
{
score = score + 200;
lcd.setCursor(0, 3);
lcd.print(“Bonus +200″);
delay(500);
lcd.setCursor(0, 3);
lcd.print(” “);
}

// Ball Drain
if (analogRead(drainSensor) < 50)
{
ball = ball – 1;
delay(2000);
}

// Print the score on LCD
totalscore = score + millis()/100;
lcd.setCursor(0, 0);
lcd.print(“Your Score: “);
lcd.setCursor(0, 1);
lcd.print(totalscore, DEC);
//lcd.setCursor(0,3);
//lcd.print(“Balls: “,ball );

/*
if (ball == 0)
{
lcd.setCursor(0, 0);
lcd.print(“Final Score: “);
lcd.setCursor(0, 1);
lcd.print(score, DEC);
lcd.setCursor(0,3);
lcd.print(“Try Again!”);
delay(2000);

//Restart Game
score = 0;
lcd.clear();
lcd.setCursor(0,0);
lcd.print(“Have Fun!”);
delay(2000);
}
*/
}
