# Arduino-UNO RFID-LCD-Access Control System

An Arduino UNO project that reads RFID cards using the RC522 module and displays the cardholder's name on a 16x2 I2C LCD display. Unknown cards are flagged on screen.

---

## Features

- Scans MIFARE Classic RFID cards/fobs
- Matches UID to a stored name and displays a greeting
- Shows "Unknown Card" with UID for unregistered cards
- Custom heart character on the idle screen
- Serial monitor output for debugging
- Supports reading pre-written block data from cards

---

## Hardware Required

| Component | Quantity |
|-----------|----------|
| Arduino UNO | 1 |
| RC522 RFID Module | 1 |
| 16x2 I2C LCD Display | 1 |
| RFID Cards / Key Fobs | As needed |
| Jumper Wires | Several |

---

## Wiring

### RC522 → Arduino UNO

| RC522 Pin | Arduino Pin |
|-----------|-------------|
| SDA (SS)  | 10          |
| SCK       | 13          |
| MOSI      | 11          |
| MISO      | 12          |
| RST       | 9           |
| GND       | GND         |
| 3.3V      | 3.3V ⚠️     |

> ⚠️ The RC522 runs on **3.3V only**. Do not connect to 5V.

### LCD I2C → Arduino UNO

| LCD Pin | Arduino Pin |
|---------|-------------|
| GND     | GND         |
| VCC     | 5V          |
| SDA     | A4          |
| SCL     | A5          |

---

## Libraries

Install the following via Arduino IDE → Library Manager:

- [MFRC522](https://github.com/miguelbalboa/rfid) by GithubCommunity
- [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C) by Frank de Brabander

---

## Setup

1. Clone this repository
   ```bash
   git clone https://github.com/YOUR_USERNAME/arduino-rfid-lcd-access.git
   ```

2. Open `RFID-LCD_Detector.ino` in Arduino IDE

3. Find your LCD I2C address by running the I2C scanner sketch (see `extras/i2c_scanner.ino`). Common addresses are `0x27` or `0x3F`

4. Update the address in the sketch if needed:
   ```cpp
   LiquidCrystal_I2C lcd(0x27, 16, 2);
   ```

5. Add your card UIDs and names:
   ```cpp
   const int NUM_CARDS = 4;

   String authorizedUIDs[NUM_CARDS] = {
     "49:BB:49:00",
     "B7:A7:14:05",
     "49:15:34:00",
     "27:64:AA:04"
   };

   String cardNames[NUM_CARDS] = {
     "Korir",
     "Bob",
     "Charlie",
     "Collins"
   };
   ```
   > To find your card's UID, upload the sketch and open Serial Monitor at 9600 baud, then scan the card.

6. Upload to Arduino UNO and open Serial Monitor at **9600 baud**

---

## LCD Behaviour

| State        | Line 1         | Line 2           |
|--------------|----------------|------------------|
| Idle         | `Hello... ♥`   | `Scan your card` |
| Known card   | `Hello!`        | `Korir`          |
| Unknown card | `Unknown Card`  | `A1:B2:C3:D4`    |

---

## File Structure

```
arduino-rfid-lcd-access/
├── RFID-LCD_Detector/
│   └── RFID-LCD_Detector.ino   # Main sketch
├── extras/
│   └── i2c_scanner.ino         # Helper to find LCD I2C address
└── README.md
```

---

## How to Get Your Card UIDs

1. Upload the sketch
2. Open **Serial Monitor** at 9600 baud
3. Scan each card — the UID will print e.g. `UID: 49:BB:49:00`
4. Copy each UID into the `authorizedUIDs[]` array

---

## License

MIT License — free to use and modify.
