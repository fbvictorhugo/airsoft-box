/*
  by Victor Hugo
  v3.0.0
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define pushed LOW
#define unpushed HIGH

const int YELLOW_BTN = A1;
const int YELLOW_LED = A2;
const int BLUE_BTN = 11;
const int BLUE_LED = 10;
const int BUZZ = 12;
const int LED_CONFIG = 13;

const long CONFIG_BLINK = 200;

const char KEY_ENTER = '#';
const char KEY_DEL = '*';

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
Keypad cutomKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

enum Teams {
  NONE,
  BLUE,
  YELLOW
};

enum GameType {
  LAST_DOMINATION,
  DOMINATION_POITS,
  BOMB,
  BOMB_2FA
};

enum GameState {
  MENU_GAME,
  CONFIG_GAME,
  IN_GAME
};

enum BombState {
  BOMB_OFF,
  BOMB_ACTIVE,
  BOMB_DEFUSED,
  BOMB_PAUSED,
  BOMB_EXPLODED
};

unsigned long started = 0;
unsigned long captureTime = 0;
long GAME_TIME = 10000;
bool capturing = false;
int lastDomination = NONE;
bool ledBlink = false;

GameState gameState = MENU_GAME;
char key;
int modoJogo = -1;
BombState bombState = BOMB_OFF;

int btnBlueActive = 0;
int btnYellowActive = 0;

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
  delay(3000);
  writeLcd("Selecione o jogo", " ");
}

void loop() {
  loop_menu();
}

void loop_menu() {
  if (gameState == MENU_GAME) {
    key = cutomKeypad.getKey();

    if (!isEmpty(String(key))) {
      if (isDigit(key)) {
        modoJogo = String(key).toInt();
        writeLcd("", String(key) + ": " + getNomeJogo(modoJogo));

      } else if (key == KEY_ENTER) {

        writeLcd("Selecionado", getNomeJogo(modoJogo));
        gameState = IN_GAME;
        delay(2000);
        clearDisplay();

      } else if (key == KEY_DEL) {
        modoJogo = -1;
        writeLcd("Selecione o jogo", " ");
      }
    }

  } else if (gameState == IN_GAME) {
    loopModoJogo(modoJogo);
  }
}

void loopModoJogo(int modo) {

  switch (modo) {
    case 1:
      loop_LastDomination();
      break;
    case 3:
      loop_BombPassword();
      break;
    case 4:
      loop_Bomb2FA();
      break;
    default:
      gameIndisponivel();
      break;

  }
}

void reset() {
  lastDomination = NONE;
  started = 0;
  capturing = false;
  captureTime = 0;
}

void gameIndisponivel() {
  writeLcd("Indisponivel", " ");
  delay(3000);
  writeLcd("Selecione o jogo", " ");
  gameState = MENU_GAME;
}

String getNomeJogo(int modoJogo) {
  switch (modoJogo) {
    case 0:
      return "Modo Aleatorio";
      break;
    case 1:
      return "Last Domination";
      break;
    case 2:
      return "Domination Points";
      break;
    case 3:
      return "Bomb PWD";
      break;
    case 4:
      return "Bomb 2FA";
      break;
    default:
      return " ";
  }
}

/// ---------------------------------------------------------------------------

void clearDisplay() {
  writeLcd(" ", " ");
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

String alignText(String text, char align) {
  int remaning =  16 - text.length();
  if (align == 'C') {
    remaning = remaning / 2;
  } else if (align == 'R' ) {
    remaning = remaning;
  } else {
    return text;
  }
  String preText = "";
  for (int i = 0; i < remaning; i++) {
    preText += " ";
  }
  return preText + text;
}
