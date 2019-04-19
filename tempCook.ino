#include <DallasTemperature.h>
#include <OneWire.h>
#include <PID_v1.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Temperture probe data line on pin 4
#define TEMP_IN 4

//Create One Wire Instance
OneWire thermometer(TEMP_IN);

DallasTemperature sensors(&thermometer);
  //Create the LCD
LiquidCrystal_I2C lcd(0x27,16,2);

float currentTemp;
int tempIn = 4;
int relayPin = 8;


OneWire ds(tempIn);
void setup() {

  pinMode(relayPin,OUTPUT);
  // Initialize LCD Display
 lcd.init();

  //Turn on Blacklight and print message
  lcd.backlight();
  
  sensors.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
lcd.setCursor(3,0);  
lcd.print("Welcome to");
lcd.setCursor(3,1);
lcd.print("TempCook!");



delay(5000);
lcd.clear();
  

lcd.print(getTemp());
digitalWrite(relayPin,HIGH);
delay(2000);
lcd.clear();
digitalWrite(relayPin,LOW);
}

  //Fuction that reports temperature from probe
float getTemp(){
sensors.requestTemperatures();
currentTemp=sensors.getTempFByIndex(0);
return currentTemp;
}
