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

const int YELLOW_BTN = 8;
const int YELLOW_LED =  9;

// ==============================================
//    RED CONFIGURATIONS
// ==============================================

const int RED_BTN = 10;
const int RED_LED =  11;

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
  {'D', '#', '0', '*'},
  {'C', '9', '8', '7'},
  {'B', '6', '5', '4'},
  {'A', '3', '2', '1'}
};
byte rowPins[ROWS] = {3, 2, 1, 0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad


// ==============================================
//    COMPONENTS INSTANCE
// ==============================================

LiquidCrystal_I2C lcd(0x3F, 20, 4);
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

int const PRE_GAME = 0;
int const STARTED = 1;
int const RED_DOMINATION = 2;
int const YELLOW_DOMINATION = 3;
int const RED_WIN = 4;
int const YELLOW_WIN = 5;
int const DRAW_GAME = 6;

int const INTRO = 7;
int const SELECT_DOM_TIME = 8;
int const START_DOM_OPT = 9;

unsigned long started = 0;
unsigned long previousMillis = 0;
long GAME_TIME = 1800000; //30min
long GAME_SAFE = 180000; //3min
unsigned long redPoint = 0;
unsigned long yellowPoint = 0;
int currentStatus = 42;
bool DEMO_GAME = false;
bool IN_DOMI_GAME = false;

int lastDomination = 0;

// ==============================================
//    PROGRAM
// ==============================================

void setup() {
  //Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(LED_CONFIG, OUTPUT);

  pinMode(BUZZ, OUTPUT);

  showLcdStatus(INTRO);

}

void loop() {

  char customKey = customKeypad.getKey();

  if (IN_DOMI_GAME == true) {
    //DominationGame();
    LastDomiGame();

    if ((currentStatus == RED_WIN || currentStatus == YELLOW_WIN || currentStatus == DRAW_GAME) && customKey) {
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
        GAME_TIME = 10000;
        GAME_SAFE = 5000;
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
  redPoint = 0;
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

    int RED_READ = digitalRead(RED_BTN);
    int YELLOW_READ = digitalRead(YELLOW_BTN);

    if (currentMillis - started >= GAME_TIME + GAME_SAFE) {

      if (lastDomination == RED_DOMINATION) {

        winRED();

      } else if (lastDomination == YELLOW_DOMINATION) {

        winYELLOW();

      } else {

        drawGame();

      }

    } else {
      if (RED_READ == LOW) {
        dominationRED();
        lastDomination = RED_DOMINATION;
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

    int RED_READ = digitalRead(RED_BTN);
    int YELLOW_READ = digitalRead(YELLOW_BTN);

    if (currentMillis - started >= GAME_TIME + GAME_SAFE) {

      if (redPoint > yellowPoint) {

        winRED();

      } else if (yellowPoint > redPoint) {

        winYELLOW();

      } else {

        drawGame();

      }

    } else {
      if (RED_READ == LOW) {
        dominationRED();
      } else if (YELLOW_READ == LOW) {
        dominationYELLOW();
      } else {
        showLcdStatus(STARTED);
      }
    }

  }
}



void dominationRED() {
  digitalWrite(YELLOW_LED, LOW); // LED OFF
  digitalWrite(RED_LED, HIGH); // LED ON
  redPoint += 1;
  showLcdStatus(RED_DOMINATION);
}

void dominationYELLOW() {
  digitalWrite(RED_LED, LOW); // LED OFF
  digitalWrite(YELLOW_LED, HIGH); // LED ON
  yellowPoint += 1;
  showLcdStatus(YELLOW_DOMINATION);
}

void winRED() {

  //Serial.println("RED WINS!");
  showLcdStatus(RED_WIN);

  //Serial.println(String(getPercent(redPoint)) + "% (" + String(redPoint)
  //               + ") contra o YELLOW: " + String(getPercent(yellowPoint))
  //               + "% (" + String(yellowPoint) + ")." );

  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  playBuzz();
  delay(500);
  digitalWrite(RED_LED, LOW);
  stopBuzz();
  delay(500);

}


void winYELLOW() {

  //Serial.print("YELLOW WINS! ");
  showLcdStatus(YELLOW_WIN);

  //Serial.println(String(getPercent(yellowPoint)) + "% (" + String(yellowPoint)
  //               + ") contra o RED: " + String(getPercent(redPoint))
  //               + "% (" + String(redPoint) + ")." );

  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, HIGH);
  playBuzz();
  delay(500);
  digitalWrite(YELLOW_LED, LOW);
  stopBuzz();
  delay(500);
}

String getPercent(unsigned long actual ) {
  float total = yellowPoint + redPoint;
  //Serial.println("Yellow: " + String(yellowPoint) + " Red: " + String(redPoint));
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
  digitalWrite(RED_LED, HIGH);
  playBuzz();
  delay(500);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
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
        if (redPoint == 0 && yellowPoint == 0) {
          if (DEMO_GAME) {
            writeLcd("DEMO Iniciada  !", "Aperte p capturar");
          } else {
            writeLcd("Jogo Iniciado  !", "Aperte p capturar");
          }

          digitalWrite(YELLOW_LED, HIGH);
          digitalWrite(RED_LED, HIGH);
          playBuzz();
          delay(300);
          digitalWrite(YELLOW_LED, LOW);
          digitalWrite(RED_LED, LOW);
          stopBuzz();
        }
        break;

      case RED_DOMINATION:
        writeLcd("Vermelho", "Dominando");
        break;

      case RED_WIN:
        writeLcd("Vencedor:", "VERMELHO: " + String(getPercent(redPoint)) + "%");
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
        writeLcd(" DOMINATION BOX", "--- Airsoft ---");
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
