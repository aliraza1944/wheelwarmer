/*
TODO:

  1. Work on error detection.
      -Read ACS712 Correctly
  2. Add Relay
  3. Add HC-05

 */


#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Ticker.h>
#include <MCP3008.h>
#include <SPI.h>
#include <SoftwareSerial.h>


/*
Pin definitions
 */
#define DHTPIN D3
#define DHTTYPE DHT22
#define relayPin D8

/*
ADC Pin Definitions
 */

#if 0
 #define CS_PIN D0    //PADS 1-8
 #define CS2_PIN D4   //PADS 9-16
 #define CLOCK_PIN 14
 #define MOSI_PIN 13
 #define MISO_PIN 12

 #define ADC_SCALE 1023.0
 #define VREF 5.0
 #define sensitivity 0.185
#endif
/*
Flags Initialization
 */
static bool tempCheckFlag = false;

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
#if 0
MCP3008 adc(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS_PIN);
MCP3008 adc2(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS2_PIN);
#endif

/*
Global Variables
 */
float temperature = 0.0;
float thresholdTemp = 0.0;
int zeroFactor[31] = {0};   //ZeroFactor for all attached ACS712s. Set thru calibrateAll Function.
float current[31] = {0.00}; //Current Sensed for all attached ACS712s.

/*
Function Declarations
 */
#if 0
void calibrateAll();   //Calibrates all 32 ACS712 Sensors.
void getCurrent();     //Gets Current from all attached ACS712s.
#endif

void setup(){
  Serial.begin(38400);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
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
  //calibrateAll();
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

  if(Serial.available())
  {
    String cmd = Serial.readString();
    thresholdTemp = cmd.toFloat();
    Serial.print("thresholdTemp : ");
    Serial.println(thresholdTemp);
  }

  if(temperature > thresholdTemp){
    digitalWrite(relayPin, HIGH);
  }
  else
    digitalWrite(relayPin, LOW);

}

/*
Function Definitions
 */
void tempFlag(){
  tempCheckFlag = true;
}


#if 0
void calibrateAll(){
  uint16_t acc[31] = {0};

  for(int j = 0; j <= 7; j++)
    for(int i = 0; i <= 10 ; i++){
      acc[j] += adc.readADC(j);
    }

  for(int j = 0; j <= 7; j++)
    for(int i = 0; i <= 10 ; i++){
      acc[j+8] += adc2.readADC(j);
    }

  for(int i = 0; i <= 31; i++){
    zeroFactor[i] = acc[i]/10;
  }

  Serial.println(zeroFactor[0]);
  Serial.println(zeroFactor[8]);
}

void getCurrent(){

  int16_t acc[31] = {0};
  for(int i = 0; i <= 10 ; i++){
    //for ACS712 0-7
    acc[0] += adc.readADC(0) - zeroFactor[0];
    #if 0
    acc[1] += adc.readADC(1) - zeroFactor[1];
    acc[2] += adc.readADC(2) - zeroFactor[2];
    acc[3] += adc.readADC(3) - zeroFactor[3];
    acc[4] += adc.readADC(4) - zeroFactor[4];
    acc[5] += adc.readADC(5) - zeroFactor[5];
    acc[6] += adc.readADC(6) - zeroFactor[6];
    acc[7] += adc.readADC(7) - zeroFactor[7];
    //for ACS712 8-15
    #endif
    acc[8] += adc2.readADC(0) - zeroFactor[8];
    #if 0
    acc[9] += adc2.readADC(1) - zeroFactor[9];
    acc[10] += adc2.readADC(2) - zeroFactor[10];
    acc[11] += adc2.readADC(3) - zeroFactor[11];
    acc[12] += adc2.readADC(4) - zeroFactor[12];
    acc[13] += adc2.readADC(5) - zeroFactor[13];
    acc[14] += adc2.readADC(6) - zeroFactor[14];
    acc[15] += adc2.readADC(7) - zeroFactor[15];
    #endif
  }

  #if 0
  for(int i = 0; i <= 31; i++){
      current[i] = (float)acc[i] / 10.0 / ADC_SCALE * VREF / sensitivity;
  }
  #endif
  current[0] = (float)acc[0] / 10.0 / ADC_SCALE * VREF / sensitivity;
  current[8] = (float)acc[8] / 10.0 / ADC_SCALE * VREF / sensitivity;
}
#endif
