#include <Wire.h> // Used to establied serial communication on the I2C bus
#include "SparkFunTMP102.h"   // https://github.com/sparkfun/SparkFun_TMP102_Arduino_Library
#include <Adafruit_GFX.h>  // https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_PCD8544.h> // https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library
#include <SimpleDHT.h> // https://github.com/winlinvip/SimpleDHT


int pinDHT11 = 7;
SimpleDHT11 dht11;
int sensorValue, indexUv;

Adafruit_PCD8544 display = Adafruit_PCD8544(5, 4, 3);
TMP102 sensor0(0x48);

#define BUZZER_PIN 8

const byte clouds[] PROGMEM = {48,20,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B11111110,B00000000,B00000000,
B00000000,B00011000,B00000001,B11111111,B00000000,B00000000,
B00000000,B01111100,B00011101,B11111111,B01110000,B00000000,
B00000011,B11111111,B10111111,B11111111,B11111100,B00000000,
B00000111,B11111111,B11111111,B11111111,B11111110,B00000000,
B00001111,B11111111,B11111111,B11111111,B11111110,B00000000,
B00001111,B11111111,B11111111,B11111111,B11111111,B00000000,
B00000111,B11111111,B11111111,B11111111,B11111111,B00000000,
B00000000,B00000001,B11111111,B11111111,B11111111,B00000000,
B00000000,B00000001,B11111111,B11111111,B11111110,B00000000,
B00000000,B00000000,B11111111,B11111111,B11111110,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
};

static const unsigned char PROGMEM soare_bmp[] = {
  B10000010,
  B01000100,
  B00011000,
  B11111111,
  B00011000,
  B01000100,
  B10000001
};

static const unsigned char PROGMEM picatura_bmp[] = {
  B00100000,
  B00100000,
  B01110000,
  B11111000,
  B11111000,
  B11111000,
  B01111000
};

static const unsigned char PROGMEM baterie_bmp[] = {
  B00111100,
  B00111100,
  B11111111,
  B11100111,
  B11100111,
  B11100111,
  B11100111,
  B11100111,
  B11100111,
  B11100111,
  B11111111
};

void setup() {
  Serial.begin(9600);
  sensor0.begin();
  Wire.begin();
  display.begin();

  display.setContrast (60);
  display.display();
  delay(2000);

  display.clearDisplay();
  display.display();
 
}

void loop() {

  float temperatureS = sensor0.readTempC();
  int err = SimpleDHTErrSuccess;
  
  indexUv = check(sensorValue);
  indexUv = 3;
  byte temperature = 0;
  byte humidity = 0;

  if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }

  Serial.print("Val adc ");
  Serial.print(sensorValue);
  Serial.print(" Indice UV : ");
  Serial.println(indexUv);
  Serial.print("Spk tmp ");
  Serial.println(temperatureS);


  display.clearDisplay();
  display.setCursor (30,0);
  display.setTextColor (BLACK);
  display.setTextSize(1);
  display.setTextColor (BLACK);
  display.setCursor (0,10);
  display.setTextSize(1);
  display.print(temperatureS,1);
  display.setCursor (25,10);
  display.print("o");
  display.setCursor (32,10);
  display.setTextSize(1);
  display.print("C");
  display.setCursor (36,10);
  display.print("   ");

  display.setTextColor (BLACK);
  display.setCursor (0,20);
  display.setTextSize(1);
  display.print("H :");
  display.print((int)humidity,1);
  display.setCursor (26,20);
  display.print("   ");
  display.setCursor (55,20);
  display.setTextSize(1);
  display.print("I:");
  display.print(indexUv);

  if((int)humidity > 29)
      display.drawBitmap(6,40,picatura_bmp,5,8,BLACK);
  if(temperatureS > 27)
  {
    display.setCursor (40,30);
    display.print("Alarma!");
    tone(BUZZER_PIN, 3000 , 500);
    delay(1000);
    noTone(BUZZER_PIN);
  }
  
  

  //if(1 == 1)
   //   display.drawBitmap(75,0,baterie_bmp,8,11,BLACK);

  display.display();

  delay(1000);

}


byte check(int index){

if(index<=55)
{
  return 0;
}
      
else if(index > 55 && index <= 227) 
      {
        return 1;
      }
    

else if(index > 227 && index <= 318) 
     {
      return 2;
     }
  

else if(index > 318 && index <= 408) 
      {
      return 3;
      }
     

else if(index > 408 && index <= 503) 
      {
        return 4;
      }
      

  else if(index > 503 && index <= 606) 
      {
        return 5;
      }
     

  else if(index > 606 && index <= 696) 
      {
        return 6;
      }
     

  else if(index > 696 && index <= 795) 
      {
        return 7;
      }
      

  else if(index > 795 && index <= 881) 
      {
        return 8;
      }
      

  else if(index > 881 && index <= 976) 
      {
        return 9;
      }
    

  else if(index > 976 && index <= 1079) 
      {
        return 10;
      }
      

  else if (index > 1079 && index <= 1170) 
      {
        return 11;
      }
}



