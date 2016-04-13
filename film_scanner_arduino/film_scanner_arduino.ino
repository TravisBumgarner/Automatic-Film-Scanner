/* Film Scanner created by Travis Bumgarner */

// Libraries & Intialization
/*
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 2
 * LCD D5 pin to digital pin 3
 * LCD D6 pin to digital pin 4
 * LCD D7 pin to digital pin 5
 */
 #include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 2,3,4,5);
int continueButton = 13;
int continueButtonValue;

/*********************************************************/
/***************  lcdClearLine()  ************************/
/*********************************************************/

void lcdClearLine()
{
    lcd.setCursor(0,1);
    lcd.print("                                ");
    lcd.setCursor(0,0);
    lcd.print("                                ");
    lcd.setCursor(0,0);
} 

/*********************************************************/
/***************  readInputButtons()  ********************/
/*********************************************************/
/*Reads the values of all the physical buttons in circuit*/
void readInputButtons(){
    continueButtonValue = digitalRead(continueButton);
}

/*********************************************************/
/************  systemStartupChecks()  ********************/
/*********************************************************/
/*Checks that each part of the system is running before  */
/* Continuing running                                    */
void systemStartupChecks(String stringToCheck, String stringToCheckLine2 = ""){
  while(continueButtonValue != 1){
    readInputButtons(); 
    lcdClearLine();
    lcd.print(stringToCheck);
    lcd.setCursor(0,1);
    lcd.print(stringToCheckLine2);
  }
  delay(500);
}

/*********************************************************/
/******************  setup()  ****************************/
/*********************************************************/

void setup() {
  lcd.begin(16, 2);
  pinMode(continueButton,INPUT);
  Serial.begin(9600);

}

/*********************************************************/
/******************  loop()  *****************************/
/*********************************************************/
void loop() {
  systemStartupChecks("Load Film and  ","Tighten");
  readInputButtons();
  systemStartupChecks("Check Camera:RAW","M Mode, Level it");
  readInputButtons();
  systemStartupChecks("Take Test Shot");
  readInputButtons();
}
