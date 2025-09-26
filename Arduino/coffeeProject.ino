// C++ code
//
#include <LiquidCrystal.h>
#include <Servo.h>

// Create LiquidCrystal object lcd
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
// Create Servo object servo
Servo servo;

// Create variable buttonState to keep track of button presses
int buttonState = 0;
// Create variable potpin for potentiometer input
int potpin = 0;
// Create variable val to map potentiometer value to
// 0 - 180 degrees
int val;
// Create variable selection to store String value printed
// on screen
String selection = "";
// Create variable cupSensor to store value of photo resistor
int cupSensor = 0;

// setup() function sets pin 10 and pin 8 to output
// sets up lcd screen, attaches the servo to pin 9 and begins
// Serial object for printing values to console
void setup()
{
  pinMode(10, OUTPUT);
  pinMode(8, OUTPUT);
  lcd.begin(16,2);
  servo.attach(9);
  Serial.begin(9600);
}

// loop() function controls flow of program
// set cupSensor variable to pin A1
// sets variable tempReading to pin A2 and performs operations to convert value to degrees celcius
// stores temperature value in temp variable
// sets val to the value read from potpin variable
// maps analog value from 0, 1023 to 0, 180
// clears lcd screen each loop
// prints String "Boiling..." followed by value of temp
// if temp is greater than 99 degrees then clear lcd and
// print String "Add cup..."
// if cupSensor value is greater than 100,
// clears lcd and calls selectCoffee() function
// calls modifiedDelay() function
// prints val to console
// prints cupSensor to console
// prints temp to console
void loop()
{
  cupSensor = analogRead(A1);
  int tempReading = analogRead(A2);
  float voltage = (float)tempReading * 5.0;
  voltage /= 1024;
  float temp = (voltage - 0.5) * 100;
  val = analogRead(potpin);
  val = map(val, 0, 1023, 180, 0);
  lcd.clear();
  lcd.print("Boiling...");
  lcd.setCursor(0, 1);
  lcd.print(temp);
  lcd.print("c");
  if(temp > 99){
    lcd.clear();
    lcd.print("Add cup...");
    if(cupSensor < 500 ){
      lcd.clear();
      selectCoffee();
    }
  }
  modifiedDelay(500);
  Serial.print("Val = ");
  Serial.println(val);
  Serial.print("Cup Sensor = ");
  Serial.println(cupSensor);
  Serial.print("Temp = ");
  Serial.print(temp);
}

// modified delay function to allow the button state to be tested during the delays in between the traffic lights
// function takes one parameter, time
// start is declared to be the time the delay starts, using the millis() function
// while millis(), the current time, minus start is less than or equal to the time passed in, the checkButton() function is called
void modifiedDelay(unsigned long time){
  unsigned long start = millis();
  
  while(millis() - start <= time){
    checkButton();
  }
}

// function to control the buzzer when the user presses the button
// takes in two parameters, wait and x
// while loop is set to condition of the x parameter.
// within the loop the buzzer is sounded, the function waits depending on the wait parameter,
// the buzzer is turned off, and the function waits the duration of x again.
// x is then decremented by 1 to ensure the loop will only run x times.
void buzzer(int wait, int x){
  while(x){
    tone(10, 1000);
    delay(wait);
    noTone(10);
    delay(wait);
    x -= 1;
  }
}

// flashes LED light
// takes one parameter x to control while loop
// light flashes x times
void flashLED(int x){
  while(x){
    digitalWrite(8, HIGH);
    delay(200);
    digitalWrite(8, LOW);
    delay(200);
    x -= 1;
  }  
}

// checkButton() function to check if button has been pressed
// if button has been pressed, appropriate function is called
// based on value of String selection
void checkButton(){
  buttonState = digitalRead(13);
  if(buttonState == HIGH && selection == "Americano"){
    americano();
  }
  
  if(buttonState == HIGH && selection == "Cappuccino"){
    cappuccino();
  }
  
  if(buttonState == HIGH && selection == "Espresso"){
    espresso();
  }
}

// function americano() to perform actions if user has selected americano
// calls buzzer() function, call servo.write() to move servo 180 degrees,
// clears lcd screen, prints value of selection to lcd screen ie "Americano"
// move cursor to second line of screen and print String "Preparing..."
// call flashLED(), (this function also acts as a delay)
// call analogWrite() on the motor connected to pin 6 (turns on the motor)
// set servo back to 0
// flashLED() to flash led 12 times, analogWrite pin 6 back to 0 (turns motor off)
// print String "Complete." on lcd screen
// sound buzzer 3 times
void americano(){
  buzzer(500, 1);
  servo.write(180);
  lcd.clear();
  lcd.print(selection);
  lcd.setCursor(0, 1);
  lcd.print("Preparing...");
  flashLED(5);
  analogWrite(6, 1023);
  servo.write(0);
  flashLED(12);
  analogWrite(6, 0);
  lcd.clear();
  lcd.print("Complete.");
  buzzer(100, 3);
}

// function cappuccino() to perform actions if user has selected cappuccino
// works in the same fashion as previous americano() function but after running motor on pin 6,
// it runs the motor on pin 7 to simulate adding milk to the cappuccino
// prints String "Complete." and sounds buzzer 3 times when finished
void cappuccino(){
  buzzer(500, 1);
  servo.write(180);
  lcd.clear();
  lcd.print(selection);
  lcd.setCursor(0, 1);
  lcd.print("Preparing...");
  flashLED(5);
  analogWrite(6, 1023);
  servo.write(0);
  flashLED(5);
  analogWrite(6, 0);
  analogWrite(7, 1023);
  flashLED(8);
  analogWrite(7, 0);
  lcd.clear();
  lcd.print("Complete.");
  buzzer(100, 3);
}

// function espresso to perform actions if user has selected espresso
// works in the same fashion as americano() function but runs for less time
// simulating less liquid in espresso vs americano
void espresso(){
  buzzer(500, 1);
  servo.write(180);
  lcd.clear();
  lcd.print(selection);
  lcd.setCursor(0, 1);
  lcd.print("Preparing...");
  flashLED(5);
  analogWrite(6, 1023);
  servo.write(0);
  flashLED(5);
  analogWrite(6, 0);
  lcd.clear();
  lcd.print("Complete.");
  buzzer(100, 3);
}

// function selectCoffee() to set value of String selection based on val (value of potentiometer)
// prints selection to lcd screen
// prints String "Press button..." on second line of lcd screen
void selectCoffee(){
  if(val <= 60){
    selection = "Americano";
  }
  
  if(val > 60 && val <= 120){
    selection = "Cappuccino";
  }
  
  if(val > 120 && val <= 180){
    selection = "Espresso";
  }
  
  lcd.print(selection);
  lcd.setCursor(0, 1);
  lcd.print("Press button...");
}