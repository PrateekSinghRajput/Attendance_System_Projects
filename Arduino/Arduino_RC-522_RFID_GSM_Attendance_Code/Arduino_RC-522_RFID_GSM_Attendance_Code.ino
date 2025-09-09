//Prateek
//www.prateeks.in
//https://justdoelectronics.com

#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <MFRC522.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial sim(2, 3);
RTC_DS3231 rtc;

#define RST_PIN 9
#define SS_PIN 10
MFRC522 rfid(SS_PIN, RST_PIN);

#define buzzerPin 5
#define yellow 6
#define green 7

struct User {
  String rfidTag;
  String name;
  String rollNumber;
  String mobileNumber;
  int state;
};

User users[] = {
  { "E3497CE2", "Prateek", "001", "+91xxxxxxxxxx", 0 },
  { "E3EC04E2", "Sonu", "002", "+91xxxxxxxxxx", 0 },
  { "C33149E2", "Punit", "003", "+91xxxxxxxxxx", 0 },
  { "29F427B3", "Rahul", "004", "+91xxxxxxxxxx", 0 },
  { "735EC01C", "Ram", "005", "+91xxxxxxxxxx", 0 }
};

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  sim.begin(9600);

  SPI.begin();
  rfid.PCD_Init();

  if (!rtc.begin()) {
    lcd.setCursor(0, 0);
    lcd.print("RTC NOT FOUND");
    while (1)
      ;
  }

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
  displayDateTime();
  rfidScan();
}

void displayDateTime() {
  DateTime now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print("Date: ");
  lcd.print(now.day());
  lcd.print("/");
  lcd.print(now.month());
  lcd.print("/");
  lcd.print(now.year());

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(now.hour());
  lcd.print(":");
  lcd.print(now.minute());
  lcd.print(":");
  lcd.print(now.second());
  delay(1000);
}

void rfidScan() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  String rfidTag = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    rfidTag += String(rfid.uid.uidByte[i], HEX);
  }
  rfidTag.toUpperCase();

  for (int i = 0; i < sizeof(users) / sizeof(users[0]); i++) {
    if (rfidTag == users[i].rfidTag) {
      handleUserAccess(users[i]);
      rfid.PICC_HaltA();
      return;
    }
  }

  handleUnauthorizedAccess();
  rfid.PICC_HaltA();
}

void handleUserAccess(User &user) {
  beepON();
  if (user.state == 0) {
    digitalWrite(green, HIGH);
    lcd.clear();
    lcd.setCursor(7, 0);
    lcd.print(user.name);
    lcd.setCursor(11, 1);
    lcd.print(user.rollNumber);
    info();
    sendSMS(user, "is Present at School");
    user.state = 1;
  } else {
    digitalWrite(yellow, HIGH);
    lcd.clear();
    lcd.setCursor(7, 0);
    lcd.print(user.name);
    lcd.setCursor(11, 1);
    lcd.print(user.rollNumber);
    info();
    sendSMS(user, "is Out Of The School");
    user.state = 0;
  }
  delay(2000);
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
}

void handleUnauthorizedAccess() {
  digitalWrite(buzzerPin, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ID : Unknown");
  lcd.setCursor(0, 1);
  lcd.print("Access denied");
  delay(1500);
  digitalWrite(buzzerPin, LOW);
  lcd.clear();
}

void sendSMS(User &user, String status) {
  lcd.setCursor(0, 0);
  lcd.print("SMS Sending");
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + user.mobileNumber + "\"\r");
  delay(1000);
  sim.print(user.name + " " + status + "\n" + getCurrentDateTime());
  delay(100);
  sim.println((char)26);
  smsSend();
}

void smsSend() {
  for (int x = 11; x < 16; x++) {
    lcd.setCursor(x, 0);
    lcd.print(".");
    delay(1000);
  }
  lcd.clear();
}

void beepON() {
  digitalWrite(buzzerPin, HIGH);
  delay(200);
  digitalWrite(buzzerPin, LOW);
  delay(100);
}

void info() {
  lcd.setCursor(0, 0);
  lcd.print("Name : ");
  lcd.setCursor(0, 1);
  lcd.print("Roll No : ");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Authorized Access");
  delay(1000);
  lcd.clear();
}

String getCurrentDateTime() {
  DateTime now = rtc.now();
  char buffer[30];
  sprintf(buffer, "Date: %02d/%02d/%04d Time: %02d:%02d:%02d",
          now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second());
  return String(buffer);
}