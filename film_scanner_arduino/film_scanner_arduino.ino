/* Film Scanner created by Travis Bumgarner */

// Libraries & Intialization
/*
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 2
   LCD D5 pin to digital pin 3
   LCD D6 pin to digital pin 4
   LCD D7 pin to digital pin 5
*/
#include <Servo.h>
Servo myservo;

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 2, 3, 4, 5);

int buttonContinuePin = A3;
int buttonUpPin = A2;
int buttonDownPin = A1;
boolean buttonUp;
boolean buttonDown;
boolean buttonContinue;
int calibrateStepperMotorButton = 7;
int numberOfPhotosToScan;

int stmMovePin = 9;
int stmDirPin = 8;

/*********************************************************/
/***************  readInputButtons()  ********************/
/*********************************************************/
/*Reads the values of all the physical buttons in circuit*/
void readInputButtons(){
    int sum;
    buttonUp = LOW;
    buttonDown = LOW;
    buttonContinue =  LOW;
    if      (analogRead(buttonContinuePin)<100){buttonContinue = HIGH;}
    else if (analogRead(buttonUpPin)<100){buttonUp = HIGH;}
    else if (analogRead(buttonDownPin)<100){buttonDown = HIGH;}
    Serial.println(buttonContinue);
    Serial.println(analogRead(buttonContinuePin));
}
/*********************************************************/
/******************  wrapString()  ***********************/
/*********************************************************/
/*Wraps strings if they're longer than one line*/
void wrapString(String unwrappedString){
  String line2;
  lcd.clear();
  lcd.print(unwrappedString);
  if (unwrappedString.length()>16){
    line2 = unwrappedString.substring(16);
    lcd.setCursor(0,1);
    lcd.print(line2);
  }
}
/*********************************************************/
/*********************  moveStm()  ***********************/
/*********************************************************/
/*Moves stepper motor in a specific direction with setDirection*/
/*Moves stepper motor for a setDistance /*                                  */
void moveStm(boolean moveDirection=LOW, int moveDistance= 1350, int moveDelay=10) {
  //stmDirPin High = CCW, stmDirPin Low = CW
  //delay(1) - Very Fast, Delay(5) - OK, Delay(10) - Perfect
  //At 12V DC, 1350 Microsteps = 37.5mm of linear distance with 
  //current 3d printed part which is distance between center of 2x 36mm 
  //negative images.
  digitalWrite(stmDirPin, moveDirection);
  for(int moveCounter = 1; moveCounter <= moveDistance; moveCounter++){
    digitalWrite(stmMovePin, HIGH);
    delay(moveDelay);
    digitalWrite(stmMovePin, LOW);
    delay(moveDelay);
    Serial.println(moveCounter);
  }
  delay(7500); //Modify this value to have enough time to take a picture.
}
/*********************************************************/
/************  calibrateStepperMotor()  ******************/
/*********************************************************/
/*Simple button that moves stepper motor to where you want it to go*/
void calibrateStepperMotor() {
  digitalWrite(stmMovePin, HIGH);
  delay(5);
  digitalWrite(stmMovePin, LOW);
  delay(5);
}
/*********************************************************/
/***************  numberOfPhotos()  **********************/
/*********************************************************/
/*Declare number of negatives to scan */
/*void numberOfPhotos(){
    while(continueButtonValue != HIGH){
        readInputButtons();
        lcd.setCursor(0,0);
        lcd.print("Negatives QTY:");
        if(buttonSetValue == HIGH){ //* Declare buttonSetValue
            return;
        }
        if(buttonUp == HIGH) {numberOfPhotosToScan++;}
        else if (buttonDown == HIGH)    {numberOfPhotosToScan--;}    
        }
        lcd.setCursor(0,1);
        lcd.print(numberOfPhotosToScan); //Change to variables
        delay(50);
    }
}
*/
/*********************************************************/
/***************  takePicture()  *************************/
/*********************************************************/
/* Script for using RX100 to take a picture */
void takePicture(){
  int pos = 0; 
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
/*********************************************************/
/************  systemStartupChecks()  ********************/
/*********************************************************/

void systemStartupChecks(){
  int currentCheck = 0;
  int totalChecks = 3; //Total checks is the number of steps to run through
  while (currentCheck < totalChecks){
    readInputButtons();
    if (currentCheck == 0){
      wrapString("Load film and tighten");
      while(buttonContinue != HIGH){
        readInputButtons();
        if (digitalRead(calibrateStepperMotorButton) == 1) {
          calibrateStepperMotor();
          Serial.println(digitalRead(calibrateStepperMotorButton));
        }
        }
      currentCheck++;
      delay(250);
      }
    else if (currentCheck == 1){
      wrapString("Check Camera: Raw, Man, Level");
      while(buttonContinue != HIGH){readInputButtons();}
      currentCheck++;
      delay(250);
    }
    else if (currentCheck == 2){
      lcd.clear();
      lcd.print("Taking test shot");
      for(int seconds = 5; seconds >=0; seconds--){
        lcd.setCursor(0,1);
        lcd.print(seconds);
        delay(1000);
        if (seconds ==2){
          takePicture();        
        }
      }

      while(buttonContinue != HIGH){readInputButtons();}
      currentCheck++;
      delay(250);
    }
  }
}

/*********************************************************/
/******************  setup()  ****************************/
/*********************************************************/

void setup() {
  lcd.begin(16, 2);
  myservo.attach(6);
  pinMode(buttonContinue, INPUT);
  pinMode(calibrateStepperMotorButton, INPUT);
  Serial.begin(9600);
  pinMode(stmDirPin, OUTPUT);
  pinMode(stmMovePin, OUTPUT);
  digitalWrite(stmDirPin, LOW);
  digitalWrite(stmMovePin, LOW);
  Serial.begin(9600);
}

/*********************************************************/
/******************  loop()  *****************************/
/*********************************************************/
void loop() {
  systemStartupChecks();
  delay(5000);
  /*while (continueButtonValue != 1) {
    if (digitalRead(calibrateStepperMotorButton) == HIGH) {
      calibrateStepperMotor();
    }
  }*/
}
