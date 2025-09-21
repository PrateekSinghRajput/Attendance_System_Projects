#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <LiquidCrystal_I2C.h>
#include <HardwareSerial.h>

#define RST_PIN 15
#define SS_PIN 2
#define BUZZER 14

MFRC522 mfrc522(SS_PIN, RST_PIN);

const String sheet_url = "https://script.google.com/macros/s/Al2_lq0ttWyJ75RqFdg2VxzX5eiEyx3ujnyk-Ga3WzFlmp04dG3iI4LA/exec?";

#define WIFI_SSID "Prateek"
#define WIFI_PASSWORD "12345@#12345"

LiquidCrystal_I2C lcd(0x27, 16, 2);
HardwareSerial GSM(2);
#define GSM_TX_PIN 17
#define GSM_RX_PIN 16

struct Student {
  String uid;
  String name;
  String phone;
  bool inside;
};

Student students[] = {
  { "0DB90805", "Prateek", "+9188305848xx", false },
  { "47460B05", "Alice", "+9188305848xx", false },
  { "A656833D", "Bob", "+9188305848xx", false },
  // Add more as needed
};

void sendATCommand(String cmd, unsigned long timeout = 1000);
void initGSM();
void sendSMS(String phoneNumber, String name, String status);
String getPhoneNumber(String name);
String getCardUID();
void sendDataToSheet(String name, String status);
void buzzerBeep(int times);
String getCurrentTime();

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(BUZZER, OUTPUT);

  GSM.begin(9600, SERIAL_8N1, GSM_RX_PIN, GSM_TX_PIN);

  lcd.init();
  lcd.backlight();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  lcd.clear();
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP().toString());

  Serial.println("\nWiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(2000);
  lcd.clear();
  lcd.print("Scan RFID Card");

  initGSM();
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String cardUID = getCardUID();
  Serial.print("Card UID: ");
  Serial.println(cardUID);

  String cardHolderName = "";
  String phone = "";
  String statusStr = "";
  bool found = false;

  for (int i = 0; i < sizeof(students) / sizeof(Student); i++) {
    if (students[i].uid.equalsIgnoreCase(cardUID)) {
      cardHolderName = students[i].name;
      phone = students[i].phone;
      found = true;

      if (students[i].inside) {
        statusStr = "OUT";
        students[i].inside = false;
      } else {
        statusStr = "IN";
        students[i].inside = true;
      }
      break;
    }
  }

  if (!found) {
    lcd.clear();
    lcd.print("Unknown Card");
    buzzerBeep(1);
    delay(2000);
    lcd.clear();
    lcd.print("Scan RFID Card");
    return;
  }

  Serial.print("Card Holder Name: ");
  Serial.println(cardHolderName);
  Serial.print("Status: ");
  Serial.println(statusStr);

  lcd.clear();
  lcd.print(cardHolderName);
  lcd.setCursor(0, 1);
  lcd.print(statusStr);
  buzzerBeep(2);

  sendDataToSheet(cardHolderName, statusStr);

  if (phone != "") {
    sendSMS(phone, cardHolderName, statusStr);
  } else {
    Serial.println("Phone number not found for " + cardHolderName);
  }

  delay(3000);
  lcd.clear();
  lcd.print("Scan RFID Card");
}

String getCardUID() {
  String uidStr = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      uidStr += "0";
    }
    uidStr += String(mfrc522.uid.uidByte[i], HEX);
  }
  uidStr.toUpperCase();
  return uidStr;
}

void sendDataToSheet(String name, String status) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return;
  }

  WiFiClientSecure client;
  client.setInsecure();

  String url = sheet_url + "name=" + name + "&status=" + status;
  Serial.print("Sending to URL: ");
  Serial.println(url);

  HTTPClient https;
  if (https.begin(client, url)) {
    int httpCode = https.GET();
    if (httpCode > 0) {
      Serial.printf("HTTP GET code: %d\n", httpCode);
    } else {
      Serial.printf("HTTP GET failed: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  } else {
    Serial.println("Unable to connect to server");
  }
}

void buzzerBeep(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(150);
    digitalWrite(BUZZER, LOW);
    delay(150);
  }
}

void initGSM() {
  Serial.println("Initializing GSM module...");
  delay(1000);
  sendATCommand("AT");
  sendATCommand("ATE0");
  sendATCommand("AT+CMGF=1");
  sendATCommand("AT+CNMI=2,2,0,0,0");
  Serial.println("GSM module ready.");
}

void sendATCommand(String cmd, unsigned long timeout) {
  GSM.println(cmd);
  unsigned long timeStart = millis();
  while (millis() - timeStart < timeout) {
    while (GSM.available()) {
      String response = GSM.readStringUntil('\n');
      response.trim();
      Serial.println("GSM Response: " + response);
      if (response.indexOf("OK") != -1 || response.indexOf(">") != -1) {
        return;
      }
    }
  }
  Serial.println("No response or timeout for command: " + cmd);
}

void sendSMS(String phoneNumber, String name, String status) {
  Serial.println("Sending SMS to " + phoneNumber);
  String message = "Hello " + name + ", you have checked " + status + " at " + getCurrentTime();

  sendATCommand("AT+CMGS=\"" + phoneNumber + "\"");
  delay(500);
  GSM.print(message);
  delay(500);
  GSM.write(26);
  delay(5000);

  Serial.println("SMS sent: " + message);
}

String getCurrentTime() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  char buffer[9];
  sprintf(buffer, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
  return String(buffer);
}