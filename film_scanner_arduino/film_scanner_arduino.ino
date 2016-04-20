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
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 2, 3, 4, 5);

int continueButton = 13;
int calibrateStepperMotorButton = 7;
int continueButtonValue;
int numberOfPhotosToScan;

int stmMovePin = 9;
int stmDirPin = 8;

/*********************************************************/
/***************  lcdClearLine()  ************************/
/*********************************************************/

void lcdClearLine()
{
  lcd.setCursor(0, 1);
  lcd.print("                                ");
  lcd.setCursor(0, 0);
  lcd.print("                                ");
  lcd.setCursor(0, 0);
}

/*********************************************************/
/***************  readInputButtons()  ********************/
/*********************************************************/
/*Reads the values of all the physical buttons in circuit*/
void readInputButtons() {
  continueButtonValue = digitalRead(continueButton);
}

/*********************************************************/
/************  systemStartupChecks()  ********************/
/*********************************************************/
/*Checks that each part of the system is running before  */
/* Continuing running                                    */
void systemStartupChecks(String stringToCheck, String stringToCheckLine2 = "") {
  while (continueButtonValue != 1) {
    readInputButtons();
    lcdClearLine();
    lcd.print(stringToCheck);
    lcd.setCursor(0, 1);
    lcd.print(stringToCheckLine2);
  }
  delay(500);
}

/*********************************************************/
/***********************  stm()  *************************/
/*********************************************************/
/*Moves stepper motor in a specific direction with setDirection*/
/*Moves stepper motor for a setDistance /*                                  */
void stm(boolean setDirection, int setDistance) {
  Serial.println(String(digitalRead(stmDirPin)) + " " + String(setDistance));
  digitalWrite(stmDirPin, setDirection);
  for (int i = 0; i < setDistance; i++) {
    digitalWrite(stmMovePin, HIGH);
    delay(1);
    digitalWrite(stmMovePin, LOW);
    delay(1);
  }
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
void numberOfPhotos(){
    while(continueButtonValue != HIGH){
        readInputButtons();
        lcd.setCursor(0,0);
        lcd.print("Negatives QTY:");
        if(buttonSetValue == HIGH){ /******************************** Declare buttonSetValue *************/
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



/*********************************************************/
/******************  setup()  ****************************/
/*********************************************************/

void setup() {
  lcd.begin(16, 2);
  pinMode(continueButton, INPUT);
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
  systemStartupChecks("Load Film and  ", "Tighten");
  readInputButtons();
  systemStartupChecks("Check Camera:RAW", "M Mode, Level it");
  readInputButtons();
  systemStartupChecks("Adj Stepper", "Take Test Shot");
  readInputButtons();
  while (continueButtonValue != 1) {
    if (digitalRead(calibrateStepperMotorButton) == HIGH) {
      calibrateStepperMotor();
    }
  }


}
