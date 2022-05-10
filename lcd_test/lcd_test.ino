#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

void setup() {
  // put your setup code here, to run once:
  LiquidCrystal_I2C lcd(0x27, 16, 2);
  lcd.begin();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ABCDEFGHIJKLMNOP");
  lcd.setCursor(0,1);
  lcd.print("QRSTUVWXYZ123456");
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
