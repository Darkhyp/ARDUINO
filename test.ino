#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//I2C pins declaration
// extract LCD+library.zip to C:\Users\GeePS\Documents\Arduino\libraries\
// если на переходнике LCM1602
// запаяны A0 A1 A2 — адрес 0x20, не запаяны — 0x27.
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

#include "timer-api.h"
#include "string.h"

int ledpin = 11;// initialize digital pin 11 for output
int potpin = 0; // initialize analog pin 0
int val = 0; // Temporarily store variables' value from the sensor
int portSPEED = 9600;
//int portSPEED = 19200;



volatile bool isblink = false;
volatile bool oldstate = false;
void timer_handle_interrupts(int timer) {
    // мигаем лампочкой
    if (isblink)
      digitalWrite(ledpin, !digitalRead(ledpin));
}

void setup() {
  pinMode(ledpin, OUTPUT); // Инициализация светодиода
  timer_init_ISR_1Hz(TIMER_DEFAULT);

  lcd.begin (16,2); // for 16 x 2 LCD module
//  lcd.setBacklightPin(3,POSITIVE);
//  lcd.setBacklight(HIGH);
  
  lcd.home ();  // go home
  lcd.print("START...");

  Serial.begin(portSPEED); // Инициализация Serial - порта
//  attachInterrupt(0, SerialInterrupt, CHANGE);
//  Serial.attachSerialInterrupt(SerialInterrupt);

  lcd.clear();
  lcd.print("Init Serial...");
  lcd.setCursor(0, 1);
  lcd.print(String(portSPEED));
  lcd.print(" baud");
}


void loop() {
  SerialInterrupt();
/*  
 
  if (Serial.available() > 0) {
    inChar = Serial.read();
    lcd.clear();
    lcd.setCursor(0, 0); //At first row first column 
    lcd.print(inChar);
  }
*/
}

void SerialInterrupt(){
  if (Serial.available() > 0) {
    char inChar = Serial.read();
    lcd.clear();
    lcd.setCursor(0, 0); //At first row first column 
    switch (inChar){
      case 't':
        lcd.print("test");

//        Serial.println("port speed test");
//        Serial.println("test");
        Serial.println('T');
        break;

      case 'c': // c - connection established
        lcd.print("Connected! ");
        break;

      case 'e': // e - Enable LED
        isblink = false;

        lcd.print("LED on! ");  

        digitalWrite(ledpin,HIGH);
        break;

      case 'd': // d - Disable LED
        isblink = false;
 
        lcd.print("LED off! ");  

        digitalWrite(ledpin,LOW);
        break;

      case 'b':  // b - Blink LED
        isblink = !isblink;
        if(isblink)
          oldstate = digitalRead(ledpin);
        else
          digitalWrite(ledpin, oldstate);
        
        lcd.print("LED blink! ");  
        break;

      case 'v':   // v - read ADC
        val = analogRead(potpin); // read the analog value of analog pin 0, and assign it to val
        Serial.println(val); // send val’s value

        lcd.print("Voltmeter:"); //Print this
        lcd.setCursor(0, 1); //At secound row first column 
        lcd.print(val*5./(1024-1)); 
        lcd.println('V'); 
        break;
      default:
        lcd.print("Unknown command:"); 
        lcd.setCursor(0, 1); //At secound row first column 
        lcd.print(inChar); 
    }
  }
}
