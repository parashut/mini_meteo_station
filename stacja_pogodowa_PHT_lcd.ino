//ProMini 5V 16mhz, dth11, ds18b20, mpl3115a2

#include <dht11.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

#include <Wire.h>
#include "SparkFunMPL3115A2.h"

#define TEMP 7
#define DHT11PIN 6

dht11 DHT11;

OneWire oneWire(TEMP);
DallasTemperature sensors(&oneWire);
// arrays to hold device address
DeviceAddress insideThermometer;

LiquidCrystal lcd(A3, A2, A1, A0, 12, 11); //12,11,5,4,3,2 (RS,E,D4,D5,D6,D7)

MPL3115A2 myPressure;  //Create an instance of the object

void setup() {
  Wire.begin();        // Join i2c bus
  Serial.begin(9600);
  sensors.begin();
  lcd.begin(16, 2);
  //set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors.setResolution(insideThermometer, 11);  //9-12 bitow, 1/2 - 1/16 stopnia dokladnosc
  
  myPressure.begin(); // Get sensor online
  myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa
  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags
}

void loop() {
  lcd.setCursor(14, 0);  //wymazuje na chwile "OK"
  lcd.print("  ");
  
  float pressure = (myPressure.readPressure()) / 100;  //wynik w hPa
  lcd.setCursor(0, 0);
  lcd.print("P: ");
  lcd.print(pressure, 2);
  
  int chk = DHT11.read(DHT11PIN);         //sprawdzenie stanu sensora, a następnie wyświetlenie komunikatu na monitorze szeregowym
  
  lcd.setCursor(0, 1);
  
  switch (chk)
  {
    case DHTLIB_OK:  
        break;
    case DHTLIB_ERROR_CHECKSUM: 
        lcd.println("Blad sumy kontr."); 
        break;
    case DHTLIB_ERROR_TIMEOUT: 
        lcd.println("brak odpowiedzi"); 
        break;
    default: 
        lcd.println("Nieznany blad"); 
        break;
  }
  lcd.print("H: ");              //wyświetlenie wartości wilgotności
  lcd.print((float)DHT11.humidity, 1);
  lcd.print(" ");
  
  sensors.requestTemperatures();
  lcd.setCursor(14, 0);
  lcd.print("OK");
  
  lcd.setCursor(8, 1);
  lcd.print("T: ");
  lcd.print(sensors.getTempCByIndex(0));
  
  delay(5000);
}
