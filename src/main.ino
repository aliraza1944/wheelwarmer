#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Ticker.h>
#include <MCP3008.h>
#include <SPI.h>

/*
Pin definitions
 */
#define DHTPIN D3
#define DHTTYPE DHT22

/*
ADC Pin Definitions
 */
 #define CS_PIN D8    //PADS 1-8
 #define CS2_PIN D0   //PADS 9-16
 #define CLOCK_PIN 14
 #define MOSI_PIN 13
 #define MISO_PIN 12

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
Ticker mcpTicker;

/*
DHT22 Initialization
 */
DHT dht(DHTPIN, DHTTYPE);

/*
MCP3008 Initialization
 */
MCP3008 adc(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS_PIN);
MCP3008 adc2(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS2_PIN);


void setup(){
  Serial.begin(115200);
  Serial.println("Starting...");
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("WheelWarmer");
  delay(2000);
  lcd.clear();
  dht22Ticker.attach(10, tempFlag);
  lcd.setCursor(0,0);
  lcd.print("Starting Sensors");
  delay(2000);
  lcd.clear();
}

void loop(){

#if 0
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
#endif

  int val = adc.readADC(1); // read Chanel 0 from MCP3008 ADC
  Serial.print("ADC 1 : ");
  Serial.println(val);
  delay(500);
  int val2 = adc2.readADC(7); // read Chanel 0 from MCP3008 ADC
  Serial.print("ADC 2 : ");
  Serial.println(val2);
  delay(1000);

}

void tempFlag(){
  tempCheckFlag = true;
  //Serial.println("Flag set");
}
