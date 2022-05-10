#include "HX711.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "RunningAverage.h"

#define LOADCELL_DOUT_PIN  2
#define LOADCELL_SCK_PIN  3

HX711 scale;
LiquidCrystal_I2C lcd(0x27, 16, 2);
RunningAverage myRA(10);

float calibration_factor = -460;
float threshold = 2;

void setup() {
  // Display initialization
  lcd.begin();

  // Load cell initialization
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
}

void loop() {
  // put your main code here, to run repeatedly:
  startup();
}

void startup() {
  Serial.begin(9600);
  
  lcd.clear();
  scale.tare();
  
  lcd.setCursor(0,0);
  lcd.print("THE PERFECT POUR");
  lcd.setCursor(6,1);
  lcd.print("v0.0");
  delay(2000);
  tareGlass();
}

void tareGlass() {
  float reading = 0;
  float average = 0;
  float point1 = 0;
  float point2 = 0;

  scale.tare();
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PUT EMPTY GLASS");
  lcd.setCursor(4,1);
  lcd.print("ON SCALE");

  while(reading <= threshold){
    reading = scale.get_units();
  }

  delay(750);
  point1 = scale.get_units();
  delay(250);
  point2 = scale.get_units();
  if(point1 >= threshold && point2 >= threshold){
    if(abs(point1 - point2) <= threshold){
      scale.tare();
      measureWine();
    }
    else{
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("REMOVE GLASS");
        while(reading >= threshold){
          reading = scale.get_units();
        }
        tareGlass();
    }
  }
  else if(point1 <= threshold && point2 >= threshold){
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("REMOVE GLASS");
    while(reading >= threshold){
      reading = scale.get_units();
    }
    tareGlass();
  }
 else{
   tareGlass();
 }
}

void measureWine() {
  float reading = 0;
  float average = 0;
  float ml = 0;
  int perfect = 0;
  int tad = 0;
  int touch = 0;
  int drip = 0;

  myRA.clear();
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  PRFCT   TAD");
  lcd.setCursor(0,1);
  lcd.print("  TOUCH   DRIP");
  
  while(average >= -0.9*threshold){
    reading = scale.get_units();
    myRA.addValue(reading);
    average = myRA.getAverage();
    Serial.println(average, 3);

    ml = average;
    Serial.println(ml);
    perfect = ml / 120;
    tad = (ml - perfect*120) / 60;
    touch = (ml - perfect*120 - tad*60) / 30;
    drip = (ml - perfect*120 - tad*60 - touch*30) / 15;

    if(perfect < 0) {
      perfect = 0;
    }
    if(tad < 0) {
      tad = 0;
    }
    if(touch < 0) {
      touch = 0;
    }
    if(drip < 0) {
      drip = 0;
    }
    
    lcd.setCursor(0,0);
    lcd.print(perfect);
    lcd.setCursor(8,0);
    lcd.print(tad);
    lcd.setCursor(0,1);
    lcd.print(touch);
    lcd.setCursor(8,1);
    lcd.print(drip);    
  }

  delay(500);
  tareGlass();
}
