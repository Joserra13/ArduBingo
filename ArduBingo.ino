/**********************************************
*               THE BINGO GAME
**********************************************/

//Librerías----------------------------------
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
  pinMode(BTN1, INPUT_PULLUP);  //Saca bola o avanza en la lista
  pinMode(BTN2, INPUT_PULLUP);  //Ver lista completa de bolas que han salido o salir de ahí
  
  //Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  //Print a message to the LCD.
  lcd.print("Bienvenido    al");
  lcd.setCursor(5, 1);
  lcd.print("BINGO!");
  
  //srand(time(NULL));
  randomSeed(analogRead(0)); // Con un pin que cuadre
}

  int bola;

void loop()
{ 
  int i = 0;
  int lista[90];  // acceso a lista[0] hasta lista[89]
  int vacios = 90;
  
  lcd.clear();
  
  for(i = 0; i <= 90; i++)
  {
    lista[i] = 0;   
  } 

  while(vacios > 0)
  {               
    /*
     * Genera un numero aleatorio entre 0 y 89.
     * Si el numero ya ha salido (ya aparece en la lista), genera un nuevo numero
     */
    do
    {
        bola = random(90); // Genera un numero desde 0 a 89
    } while(lista[bola] != 0);

    vacios--;
    
    /*
    Como los numeros aleatorios son entre 0 y 89 y los queremos entre 1 y 90, guardamos bola + 1
    Si generamos directamente un numero entre 1 y 90 y lo usamos para indexar el vector,
    el problema es cuando salga el numero 90 y trates de indexar el lista[90], porque que es una posicion
    de memoria a la que no tienes acceso, tu vector tiene 90 posiciones: del 0 al 89.
    */
    lista[bola] = bola + 1;

    lcd.clear();
    // set the cursor to column 0, line 0
    lcd.setCursor(0,0);
    lcd.print("Numero: ");
    lcd.print(lista[bola]); // Cambiado a lista[bola] en lugar de bola
    lcd.setCursor(0,1);
    lcd.print("1:Next    2:List");
    
    here:
    delay(300);

    while(digitalRead(BTN1) == HIGH)
    {      
      if(digitalRead(BTN2) == LOW)
      {
        // for(i = 1; i < 91; i++) // Esto puede dar problemas, porque no tienes acceso a lista[90]
        for(i = 0; i < 90; i++)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Lista[");
          lcd.print(i);
          lcd.print("] = ");
          lcd.print(lista[i]);

          if(lista[i] != 0)
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
  lcd.print("YA SALIERON TODOS");  

  while(digitalRead(BTN1) == HIGH)
  {
    lcd.blink();
  }

  delay(2000);

  
}
