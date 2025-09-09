//Prateek
//www.prateeks.in
//https://justdoelectronics.com

/*
int count = 0;
char card_no[12];
void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    count = 0;
    while (Serial.available() && count < 12) {
      card_no[count] = Serial.read();
      count++;
      delay(5);
    }
    Serial.print(card_no);
  }
}
*/

#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>

DS3231 rtc(SDA, SCL);

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial sim(2, 3);
String number = "+91xxxxxxxxxx";
String number1 = "+91xxxxxxxxxx";
String number2 = "+91xxxxxxxxxx";
String number3 = "+91xxxxxxxxxx";
String number4 = "+91xxxxxxxxxx";
int state1 = 0;
int state2 = 0;
int state3 = 0;
int state4 = 0;

#define buzzerPin 5
#define yellow 6
#define green 7


String page = "";
char input[12];
int count = 0;
int a;
double total = 0;
int count_prod = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  Serial.begin(9600);
  sim.begin(9600);
  rtc.begin();


  pinMode(buzzerPin, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("    WELCOME    ");
  lcd.setCursor(0, 1);
  lcd.print("TO OUR PROJECT");
  delay(4000);
  lcd.clear();
}

void loop() {
  rfid();
  RTC();
}

void RTC() {
  //rtc.setDOW(FRIDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(13, 52, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(4, 10, 2024);   // Set the date to January 1st, 2014
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Put Your Card to");
  lcd.setCursor(0, 1);
  lcd.print("the Reader......");
  delay(800);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(rtc.getTimeStr());
  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(rtc.getDateStr());
  delay(800);
  lcd.clear();
}

void rfid() {
  if (Serial.available()) {
    count = 0;
    while (Serial.available() && count < 12) {
      input[count] = Serial.read();
      count++;
      delay(5);
    }
    if (count == 12) {
      if ((strncmp(input, "4B00E1999CAF", 12) == 0) && (state1 == 0)) {
        beepON();
        digitalWrite(green, HIGH);
        delay(2000);
        digitalWrite(green, LOW);
        lcd.clear();
        lcd.setCursor(7, 0);
        lcd.print("Prateek");
        lcd.setCursor(11, 1);
        lcd.print("001");
        info();
        SendPrateek();
        state1 = 1;
      } else if ((strncmp(input, "4B00E1999CAF", 12) == 0) && (state1 == 1)) {
        beepOFF();
        digitalWrite(yellow, HIGH);
        delay(2000);
        digitalWrite(yellow, LOW);
        lcd.clear();
        lcd.setCursor(7, 0);
        lcd.print("Prateek");
        lcd.setCursor(11, 1);
        lcd.print("001");
        info();
        SendPrateek();
        state1 = 0;
      } else if ((strncmp(input, "5500140E216E", 12) == 0) && (state2 == 0)) {
        beepON();
        digitalWrite(green, HIGH);
        delay(2000);
        digitalWrite(green, LOW);
        lcd.clear();
        lcd.setCursor(7, 0);
        lcd.print("Sonu");
        lcd.setCursor(11, 1);
        lcd.print("002");
        info();
        SendSonu();
        state2 = 1;
      } else if ((strncmp(input, "5500140E216E", 12) == 0) && (state2 == 1)) {
        beepOFF();
        digitalWrite(yellow, HIGH);
        delay(2000);
        digitalWrite(yellow, LOW);
        lcd.clear();
        lcd.setCursor(7, 0);
        lcd.print("Sonu");
        lcd.setCursor(11, 1);
        lcd.print("002");
        info();
        SendSonu();
        state2 = 0;
      } else if ((strncmp(input, "550014285E37", 12) == 0) && (state3 == 0)) {
        beepON();
        digitalWrite(green, HIGH);
        delay(2000);
        digitalWrite(green, LOW);
        lcd.clear();
        lcd.setCursor(7, 0);
        lcd.print("Punit");
        lcd.setCursor(11, 1);
        lcd.print("003");
        info();
        SendPunit();
        state3 = 1;
      } else if ((strncmp(input, "550014285E37", 12) == 0) && (state3 == 1)) {
        beepOFF();
        digitalWrite(yellow, HIGH);
        delay(2000);
        digitalWrite(yellow, LOW);
        lcd.clear();
        lcd.setCursor(7, 0);
        lcd.print("Punit");
        lcd.setCursor(11, 1);
        lcd.print("003");
        info();
        SendPunit();
        state3 = 0;
      } else if ((strncmp(input, "550014B9BC44", 12) == 0) && (state4 == 0)) {
        beepON();
        digitalWrite(green, HIGH);
        delay(2000);
        digitalWrite(green, LOW);
        lcd.clear();
        lcd.setCursor(7, 0);
        lcd.print("Ram");
        lcd.setCursor(11, 1);
        lcd.print("004");
        info();
        SendRam();
        state4 = 1;
      } else if ((strncmp(input, "550014B9BC44", 12) == 0) && (state4 == 1)) {
        beepOFF();
        digitalWrite(yellow, HIGH);
        delay(2000);
        digitalWrite(yellow, LOW);
        lcd.clear();
        lcd.setCursor(7, 0);
        lcd.print("Ram");
        lcd.setCursor(11, 1);
        lcd.print("003");
        info();
        SendRam();
        state4 = 0;
      }

      else {
        digitalWrite(buzzerPin, HIGH);
        beepOFF();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ID : ");
        lcd.print("Unknown");
        lcd.setCursor(0, 1);
        lcd.print("Access denied");
        Serial.println(" Access denied");
        delay(1500);
        digitalWrite(buzzerPin, LOW);
        lcd.clear();
      }
    }
  }
}

void smsSend() {
  lcd.setCursor(0, 0);
  lcd.print("SMS Sending");
  for (int x = 11; x < 16; x++) {
    lcd.setCursor(x, 0);
    lcd.print(".");
    delay(1000);
    lcd.clear();
  }
}
void beepON() {
  digitalWrite(buzzerPin, HIGH);
  delay(200);
  digitalWrite(buzzerPin, LOW);
  delay(100);
}

void beepOFF() {
  digitalWrite(buzzerPin, HIGH);
  delay(50);
  digitalWrite(buzzerPin, LOW);
  delay(10);
}
void info() {
  lcd.setCursor(0, 0);
  lcd.print("Name : ");
  lcd.setCursor(0, 1);
  lcd.print("Roll No : ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Authorized Access");
  delay(1000);
  lcd.clear();
}

void SendPrateek() {
  lcd.setCursor(0, 0);
  lcd.print("SMS Sending");
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + number + "\"\r");
  delay(1000);
  if (state1 == 0) {
    const char Prateek[] = " Prateek is Present at School ";
    sim.print(Prateek);
  } else if (state1 == 1) {
    const char Prateek[] = " Prateek is Out Of The School";
    sim.print(Prateek);
  }
  delay(100);
  sim.println((char)26);
  smsSend();
}

void SendSonu() {
  lcd.setCursor(0, 0);
  lcd.print("SMS Sending");
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + number1 + "\"\r");
  delay(1000);
  if (state2 == 0) {
    const char Sonu[] = "Sonu is Present at School  ";
    sim.print(Sonu);
    sim.print(rtc.getTimeStr());
  } else if (state2 == 1) {
    const char Sonu[] = "Sonu is Out Of The School  ";
    sim.print(Sonu);
    sim.print(rtc.getTimeStr());
  }

  delay(100);
  sim.println((char)26);
  smsSend();
}
void SendPunit() {
  lcd.setCursor(0, 0);
  lcd.print("SMS Sending");
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + number2 + "\"\r");
  delay(1000);
  if (state3 == 0) {
    const char Punit[] = "Punit is Present at School  ";
    sim.print(Punit);
    sim.print(rtc.getTimeStr());
  } else if (state3 == 1) {
    const char Punit[] = "Punit is Out Of The School  ";
    sim.print(Punit);
    sim.print(rtc.getTimeStr());
  }

  delay(100);
  sim.println((char)26);
  smsSend();
}
void SendRam() {
  lcd.setCursor(0, 0);
  lcd.print("SMS Sending");
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + number3 + "\"\r");
  delay(1000);
  if (state4 == 0) {
    const char Ram[] = "Ram is Present at School  ";
    sim.print(Ram);
    sim.print(rtc.getTimeStr());
  } else if (state4 == 1) {
    const char Ram[] = "Ram is Out Of The School  ";
    sim.print(Ram);
    sim.print(rtc.getTimeStr());
  }

  delay(100);
  sim.println((char)26);
  smsSend();
}