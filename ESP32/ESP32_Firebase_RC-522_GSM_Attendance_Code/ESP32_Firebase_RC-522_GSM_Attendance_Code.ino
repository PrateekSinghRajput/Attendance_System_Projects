#include <SoftwareSerial.h>
#include <FirebaseESP32.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);

#define FIREBASE_HOST "attendance-ed3eblt-rtdb.firebaseio.com"
#define RXD2 16
#define TXD2 17
#define SS_PIN 2
#define RST_PIN 15

#define WIFI_SSID "Prateek"
#define WIFI_PASSWORD "12345s@#12345"

// Firebase configuration
const char* FIREBASE_PROJECT_ID = "attendance-ed3eb";
const char* FIREBASE_CLIENT_EMAIL = "firebase-adminsvc@attendance-ed3eb.iam.gserviceaccount.com";
const char* FIREBASE_PRIVATE_KEY = "-----BEGIN PRIVATE KEY-----\nMIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoz/OP7N796s\nLZYX3L/Z7A6aLhSzOcPIbRuYdRryPVrqlxY5xL4JPAZYR0jDFH4aaLvXWsYbaVV6\nMp+UIzuumqJ/kWcSXvQLEOhVT1eiNxPa5h7dfnVQi12lnwh+AZOYBqG4rs09KsNZ\nIrFNknKWc0S2fW6w4Q7oeio1nRZcQTNw4H3t1SvzbBcl9D3X4UYXjKSZ7SKkernU\nImRArtlqRTgjxZug4o66FwXk58vfiwEtmGj0bpiSuT8LPeS2ipCqGSaU4ic9EATE\nR4jCUWU8r1iqK4zRFPesfmtgpZO3utj+LILJW4p2Oj5am/YTxEnpdG8KFVyTERXZ\ncPdho4J5AgMBAAECggEAHd/25NBurNY8nw3rSHqbLO2jyMaYmTGYW9ouHhSEVCff\nQeBcjLJseGTWpUqlFVKVwtovS52FI5Z7qGmP9dTSuBkfAVGu1PqRetHK5Xqe2Qlw\nT5gEWP18YPa51/Qn1RQvQ1G04lS9RFzTEpAGGw6NNqI0kj5gIHyjRHcQgeQhOZD4\nUwDLzyD1F+GcQ7ywnmQaOAVcZL+r18oBj6+NYjCie/vab0x3SWJ89h8WZpXWs5Is\nCzW88qWTeUvh6NrzfixNp7JzAn/nU6UPKWenIu32KvwyG7NfqwIv3TbAruyLNWwf\nWPjQ5CN+E4rS3Zh8nAKqnF3KPhc1hcmPjKYRP+eOzQKBgQDoRuRl1QPlnEB/Dj6i\nWzqHvnd++8robAxMn9G+dvkplxadJvd+N04HrmD+kbgRv06DBvmTbRH6BOCIToxO\nhG07ZvdRsM3d4xXXPjRGjolAVu1kWUENDTAyjvcViYLx7e0XFoqokbZ4nr7E6Cyo\noHjIXlLzoOCCDwqu5GUVQ7SwLQKBgQC4+avNkyWuAp/TbFK+pVZUqQDF9XjCJWMD\nYL1FknPueV825oL5irMSgEvJ9s+afqqm7S4Hrh2K4MoLefJjC7MXlkRUVdB+GB5b\nk0HL8lR3dETMACvbwQWmuAKtVcRZg7o4iP+FN00+JaavcBykzmtUGHTA/o6OGAMP\nKGe31JC+/QKBgQDXUzDUPf6Y0prvrfLea5f80tLQAvd9MvzL47R6zqv4pDmwgbkZ\nZoeNk1gTZl6vBRvxV3vx8UCi9/Z2JernttfUI4XOUwrlUWij2uR3LBeMb4kuZ5do\nU/uEQwhHDO04/Ym2x4zO7BWuB+ttepY71Fdj2B0qAM9fgjYzH0OdzOAqGQKBgQCJ\nE0xDnYXQu9Ug4ic3bGLbfho6mLwSc446VJguVGdhE8SBdLUeo7AsR7LiSepyvOdN\nqHnx6w+teQOBGa+LwoHHOu4L6UQGDW2Bw1CBCGWb2CebXY0i3pmHgNlLBkHpa/q2\nJp/kYqT1JwWtANHZN7bFrRHoazvNBPJPa0vhKL7TKQKBgHkvv23sLda7UeXFdavS\nzw0WdHD5XutnYnlBjoCr/Q/OOYJ7uB93SEVHtuwmX4xKs+2M+6N1VB29F3uyEfw7\n1DxSf0Ho9MNSJ9qwaWaln7AjrEIIkXIzZ+EuaerxNKVEAubjhQM9cyw3QzbcnZvt\nPhCUoeSRTWMctNDu5purTZDz\n-----END PRIVATE KEY-----\n";
const char* FIREBASE_DATABASE_URL = "https://attendance-ed3eb-default-rtdb.firebaseio.com/";

struct User {
  String rfid;
  String name;
  String phoneNumber;
};


const User authorizedUsers[] = {
  { "e339aed9", "Prateek", "+9188305848xx" },
  { "3fd5e2", "Rohan", "+9188305848xx" },
  { "373b113", "Shyam", "+9188305848xx" },
  { "735ec01c", "Emily", "+9188305848xx" },
  { "q7r8s9t0", "Michael", "+9188305848xx" }
};

SoftwareSerial sim800l(RXD2, TXD2);
FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup...");

  sim800l.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  Serial.println("Initialized SIM800L, RFID, and LCD");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  int wifiAttempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wifiAttempts++;
    if (wifiAttempts > 20) {
      Serial.println("\nWiFi connection failed!");
      lcd.clear();
      lcd.print("WiFi Failed!");
      delay(2000);
      ESP.restart();
    }
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
  timeClient.update();

  config.service_account.data.client_email = FIREBASE_CLIENT_EMAIL;
  config.service_account.data.project_id = FIREBASE_PROJECT_ID;
  config.service_account.data.private_key = FIREBASE_PRIVATE_KEY;
  config.database_url = FIREBASE_DATABASE_URL;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  if (Firebase.ready()) {
    Serial.println("Firebase connected!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System Ready");
  } else {
    Serial.println("Firebase connection failed!");
    lcd.clear();
    lcd.print("Firebase Error");
  }

  initializeGSM();
}

void loop() {
  if (!Firebase.ready()) {
    Serial.println("Firebase not ready");
    lcd.clear();
    lcd.print("Firebase Error");
    delay(2000);
    return;
  }

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String tagID = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      tagID += String(rfid.uid.uidByte[i], HEX);
    }
    rfid.PICC_HaltA();

    Serial.println("\nRFID Tag: " + tagID);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RFID: " + tagID);

    int userIndex = -1;
    for (int i = 0; i < 5; i++) {
      if (tagID.equalsIgnoreCase(authorizedUsers[i].rfid)) {
        userIndex = i;
        break;
      }
    }

    if (userIndex != -1) {
      String userName = authorizedUsers[userIndex].name;
      String userPhone = authorizedUsers[userIndex].phoneNumber;

      Serial.println("User: " + userName);
      lcd.setCursor(0, 1);
      lcd.print("Welcome " + userName);
      timeClient.update();
      String currentTime = getFormattedTime();
      String currentDate = getFormattedDate();

      String path = "/attendance/" + authorizedUsers[userIndex].rfid + "/" + String(timeClient.getEpochTime());

      FirebaseJson json;
      json.set("name", userName);
      json.set("time", currentTime);
      json.set("date", currentDate);
      json.set("status", "Present");

      if (Firebase.setJSON(fbdo, path, json)) {
        Serial.println("Data sent to Firebase");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(userName);
        lcd.setCursor(0, 1);
        lcd.print("Attendance marked");

        String smsMessage = userName + " attendance\nTime: " + currentTime + "\nDate: " + currentDate;
        sendSMS(userPhone, smsMessage);
      } else {
        Serial.println("Firebase error: " + fbdo.errorReason());
        lcd.setCursor(0, 1);
        lcd.print("Error sending data");
      }
    } else {
      Serial.println("Unauthorized card!");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unauthorized!");
      lcd.setCursor(0, 1);
      lcd.print("Access Denied");

      String smsMessage = "Unauthorized access\nRFID: " + tagID + "\nTime: " + getFormattedTime();
      sendSMS(authorizedUsers[0].phoneNumber, smsMessage);
    }
    delay(2000);
  }
  delay(100);
}

String getFormattedDate() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm* ptm = gmtime(&epochTime);

  return String(ptm->tm_year + 1900) + "-" + String(ptm->tm_mon + 1) + "-" + String(ptm->tm_mday);
}

String getFormattedTime() {
  timeClient.update();
  time_t rawTime = timeClient.getEpochTime();
  struct tm* ti = localtime(&rawTime);

  return String(ti->tm_hour) + ":" + String(ti->tm_min) + ":" + String(ti->tm_sec);
}

void initializeGSM() {
  Serial.println("Initializing GSM...");
  sim800l.println("AT");
  delay(1000);
  sim800l.println("AT+CMGF=1");
  delay(1000);
  sim800l.println("AT+CNMI=1,2,0,0,0");
  delay(1000);
  Serial.println("GSM Ready");
}

void sendSMS(String phoneNumber, String message) {
  Serial.println("Sending to: " + phoneNumber);
  sim800l.println("AT+CMGS=\"" + phoneNumber + "\"");
  delay(1000);
  sim800l.print(message);
  delay(1000);
  sim800l.write(26);  // CTRL+Z to send
  delay(1000);
  Serial.println("SMS Sent");
}