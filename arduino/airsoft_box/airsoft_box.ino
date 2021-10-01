/*
  by Victor Hugo
  v3.0.0
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const int YELLOW_BTN = A1;
const int YELLOW_LED = A2;
const int BLUE_BTN = 10;
const int BLUE_LED = 11;
const int BUZZ = 12;
const int LED_CONFIG = 13;

const int TONE = 528;
const long CONFIG_BLINK = 200;

const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'D', 'C', 'B', 'A'},
  {'#', '9', '6', '3'},
  {'0', '8', '5', '2'},
  {'*', '7', '4', '1'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

LiquidCrystal_I2C lcd(0x3F, 16, 2);
Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

enum Teams {
  NONE,
  BLUE,
  YELLOW
};

enum GameType {
  LAST_DOMINATION,
  DOMINATION_POITS,
  SECURE,
  BOMB
};

enum GameState {
  INTRO,
  MENU_GAME,
  PRE_GAME,
  IN_GAME,
  END_GAME
};


unsigned long started = 0;
unsigned long captureTime = 0;
long GAME_TIME = 60000;
bool capturing = false;
int lastDomination = NONE;
bool ledBlink = false;


// ==============================================
//    PROGRAM
// ==============================================

void pinModes() {
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(LED_CONFIG, OUTPUT);
  pinMode(BUZZ, OUTPUT);
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinModes();
  writeLcd("  AIRSOFT BOX", " --- ELITE --- ");
}

void loop() {

  long diff = captureTime - started;

  int BLUE_READ = digitalRead(BLUE_BTN);
  int YELLOW_READ = digitalRead(YELLOW_BTN);
  ledBlink = diff % 100;

  if (BLUE_READ == LOW ) {
    capture(BLUE);
  } else if (YELLOW_READ == LOW) {
    capture(YELLOW);
  } else {
    writeLcd("   DOMINATION", " --- ELITE --- ");
    reset();
  }

  if ( diff >= GAME_TIME) {
    if (lastDomination == BLUE) {
      dominationBLUE();
    } else if (lastDomination == YELLOW) {
      dominationYELLOW();
    }
    started = 0;
    captureTime = 0;
  }
}

void capture(Teams team) {
  captureTime = millis();

  if (lastDomination != team) {
    reset();
    lastDomination = team;
  } else {
    writeLcd("Capturando ...", " ");
    if (!capturing) {
      started = millis();
    }
    capturing = true;
    if (ledBlink) {
      ledBlinkTeam(team);
    }
  }
}

void reset() {
  lastDomination = NONE;
  started = 0;
  capturing = false;
  captureTime = 0;
}

void ledBlinkTeam(Teams team) {
  switch (team) {
    case BLUE:
      ledBlinkBlue();
      break;
    case YELLOW:
      ledBlinkYellow();
      break;
  }
}

/// ---------------------------------------------------------------------------
void ledBlinkYellow() {
  digitalWrite(YELLOW_LED, HIGH); // LED ON
  delay(50);
  digitalWrite(YELLOW_LED, LOW); // LED ON
}

void ledBlinkBlue() {
  digitalWrite(BLUE_LED, HIGH); // LED ON
  delay(50);
  digitalWrite(BLUE_LED, LOW); // LED ON
}

void dominationBLUE() {
  digitalWrite(YELLOW_LED, LOW); // LED OFF
  digitalWrite(BLUE_LED, HIGH); // LED ON
}

void dominationYELLOW() {
  digitalWrite(BLUE_LED, LOW); // LED OFF
  digitalWrite(YELLOW_LED, HIGH); // LED ON
}

void writeLcd(String primary, String second) {
  if (!isEmpty(primary)) {
    lcd.setCursor(0, 0);
    lcd.print(fillText(primary));
  }
  if (!isEmpty(second)) {
    lcd.setCursor(0, 1);
    lcd.print(fillText(second));
  }
}

void playBuzz() {
  digitalWrite(BUZZ, HIGH);
}

void stopBuzz() {
  digitalWrite(BUZZ, LOW);
}

//------------------------------------------------------

bool isEmpty(String text) {
  return text.length() == 0;
}

String fillText(String text) {
  int remaning =  16 - text.length() ;
  if (remaning > 0) {
    for (int i = 0; i <= remaning; i++) {
      text += " ";
    }
  }
  return text;
}
