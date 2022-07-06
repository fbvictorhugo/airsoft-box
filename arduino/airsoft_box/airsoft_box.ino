/*
  by Victor Hugo
  v4.0.0
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

char hexaKeys[4][4] = {
  {'D', 'C', 'B', 'A'},
  {'#', '9', '6', '3'},
  {'0', '8', '5', '2'},
  {'*', '7', '4', '1'}
};

// ARDUINO NANO ----------------------
byte rowPins[4] = {9, 8, 7, 6};
byte colPins[4] = {5, 4, 3, 2};

Keypad mKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, 4, 4);
LiquidCrystal_I2C lcd(0x3F, 16, 2);
//----------------------------------

const int YELLOW_BTN = A1;
const int BLUE_BTN = 11;

const char KEY_ENTER = '#';
const char KEY_DEL = '*';

enum PinLed {
  ledBlue = 10,
  ledYellow = A2
};

enum PinButton {
  btnBlue = 11,
  btnYellow = A1
};

enum Teams {
  NONE,
  BLUE,
  YELLOW
};

enum GameState {
  MENU_GAME,
  IN_GAME
};

enum BombState {
  BOMB_CONFIG,
  BOMB_OFF,
  BOMB_ACTIVE,
  BOMB_DEFUSED,
  BOMB_PAUSED,
  BOMB_EXPLODED
};


enum MenuConfig {
  CFG_BOMB_TIME,
  CFG_CAPTURE_TIME
};

const int t_wait_menu = 5000;
const int t_wait_device = 3000;

int BLUE_READ = -1;
int YELLOW_READ = -1;

GameState gameState = MENU_GAME;
BombState bombState = BOMB_CONFIG;

char key;
int modoJogo = -1;
String passToArm = "";
String passToDisarm = "";
String configBombTime = "";
String configCaptureTime = "";
int progress = 0;
int progressMenu = 0;
unsigned long bombStarted = 0;
unsigned long bombFinished = 0;
unsigned long captureTime = 0;
bool hasPassToArm = true;

// ==============================================
//    PROGRAM
// ==============================================

void pinModes() {
  pinMode(ledBlue, OUTPUT);
  pinMode(ledYellow, OUTPUT);
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  randomSeed(analogRead(0));//for generate real random

  pinModes();
  writeLcd(alignText("AIRSOFT BOX", 'C'), alignText("- VH TACTICAL -", 'C'));
  delay(1500);

  writeLcd("Selecione o jogo",  " ");
}

void loop() {
  loop_menu();
}

void loop_menu() {
  if (gameState == MENU_GAME) {
    key = mKeypad.getKey();

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
      loop_BombPWD();
      break;
    case 4:
      loop_Bomb2FA();
      break;
    case 5:
      gameIndisponivel();
      break;
    case 6:
      loop_Genius();
      break;
    case 7:
      loop_Capacitor();
      break;
    default:
      gameIndisponivel();
      break;
  }
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
      return "Bomba2FA";
      break;
    case 5:
      return "Crypto";
      break;
    case 6:
      return "Genius";
      break;
    case 7:
      return "Capacitor";
      break;
    default:
      return " ";
  }
}

void gameIndisponivel() {
  writeLcd("Indisponivel", " ");
  delay(3000);
  writeLcd("Selecione o jogo", " ");
  gameState = MENU_GAME;
}

void returnToMenu() {
  gameState = MENU_GAME;
  bombState = BOMB_CONFIG;
  // clean
  key = ' ';
  modoJogo = -1;
  passToArm = "";
  passToDisarm = "";
  configBombTime = "";
  configCaptureTime = "";
  progress = 0;
  progressMenu = 0;
  bombStarted = 0;
  bombFinished = 0;
  captureTime = 0;
  hasPassToArm = true;

  writeLcd("Selecione o jogo",  " ");
}

bool isBtnsConfirm(char key) {
  if (key == KEY_ENTER || BLUE_READ == LOW || YELLOW_READ == LOW) {
    return true;
  } else {
    return false;
  }
}

void showMenuDeviceConfig(int configs[]) {
  int cgfSize = (sizeof(configs) / sizeof(int));
  // Serial.println("SIZE: " + String(cgfSize ));

  for (int i = 0; i < cgfSize ; i) {
    key = mKeypad.getKey();

    switch (configs[i]) {
      case CFG_BOMB_TIME:

        writeLcd("Configure tempo", "");
        lcd.setCursor(0, 1);
        lcd.print("Minutos:");

        if (isDigit(key)) {
          configBombTime += String(key);
          writeLcd("", "Minutos: " + String(configBombTime));
        } else if (key == KEY_ENTER) {
          i++;
        } else if (key == KEY_DEL) {
          configBombTime = "";
          writeLcd("Configure tempo", "Minutos:");
        }

        break;

      //----
      case CFG_CAPTURE_TIME:

        writeLcd("Configure Captura", "");
        lcd.setCursor(0, 1);
        lcd.print("Minutos:");

        if (isDigit(key)) {
          configBombTime += String(key);
          writeLcd("", "Minutos: " + String(configBombTime));
        } else if (key == KEY_ENTER) {
          i++;
        } else if (key == KEY_DEL) {
          configBombTime = "";
          writeLcd("Configure tempo", "Minutos:");
        }

        break;
    }
  }

}
