//Prateek
//www.justdoelectronics.com

#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include "LiquidCrystal.h"
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

#define buzzerPin 6
#define yellow 5
#define green 4

#define RST_PIN 9
#define SDA_PIN 10

#define CS_SD 8

MFRC522 mfrc522(SDA_PIN, RST_PIN);
File myFile;

void setup() {

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(buzzerPin, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("    WELCOME    ");
  lcd.setCursor(0, 1);
  lcd.print("TO OUR PROJECT");
  delay(4000);
  lcd.clear();

  while (!Serial)
    ;
  SPI.begin();
  Serial.print("Initializing SD card...");
  lcd.print("Initializing ");
  lcd.setCursor(0, 1);
  lcd.print("SD card...");
  delay(3000);
  lcd.clear();
  if (!SD.begin(CS_SD)) {
    Serial.println("initialization failed!");
    lcd.print("Initializing ");
    lcd.setCursor(0, 1);
    lcd.print("failed!");
    return;
  }
  Serial.println("initialization done.");
  lcd.print("Initialization ");
  lcd.setCursor(0, 1);
  lcd.print("Done...");
  lcd.clear();
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Put Your Card to");
  lcd.setCursor(0, 1);
  lcd.print("the Reader......");
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();


  if (content.substring(1) == "53 BE CD 95")  //change here the UID of the card/cards that you want to give access
  {
    digitalWrite(buzzerPin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Name : ");
    lcd.setCursor(0, 1);
    lcd.print("Roll No : ");
    lcd.setCursor(7, 0);
    lcd.print("Sandhya");
    lcd.setCursor(11, 1);
    lcd.print("001");
    delay(1000);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(CS_SD, LOW);
    myFile = SD.open("DATA.txt", FILE_WRITE);
    if (myFile) {
      Serial.println("File opened ok");
      lcd.clear();
      lcd.print("File opened ok");
      delay(2000);
      myFile.println("Sandhya .............. PRESENT");
      lcd.setCursor(0, 1);
      lcd.print("Written on SD...");
      delay(2000);
      lcd.clear();
      myFile.close();
    } else {
      Serial.println("error opening data.txt");
      lcd.clear();
      lcd.print("error opening data.txt");
    }
    digitalWrite(CS_SD, HIGH);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Put Your Card to");
    lcd.setCursor(0, 1);
    lcd.print("the Reader......");

  }

  else if (content.substring(1) == "43 B5 2F 96")  //change here the UID of the card/cards that you want to give access
  {
    digitalWrite(buzzerPin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Name : ");
    lcd.setCursor(0, 1);
    lcd.print("Roll No : ");
    lcd.setCursor(7, 0);
    lcd.print("Ashish");
    lcd.setCursor(11, 1);
    lcd.print("002");
    delay(1000);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(CS_SD, LOW);
    myFile = SD.open("DATA.txt", FILE_WRITE);
    if (myFile) {
      Serial.println("File opened ok");
      lcd.clear();
      lcd.print("File opened ok");
      delay(2000);
      myFile.println("Ashish .............. PRESENT");
      lcd.setCursor(0, 1);
      lcd.print("Written on SD...");
      delay(2000);
      lcd.clear();
      myFile.close();
    } else {
      Serial.println("error opening data.txt");
      lcd.clear();
      lcd.print("error opening data.txt");
    }
    digitalWrite(CS_SD, HIGH);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Put Your Card to");
    lcd.setCursor(0, 1);
    lcd.print("the Reader......");

  }

  else if (content.substring(1) == "33 8D 28 96")  //change here the UID of the card/cards that you want to give access
  {
    digitalWrite(buzzerPin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Name : ");
    lcd.setCursor(0, 1);
    lcd.print("Roll No : ");
    lcd.setCursor(7, 0);
    lcd.print("Abhishek");
    lcd.setCursor(11, 1);
    lcd.print("003");
    delay(1000);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(CS_SD, LOW);
    myFile = SD.open("DATA.txt", FILE_WRITE);
    if (myFile) {
      Serial.println("File opened ok");
      lcd.clear();
      lcd.print("File opened ok");
      delay(2000);
      myFile.println("Abhishek .............. PRESENT");
      lcd.setCursor(0, 1);
      lcd.print("Written on SD...");
      delay(2000);
      lcd.clear();
      myFile.close();
    } else {
      Serial.println("error opening data.txt");
      lcd.clear();
      lcd.print("error opening data.txt");
    }
    digitalWrite(CS_SD, HIGH);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Put Your Card to");
    lcd.setCursor(0, 1);
    lcd.print("the Reader......");

  } else if (content.substring(1) == "C0 59 44 32")  //change here the UID of the card/cards that you want to give access
  {
    digitalWrite(buzzerPin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Name : ");
    lcd.setCursor(0, 1);
    lcd.print("Roll No : ");
    lcd.setCursor(7, 0);
    lcd.print("Vaidehi");
    lcd.setCursor(11, 1);
    lcd.print("004");
    delay(1000);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(CS_SD, LOW);
    myFile = SD.open("DATA.txt", FILE_WRITE);
    if (myFile) {
      Serial.println("File opened ok");
      lcd.clear();
      lcd.print("File opened ok");
      delay(2000);
      myFile.println("Vaidehi .............. PRESENT");
      lcd.setCursor(0, 1);
      lcd.print("Written on SD...");
      delay(2000);
      lcd.clear();
      myFile.close();
    } else {
      Serial.println("error opening data.txt");
      lcd.clear();
      lcd.print("error opening data.txt");
    }
    digitalWrite(CS_SD, HIGH);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Put Your Card to");
    lcd.setCursor(0, 1);
    lcd.print("the Reader......");

  }

  else if (content.substring(1) == "19 A2 F6 E2")  //change here the UID of the card/cards that you want to give access
  {
    digitalWrite(buzzerPin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Name : ");
    lcd.setCursor(0, 1);
    lcd.print("Roll No : ");
    lcd.setCursor(7, 0);
    lcd.print("Kajal");
    lcd.setCursor(11, 1);
    lcd.print("005");
    delay(1000);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(CS_SD, LOW);
    myFile = SD.open("DATA.txt", FILE_WRITE);
    if (myFile) {
      Serial.println("File opened ok");
      lcd.clear();
      lcd.print("File opened ok");
      delay(2000);
      myFile.println("Kajal .............. PRESENT");
      lcd.setCursor(0, 1);
      lcd.print("Written on SD...");
      delay(2000);
      lcd.clear();
      myFile.close();
    } else {
      Serial.println("error opening data.txt");
      lcd.clear();
      lcd.print("error opening data.txt");
    }
    digitalWrite(CS_SD, HIGH);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Put Your Card to");
    lcd.setCursor(0, 1);
    lcd.print("the Reader......");
  }


  else {
    digitalWrite(buzzerPin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ID : ");
    lcd.print("Unknown");
    lcd.setCursor(0, 1);
    lcd.print("Access denied");
    Serial.println(" Access denied");
    delay(4000);
    digitalWrite(buzzerPin, LOW);
    lcd.clear();
  }
}