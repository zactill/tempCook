#include <DallasTemperature.h>
#include <OneWire.h>
#include <PID_v1.h>
//#include <PID_AutoTune_v0.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Pin Assignments
#define tempIn 4
#define relayPin 2
#define downButton 8
#define upButton 9
#define enterButton 10
#define ledPin 13

//Define variables for PID and constancts we are conneccting to 
double setPoint = 75.0;
double Input, Output;
volatile long onTime = 0;

//PID tuning parameters
double kp = 220;
double ki = 0.1;
double kd = 0.1;

//Tuning parameters for PID
PID myPID(&Input, &Output, &setPoint, kp, ki, kd, DIRECT);

//10 second Time output window
int windowSize = 10000;
unsigned long windowStartTime;

boolean isSet = false;
boolean tuning = false;
boolean overTemp = false;

//PID_ATune atune(&Input, &Output);

//*********************
//Auto Tune variables
//*********************

byte ATuneModeRemember=2;

double aTuneStep = 500;
double aTuneNoise = 1;
unsigned int aTuneLookBack=20;

  //Create the LCD
LiquidCrystal_I2C lcd(0x27,16,2);

//Create One Wire Instance
OneWire oneWire(tempIn);

DallasTemperature sensors(&oneWire);

//EEPROM addresses for persistent memory
const int SpAddress = 0;
const int KpAddress = 8;
const int KiAddress = 16;
const int KdAddress = 24;

int upState=0;
int downState=0;
int enterState=0;

void setup() {

  
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(relayPin,OUTPUT);
  pinMode(upButton,INPUT);
  pinMode(downButton,INPUT);
  pinMode(enterButton,INPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(relayPin, LOW);
  
  // Initialize LCD Display
  lcd.init();

  //Turn on Blacklight and print message
  lcd.backlight();

  //Start instance of thermometer
  sensors.begin();
  
  //Welcome Screen
  lcd.setCursor(3,0);  
  lcd.print("Welcome to");
  lcd.setCursor(3,1);
  lcd.print("TempCook!");

  delay(3000);
  lcd.clear();

  lcd.setCursor(0,0); 
  lcd.print("Use UP and DOWN");
  lcd.setCursor(3,1);
  lcd.print("to select");

  delay(4000);
  lcd.clear();

  lcd.setCursor(2,0); 
  lcd.print("temperature.");
  lcd.setCursor(1,1);
  lcd.print("Press ENTER to");

  delay(4000);
  lcd.clear();

  lcd.setCursor(0,0); 
  lcd.print("toggle heating.");
  lcd.setCursor(5,1);
  lcd.print("Enjoy!");
  delay(4000);
  lcd.clear();

  //Initialize PID
  myPID.SetTunings(kp,ki,kd);
  myPID.SetSampleTime(1000);
  myPID.SetOutputLimits(0, windowSize);

  //Start PID
  windowStartTime = millis();
  myPID.SetMode(AUTOMATIC);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //start getting temperatures
  sensors.requestTemperatures();

  lcd.setCursor(2,0);
  lcd.print("Current Temp:");
  lcd.setCursor(0,1);
  lcd.print("      ");
  lcd.setCursor(5,1);
  lcd.print(getTemp());
  lcd.setCursor(11,1);
  lcd.print("    ");
  



  //gets button states
  upState = digitalRead(upButton);
  downState = digitalRead(downButton);
  enterState = digitalRead(enterButton);

  
    if (upState == HIGH) {
    // turn LED on:
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    // turn LED off:
    digitalWrite(LED_BUILTIN, LOW);
  }

  //Set Temperature
  //setPoint= input from buttons

  //Read Current Temp
  Input = getTemp();

  //Checks to see if temp is over setpoint
  //overTemp=(getTemp()>setPoint);

}


  //Fuction that reports temperature from probe
float getTemp(){
double currentTemp;
currentTemp=sensors.getTempFByIndex(0);
return currentTemp;
}

//Sets temperature from user input


//Turn the heating element on or off
void heat(void){

  if(overTemp){
    digitalWrite(relayPin,LOW);
    return;
  }

  long now = millis();

  if(now - windowStartTime > windowSize){
    windowStartTime += windowSize;
  }

  if((Output >100) && (Output > (now -windowStartTime))){
    digitalWrite(relayPin,HIGH);
  } else{
    digitalWrite(relayPin,LOW);
  }

  



}
