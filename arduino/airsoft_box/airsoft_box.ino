/*
  by Victor Hugo
  v2.0.0
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// ==============================================
//    YELLOW CONFIGURATIONS
// ==============================================

const int YELLOW_BTN = A1;
const int YELLOW_LED = A2;

// ==============================================
//    BLUE CONFIGURATIONS
// ==============================================

const int BLUE_BTN = 10;
const int BLUE_LED =  11;

// ==============================================
//    BUZZER CONFIGURATIONS
// ==============================================
const int BUZZ =  12;
const int TONE = 528;

// ==============================================
//    GAME CONFIGURATIONS
// ==============================================
const int LED_CONFIG =  13;
const long CONFIG_BLINK = 200;
int ledConfigState = 0;

// ==============================================
//    KEYBOARD
// ==============================================

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


// ==============================================
//    COMPONENTS INSTANCE
// ==============================================

LiquidCrystal_I2C lcd(0x3F, 20, 4);
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

int const PRE_GAME = 0;
int const STARTED = 1;
int const BLUE_DOMINATION = 2;
int const YELLOW_DOMINATION = 3;
int const BLUE_WIN = 4;
int const YELLOW_WIN = 5;
int const DRAW_GAME = 6;

int const INTRO = 7;
int const SELECT_DOM_TIME = 8;
int const START_DOM_OPT = 9;

unsigned long started = 0;
unsigned long previousMillis = 0;
long GAME_TIME = 1800000; //30min
long GAME_SAFE = 180000; //3min
unsigned long bluePoint = 0;
unsigned long yellowPoint = 0;
int currentStatus = 42;
bool DEMO_GAME = false;
bool IN_DOMI_GAME = false;

int lastDomination = 0;

// ==============================================
//    PROGRAM
// ==============================================

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(LED_CONFIG, OUTPUT);

  pinMode(BUZZ, OUTPUT);

  showLcdStatus(INTRO);

}

void loop() {

  char customKey = customKeypad.getKey();
  Serial.println(String("key: " + customKey));

  if (IN_DOMI_GAME == true) {
    DominationGame();
    //LastDomiGame();

    if ((currentStatus == BLUE_WIN || currentStatus == YELLOW_WIN || currentStatus == DRAW_GAME) && customKey) {
      IN_DOMI_GAME = false;
      showLcdStatus(INTRO);
    }

  } else {
    if (currentStatus == SELECT_DOM_TIME) {

      if (customKey == '1') {
        GAME_TIME = 900000;
        started = millis();
        IN_DOMI_GAME = true;

      } else if (customKey == '2') {
        GAME_TIME = 1800000;
        started = millis();
        IN_DOMI_GAME = true;

      } else if (customKey == '3') {
        GAME_TIME = 2400000;
        started = millis();
        IN_DOMI_GAME = true;

      } else if (customKey == '0') {
        DEMO_GAME = true;
        GAME_TIME = 30000;
        GAME_SAFE = 3000;
        started = millis();
        IN_DOMI_GAME = true;
      }

    } else if (customKey) {
      showLcdStatus(SELECT_DOM_TIME);
    }
  }

  //gameFlux();
}

void initialize() {
  started = 0;
  previousMillis = 0;
  GAME_TIME = 1800000; //30min
  GAME_SAFE = 180000; //3min
  bluePoint = 0;
  yellowPoint = 0;
  DEMO_GAME = false;
  IN_DOMI_GAME = false;
}


void LastDomiGame() {

  unsigned long currentMillis = millis();

  if (currentMillis - started <= GAME_SAFE) {
    showLcdStatus(PRE_GAME);

    if (currentMillis - previousMillis >= CONFIG_BLINK) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;

      // if the LED is off turn it on and vice-versa:
      if (ledConfigState == LOW) {
        ledConfigState = HIGH;
      } else {
        ledConfigState = LOW;
      }

      // set the LED with the ledState of the variable:
      digitalWrite(LED_CONFIG, ledConfigState);
    }

  } else {
    digitalWrite(LED_CONFIG, HIGH);

    int BLUE_READ = digitalRead(BLUE_BTN);
    int YELLOW_READ = digitalRead(YELLOW_BTN);

    if (currentMillis - started >= GAME_TIME + GAME_SAFE) {

      if (lastDomination == BLUE_DOMINATION) {

        winBLUE();

      } else if (lastDomination == YELLOW_DOMINATION) {

        winYELLOW();

      } else {

        drawGame();

      }

    } else {
      if (BLUE_READ == LOW) {
        dominationBLUE();
        lastDomination = BLUE_DOMINATION;
      } else if (YELLOW_READ == LOW) {
        dominationYELLOW();
        lastDomination = YELLOW_DOMINATION;
      } else {
        showLcdStatus(STARTED);
      }
    }
  }
}

void DominationGame() {

  //digitalWrite(LED_CONFIG, HIGH);
  unsigned long currentMillis = millis();

  if (currentMillis - started <= GAME_SAFE) {
    showLcdStatus(PRE_GAME);

    if (currentMillis - previousMillis >= CONFIG_BLINK) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;

      // if the LED is off turn it on and vice-versa:
      if (ledConfigState == LOW) {
        ledConfigState = HIGH;
      } else {
        ledConfigState = LOW;
      }

      // set the LED with the ledState of the variable:
      digitalWrite(LED_CONFIG, ledConfigState);
    }

  } else {

    digitalWrite(LED_CONFIG, HIGH);

    int BLUE_READ = digitalRead(BLUE_BTN);
    int YELLOW_READ = digitalRead(YELLOW_BTN);

    if (currentMillis - started >= GAME_TIME + GAME_SAFE) {

      if (bluePoint > yellowPoint) {

        winBLUE();

      } else if (yellowPoint > bluePoint) {

        winYELLOW();

      } else {

        drawGame();

      }

    } else {
      if (BLUE_READ == LOW) {
        dominationBLUE();
      } else if (YELLOW_READ == LOW) {
        dominationYELLOW();
      } else {
        showLcdStatus(STARTED);
      }
    }

  }
}

void dominationBLUE() {
  digitalWrite(YELLOW_LED, LOW); // LED OFF
  digitalWrite(BLUE_LED, HIGH); // LED ON
  bluePoint += 1;
  showLcdStatus(BLUE_DOMINATION);
}

void dominationYELLOW() {
  digitalWrite(BLUE_LED, LOW); // LED OFF
  digitalWrite(YELLOW_LED, HIGH); // LED ON
  yellowPoint += 1;
  showLcdStatus(YELLOW_DOMINATION);
}

void winBLUE() {

  //Serial.println("BLUE WINS!");
  showLcdStatus(BLUE_WIN);

  //Serial.println(String(getPercent(bluePoint)) + "% (" + String(bluePoint)
  //               + ") contra o YELLOW: " + String(getPercent(yellowPoint))
  //               + "% (" + String(yellowPoint) + ")." );

  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(BLUE_LED, HIGH);
  playBuzz();
  delay(500);
  digitalWrite(BLUE_LED, LOW);
  stopBuzz();
  delay(500);
}

void winYELLOW() {

  //Serial.print("YELLOW WINS! ");
  showLcdStatus(YELLOW_WIN);

  //Serial.println(String(getPercent(yellowPoint)) + "% (" + String(yellowPoint)
  //               + ") contra o BLUE: " + String(getPercent(bluePoint))
  //               + "% (" + String(bluePoint) + ")." );

  digitalWrite(BLUE_LED, LOW);
  digitalWrite(YELLOW_LED, HIGH);
  playBuzz();
  delay(500);
  digitalWrite(YELLOW_LED, LOW);
  stopBuzz();
  delay(500);
}

String getPercent(unsigned long actual ) {
  float total = yellowPoint + bluePoint;
  //Serial.println("Yellow: " + String(yellowPoint) + " Blue: " + String(bluePoint));
  float percent =  actual * 100 / total;

  int decimals = ExtractDecimalPart(percent);
  if (decimals == 00) {
    return String((int)percent );
  } else {
    return String(percent);
  }
}

int ExtractDecimalPart(float Value) {
  float temp = Value - (long)(Value);
  long p = 1;
  for (int i = 0; i < 2; i++) p *= 10;
  int DecimalPart = p * temp;
  return DecimalPart;
}

void drawGame () {
  //Serial.println("DRAW GAME !!!!");
  showLcdStatus(DRAW_GAME);

  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  playBuzz();
  delay(500);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  stopBuzz();
  delay(500);

}

void showLcdStatus() {
  showLcdStatus(currentStatus);
}
void showLcdStatus(int sts) {
  if (sts != currentStatus) {
    switch (sts) {
      case PRE_GAME:
        playBuzz();

        if (DEMO_GAME) {
          writeLcd("DEMONSTRATIVO", "Em pre jogo  .....");
        } else {
          writeLcd("Preparando Jogo", "Aguarde .....");
        }
        delay(100);

        stopBuzz();
        break;

      case STARTED:
        if (bluePoint == 0 && yellowPoint == 0) {
          if (DEMO_GAME) {
            writeLcd("DEMO Iniciada  !", "Aperte p capturar");
          } else {
            writeLcd("Jogo Iniciado  !", "Aperte p capturar");
          }

          digitalWrite(YELLOW_LED, HIGH);
          digitalWrite(BLUE_LED, HIGH);
          playBuzz();
          delay(300);
          digitalWrite(YELLOW_LED, LOW);
          digitalWrite(BLUE_LED, LOW);
          stopBuzz();
        }
        break;

      case BLUE_DOMINATION:
        writeLcd("Azul", "Dominando");
        break;

      case BLUE_WIN:
        writeLcd("Vencedor:", "AZUL: " + String(getPercent(bluePoint)) + "%");
        break;

      case YELLOW_WIN:
        writeLcd("Vencedor:", "AMARELO: " + String(getPercent(yellowPoint)) + "%");
        break;

      case YELLOW_DOMINATION:
        writeLcd("Amarelo", "Dominando");
        break;

      case DRAW_GAME:
        writeLcd("*    EMPATE    *", "");
        break;

      case SELECT_DOM_TIME:
        writeLcd("Minutos p/ Jogo", "1=15m 2=30m 3=40m");
        break;

      case INTRO:
        writeLcd("  AIRSOFT BOX", "--- Airsoft ---");
        initialize();
        break;

    }
  }
  currentStatus = sts;
}

void writeLcd(String primary, String second) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(primary);
  lcd.setCursor(0, 1);
  lcd.print(second);
}

void playBuzz() {
  //tone(BUZZ, TONE);
  digitalWrite(BUZZ, HIGH);
}

void stopBuzz() {
  //noTone(BUZZ);
  digitalWrite(BUZZ, LOW);
}
