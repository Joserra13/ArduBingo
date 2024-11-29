/**********************************************
*               THE BINGO GAME
**********************************************/

//Libraries----------------------------------
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <LiquidCrystal.h>

//Defines------------------------------------
#define N 90
#define BTN1 2 
#define BTN2 3

//Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup()
{
  //Pin config
  pinMode(BTN1, INPUT_PULLUP);  //Get next number OR see next one in the list
  pinMode(BTN2, INPUT_PULLUP);  //See full list of selected number until now OR exit the list view
  
  //Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  //Print a message to the LCD.
  lcd.print("Welcome  to  the");
  lcd.setCursor(5, 1);
  lcd.print("BINGO!");
  
  randomSeed(analogRead(0)); // Get the seed from an analog input -> Random each time
}

  int ball;

void loop()
{ 
  int i = 0;
  int list[90];
  int empties = 90;
  
  lcd.clear();
  
  for(i = 0; i <= 90; i++)
  {
    list[i] = 0;   
  } 

  while(empties > 0)
  {               
    /*
    * Generate a random number between 0 and 89.
    * If the number has already been drawn (already appears in the list), generate a new number.
    */

    do
    {
        ball = random(90); // Generates a number from 0 to 89
    } while(list[ball] != 0);

    empties--;
    
    /*
    * Since the random numbers are between 0 and 89 and we want them between 1 and 90, we store ball + 1.
    * If we directly generate a number between 1 and 90 and use it to index the array,
    * the problem arises when the number 90 is generated, and you try to index list[90],
    * as this is a memory location you don't have access to. 
    * Your array has 90 positions: from 0 to 89.
    */

    list[ball] = ball + 1;

    lcd.clear();
    // set the cursor to column 0, line 0
    lcd.setCursor(0,0);
    lcd.print("Numero: ");
    lcd.print(list[ball]); // Changed to list[ball] instead of ball
    lcd.setCursor(0,1);
    lcd.print("1:Next    2:List");
    
    here:
    delay(300);

    while(digitalRead(BTN1) == HIGH)
    {      
      if(digitalRead(BTN2) == LOW)
      {
        // for(i = 1; i < 91; i++) // This can cause problems because you don't have access to list[90]
        for(i = 0; i < 90; i++)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("list[");
          lcd.print(i);
          lcd.print("] = ");
          lcd.print(list[i]);

          if(list[i] != 0)
          {
            delay(500);
          }
          else
          {
            delay(200);
          }

          if(digitalRead(BTN2) == LOW)
          {
            goto here;
          }
        }
      }
    }
  }
  
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("   GAME   OVER   ");

  while(digitalRead(BTN1) == HIGH)
  {
    lcd.blink();
  }

  delay(2000);

}
