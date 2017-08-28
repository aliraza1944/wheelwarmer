#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Ticker.h>


/*
Pin definitions
 */
#define DHTPIN D3
#define DHTTYPE DHT22

/*
Flags Initialization
 */
static bool tempCheckFlag = false;

/*
Global Variables
 */
float temperature = 0.0;

/*
LCD Initialization
 */
LiquidCrystal_I2C lcd(0x3F, 16, 2);

/*
Ticker Intialization
 */
Ticker dht22Ticker;

/*
DHT22 Initialization
 */
DHT dht(DHTPIN, DHTTYPE);


void setup(){
  Serial.begin(115200);
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("WheelWarmer");
  delay(2000);
  lcd.clear();
  dht22Ticker.attach(5, tempFlag);
  lcd.setCursor(0,0);
  lcd.print("Starting Sensors");
  delay(2000);
  lcd.clear();
}

void loop(){

  if(tempCheckFlag){

    temperature = dht.readTemperature();

    if (isnan(temperature)) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Sensor Error");
      tempCheckFlag = false;
      return;
    }
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Temp : ");
    lcd.print(temperature);

    tempCheckFlag = false;

  }

}

void tempFlag(){
  tempCheckFlag = true;
  Serial.println("Flag set");
}
