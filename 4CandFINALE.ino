#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

hd44780_I2Cexp lcd;

int pin_rpm = A0;
int pin_spark = A1;

int candela1 = 2;
int candela2 = 3;
int candela3 = 4;
int candela4 = 5;
int puls=6;

unsigned long phaseD2;
unsigned long spark2;
unsigned long T2;

byte cand1Stato = LOW;
byte cand2Stato = LOW;
byte cand3Stato = LOW;
byte cand4Stato = LOW;

unsigned long t;

unsigned long cand1start;
unsigned long cand1EndPulse;
unsigned long cand1End;

unsigned long cand2start;
unsigned long cand2EndPulse;
unsigned long cand2End;

unsigned long cand3start;
unsigned long cand3EndPulse;
unsigned long cand3End;

unsigned long cand4start;
unsigned long cand4EndPulse;
unsigned long cand4End;


boolean check = true;

void setup() {
  Serial.begin(9600);
  pinMode(candela1, OUTPUT);
  pinMode(candela2, OUTPUT);
  pinMode(candela3, OUTPUT);
  pinMode(candela4, OUTPUT);
  pinMode(puls, INPUT);

  digitalWrite(candela1, cand1Stato);
  digitalWrite(candela2, cand2Stato);
  digitalWrite(candela3, cand3Stato);
  digitalWrite(candela4, cand4Stato);


  //settaggi lcd
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Inizializzazione");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(" RPM = ");

  lcd.setCursor(0, 1);
  lcd.print("TIME = ");

  lcd.setCursor(12, 1);
  lcd.print("us");
}

void loop() {

    int statopuls = digitalRead(puls);
    if ((statopuls == HIGH) || (check == true)) {

    int val_rpm = analogRead(pin_rpm);
    int val_spark = analogRead(pin_spark);

    unsigned long int rpm = ((9000 / 1023.0) * val_rpm) + 1000;  //conversione in rpm

    float spark = ((5000.0 / 1023.0) * val_spark) + 1000;  //calcolo dwell time

    float T = ((60000.0 / ((float) rpm)) * 1000.0) * 2.0; // calcolo periodo

    float phaseD = T / 4;
 
    T2 = T;
    spark2 = spark;
    phaseD2 = phaseD;
    Serial.println(T);
    Serial.println(phaseD);

    //stampa su lcd
        if (rpm < 10000) {
          lcd.setCursor(11, 0);
          lcd.print(" ");
        }
        lcd.setCursor(7, 0);
        lcd.print(rpm);

        lcd.setCursor(7, 1);
        lcd.print(spark2);
    
     check = false;
     
     cand1start = micros();
     cand2start = cand1start + phaseD2;
     cand3start = cand2start + phaseD2;
     cand4start = cand3start + phaseD2;
 }
 
  else{
    cand1EndPulse = cand1start + spark2;
    cand1End = cand1start + T2;
    cand2EndPulse = cand2start + spark2;
    cand2End = cand2start + T2;
    cand3EndPulse = cand3start + spark2;
    cand3End = cand3start + T2;
    cand4EndPulse = cand4start + spark2;
    cand4End = cand4start + T2;

 t = micros();
/* Serial.println("t");
 Serial.println(t);

Serial.println(cand1start);
Serial.println(cand1End);
Serial.println(cand1EndPulse);
Serial.println(T2);
Serial.println(spark2);*/
 
// candela 1
    if ((t >= cand1start) && (t <= cand1EndPulse)) {
      cand1Stato = HIGH;
    }
    else if (t <= cand1End) {
      cand1Stato = LOW;
    }
    else cand1start = t;
   

// candela 2
  if ((t >= cand2start) && (t <= cand2EndPulse)) {
    cand2Stato = HIGH;
  }
  else if (t <= cand2End) {
    cand2Stato = LOW;
  }
  else cand2start = t;


// candela 3
  if ((t >= cand3start) && (t <= cand3EndPulse)) {
    cand3Stato = HIGH;
  }
  else if (t <= cand3End) {
    cand3Stato = LOW;
  }
  else cand3start = t;


// candela 4
  if ((t >= cand4start) && (t <= cand4EndPulse)) {
    cand4Stato = HIGH;
  }
  else if (t <= cand4End) {
    cand4Stato = LOW;
  }
  else cand4start = t;

  digitalWrite(candela1, cand1Stato);
  digitalWrite(candela2, cand2Stato);
  digitalWrite(candela3, cand3Stato);
  digitalWrite(candela4, cand4Stato);
 }
}
