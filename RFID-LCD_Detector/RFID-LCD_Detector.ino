#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9

#define RELAY_PIN 7
#define GREEN_LED 6
#define RED_LED 5

uint8_t heart[8] = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int NUM_CARDS = 4;

String authorizedUIDs[NUM_CARDS] = {
  "49:BB:49:00",
  "B7:A7:14:05",
  "49:15:34:00",
  "27:64:AA:04"
};

String cardNames[NUM_CARDS] = {
  "John Doe",
  "June",
  "Charlie",
  "Korir"
};

void setup() {

  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  lcd.init();
  lcd.backlight();
  lcd.createChar(3, heart);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH); // relay OFF

  lcd.setCursor(0,0);
  lcd.print("Hello...");
  lcd.write(3);
  lcd.setCursor(0,1);
  lcd.print("Scan your card");
}

void loop() {

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  String uid = "";

  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) uid += ":";
  }

  uid.toUpperCase();
  Serial.println("UID: " + uid);

  String name = "";

  for (int i = 0; i < NUM_CARDS; i++) {
    if (uid == authorizedUIDs[i]) {
      name = cardNames[i];
      break;
    }
  }

  lcd.clear();
  lcd.setCursor(0,0);

  if (name != "") {

    // AUTHORIZED USER
    lcd.print("Hello!");
    lcd.setCursor(0,1);
    lcd.print(name);

    Serial.println("Authorized: " + name);

    digitalWrite(GREEN_LED, HIGH);

    // OPEN VALVE
    digitalWrite(RELAY_PIN, LOW);

    delay(5000); // dispensing time

    // CLOSE VALVE
    digitalWrite(RELAY_PIN, HIGH);

    digitalWrite(GREEN_LED, LOW);

  }

  else {

    // UNAUTHORIZED USER
    lcd.print("Unidentified");
    lcd.setCursor(0,1);
    lcd.print("Access Denied");

    Serial.println(uid + " not registered");

    digitalWrite(RED_LED, HIGH);
    delay(3000);
    digitalWrite(RED_LED, LOW);

  }

  delay(2000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hello...");
  lcd.write(3);
  lcd.setCursor(0,1);
  lcd.print("Scan your card");

  rfid.PICC_HaltA();
}