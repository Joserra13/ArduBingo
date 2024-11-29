/**********************************************
*                   ARDUBINGO
**********************************************/

//Libraries----------------------------------
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <LiquidCrystal.h>

//Defines------------------------------------
#define N 91
#define BTN1 2
#define BTN2 3

//Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

//Global Variables
int list[N];
int empties = N - 1;
int ball;
int getNumberFlag = false;
int seeListFlag = false;

void setup() {
  //Pin config
  pinMode(BTN1, INPUT_PULLUP);  //Get next number
  pinMode(BTN2, INPUT_PULLUP);  //See full list of selected number until now OR exit the list view

  //Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  //Print a message to the LCD.
  lcd.print("Welcome  to  the");
  lcd.setCursor(5, 1);
  lcd.print("BINGO!");

  // Interrupt on falling edge
  attachInterrupt(digitalPinToInterrupt(BTN1), setNumberFlag, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN2), setListFlag, FALLING);

  // Get the seed from an analog input -> Random each time
  randomSeed(analogRead(0));

  //Initialize the Serial port
  Serial.begin(9600);
  delay(1000);

  //item 0 from list -- unused
  list[0] = 999;
}

void setNumberFlag() {
  getNumberFlag = true;
}

void setListFlag() {
  seeListFlag = true;
}

void initiateScreen() {
  //Now, let's print the value in the lcd
  lcd.clear();
  // set the cursor to column 0, line 0
  lcd.setCursor(0, 0);
  lcd.print("Numero: ");
  lcd.print(list[ball]);
  lcd.setCursor(0, 1);
  lcd.print("1:Next    2:List");
}

void getNumber() {
  /*
    * Generate a random number between 1 and 90.
    * If the number has already been drawn (already appears in the list), generate a new number.
    */

  do {
    ball = random(1, N);  // Generates a number from 1 to 90
  } while (list[ball] != 0);

  Serial.print(ball);
  Serial.println();

  empties--;
  list[ball] = ball;

  initiateScreen();
}

void seeList() {

  String message;
  int ii = 0;
  int strLength;
  String toShow;

  for(int i = 1; i < N; i++){
    if(list[i] != 0){
      message += String(list[i]) + " ";
    }
  }

  strLength = message.length();
  lcd.clear();
  lcd.home();

  while(1){

    //Credits for this workaround to https://github.com/build2master/arduino-lcd-scroll-long-text/tree/master
    lcd.home();

    // Get 16 characters so that we can display on the LCD
    toShow = message.substring(ii,ii+16);
    
    lcd.print(toShow);

    ii = ii + 2;

    delay(500);

    // We have to reset ii after there is less text displayed.
    if(ii>(strLength-16)) {
      ii = 0;
      break;
    }
  }

  delay(1000);

  initiateScreen();
}

void loop() {

  for (int i = 1; i < N; i++) {
    list[i] = 0;
  }

  while (empties > 0) {

    if (getNumberFlag) {

      getNumber();
      getNumberFlag = false;
    }
    if (seeListFlag) {

      seeList();
      seeListFlag = false;
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("      GAME       ");
  lcd.setCursor(0, 1);
  lcd.print("      OVER       ");

  while (digitalRead(BTN1) == HIGH) {
    lcd.blink();
  }

  delay(5000);
}
