#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN  10
#define RST_PIN  9

uint8_t heart[8] = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ── Fix 1: NUM_CARDS matches actual number of entries (4) ──
const int NUM_CARDS = 5;

String authorizedUIDs[NUM_CARDS] = {
  "49:BB:49:00",
  "B7:A7:14:05",
  "49:15:34:00",
  "27:64:AA:04",
  "81:41:9D:7B"
};

String cardNames[NUM_CARDS] = {
  "Korir",
  "Bob",
  "Charlie",
  "Collins"   // Fix 2: added missing 4th name
};

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  lcd.init();
  lcd.backlight();
  lcd.createChar(3, heart);

  lcd.setCursor(0, 0);
  lcd.print("Hello...");
  lcd.write(3);
  lcd.setCursor(0, 1);
  lcd.print("Scan your card");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Build UID string
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) uid += ":";
  }
  uid.toUpperCase();

  Serial.println("UID: " + uid);

  // Look up name
  String name = "";
  for (int i = 0; i < NUM_CARDS; i++) {
    if (uid == authorizedUIDs[i]) {
      name = cardNames[i];
      break;
    }
  }


  lcd.clear();
  lcd.setCursor(0, 0);

  if (name != "") {
    lcd.print("Hello!");
    lcd.setCursor(0, 1);
    lcd.print(name);
    Serial.println("Name: " + name);
  } else {
    lcd.print("Unidentified");
    lcd.setCursor(0, 1);
    lcd.print("Please Register");
    Serial.println(uid+" is not Registered");
  }

  delay(5000);

  // Return to ready state
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello...");
  lcd.write(3);
  lcd.setCursor(0, 1);
  lcd.print("Scan your card");

  rfid.PICC_HaltA();
}
