#include <LiquidCrystal.h>

#include "Arduino.h"

// github.com/Phaiax/sainsmartkeypad
#include "sainsmartkeypad.h"

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
SainsmartKeypad keypad(0);
int value = 0;
uint8_t key;

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Keypad Example");
}

void loop() {
  key = keypad.getKey_fastscroll();
  
  // Try the other possibilities:
  // (Only one at a time.
  //  fastscroll, waitrelease and periodic
  //  won't work next to each other.)
  
  //key = keypad.getKey_waitrelease(); // without fastscroll but usable
  //key = keypad.getKey_periodic(); // not quite usable
  //key = keypad.getKey_instant(); // not quite usable
  
  if(key != SAMPLE_WAIT) // Do not refresh screen every loop
  {
     switch(key)
     {
       case UP_KEY:
        value++;
        break;
       case DOWN_KEY:
        value--;
        break;
     }
     lcd.setCursor(5,1);
     lcd.print(value);
     lcd.print("        ");
  }
}

