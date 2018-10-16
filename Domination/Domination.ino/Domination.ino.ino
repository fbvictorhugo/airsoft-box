
/*
  by Victor Hugo
  v1.1.0
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ==============================================
//    BLUE CONFIGURATIONS
// ==============================================

const int BLUE_BTN = 8;
const int BLUE_LED =  9;
unsigned long bluePoint = 0;

// ==============================================
//    RED CONFIGURATIONS
// ==============================================

const int RED_BTN = 10;
const int RED_LED =  11;
unsigned long redPoint = 0;

// ==============================================
//    BUZZER CONFIGURATIONS
// ==============================================
const int BUZZ =  12;
const int TONE = 528;

// ==============================================
//    GAME CONFIGURATIONS
// ==============================================
const int LED_CONFIG =  13;



///// GAME REAL
long GAME_TIME = 1800000; //30min
long GAME_SAFE = 180000; //3min

const long CONFIG_BLINK = 200;

int ledConfigState = 0;

unsigned long started = 0;
unsigned long previousMillis = 0;


// ==============================================
//    COMPONENTS INSTANCE
// ==============================================

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int const PRE_GAME = 0;
int const STARTED = 1;
int const RED_DOMINATION = 2;
int const BLUE_DOMINATION = 3;
int const RED_WIN = 4;
int const BLUE_WIN = 5;
int const DRAW_GAME = 6;

int const DEMO_JUMPER_R = 3;
int const DEMO_JUMPER_W = 4;

int currentStatus = -42;
bool DEMO_GAME = false;

// ==============================================
//    PROGRAM
// ==============================================

void setup() {
  Serial.begin(9600);
  lcd.begin (16, 2);

  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(LED_CONFIG, OUTPUT);

  pinMode(DEMO_JUMPER_W, OUTPUT);

  digitalWrite(DEMO_JUMPER_W, HIGH);
  DEMO_GAME = digitalRead(DEMO_JUMPER_R) > 0;
  if (DEMO_GAME) {
    ///// DEMONSTRACAO
    GAME_TIME = 10000;//DEMO
    GAME_SAFE = 5000; //DEMO
    Serial.println("DEMO ");
  } else {
    Serial.println("NORMAL");
  }

  started = millis();

}

void loop() {

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
    int BLUE_READ = digitalRead(BLUE_BTN);

    if (currentMillis - started >= GAME_TIME + GAME_SAFE) {

      if (redPoint > bluePoint) {

        winRED();

      } else if (bluePoint > redPoint) {

        winBLUE();

      } else {

        drawGame();

      }

    } else {
      if (RED_READ == LOW) {
        dominationRED();
      } else if (BLUE_READ == LOW) {
        dominationBLUE();
      } else {
        showLcdStatus(STARTED);
      }
    }

  }
}

void dominationRED() {
  digitalWrite(BLUE_LED, LOW); // LED OFF
  digitalWrite(RED_LED, HIGH); // LED ON
  redPoint += 1;
  showLcdStatus(RED_DOMINATION);
}

void dominationBLUE() {
  digitalWrite(RED_LED, LOW); // LED OFF
  digitalWrite(BLUE_LED, HIGH); // LED ON
  bluePoint += 1;
  showLcdStatus(BLUE_DOMINATION);
}

void winRED() {

  Serial.println("RED WINS!");
  showLcdStatus(RED_WIN);

  Serial.println(String(getPercent(redPoint)) + "% (" + String(redPoint)
                 + ") contra o BLUE: " + String(getPercent(bluePoint))
                 + "% (" + String(bluePoint) + ")." );

  digitalWrite(BLUE_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  playBuzz();
  delay(500);
  digitalWrite(RED_LED, LOW);
  stopBuzz();
  delay(500);

}


void winBLUE() {

  Serial.print("BLUE WINS! ");
  showLcdStatus(BLUE_WIN);

  Serial.println(String(getPercent(bluePoint)) + "% (" + String(bluePoint)
                 + ") contra o RED: " + String(getPercent(redPoint))
                 + "% (" + String(redPoint) + ")." );

  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, HIGH);
  playBuzz();
  delay(500);
  digitalWrite(BLUE_LED, LOW);
  stopBuzz();
  delay(500);
}

String getPercent(unsigned long actual ) {
  float total = bluePoint + redPoint;
  //Serial.println("Blue: " + String(bluePoint) + " Red: " + String(redPoint));
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
  Serial.println("DRAW GAME !!!!");
  showLcdStatus(DRAW_GAME);

  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(RED_LED, HIGH);
  playBuzz();
  delay(500);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(RED_LED, LOW);
  stopBuzz();
  delay(500);

}

void showLcdStatus(int sts) {
  if (sts != currentStatus) {
    switch (sts) {
      case PRE_GAME:
        if (DEMO_GAME) {
          writeLcd("DEMONSTRATIVO", "Aguarde .....");
        } else {
          writeLcd("Preparando Jogo", "Aguarde .....");
        }
        break;

      case STARTED:
        if (redPoint == 0 && bluePoint == 0) {
          if (DEMO_GAME) {
            writeLcd("DEMO Iniciada  !", "Aperte p capturar");
          } else {
            writeLcd("Jogo Iniciado  !", "Aperte p capturar");
          }

          digitalWrite(BLUE_LED, HIGH);
          digitalWrite(RED_LED, HIGH);
          playBuzz();
          delay(300);
          digitalWrite(BLUE_LED, LOW);
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

      case BLUE_WIN:
        writeLcd("Vencedor:", "AMARELO: " + String(getPercent(bluePoint)) + "%");
        break;

      case BLUE_DOMINATION:
        writeLcd("Amarelo", "Dominando");
        break;

      case DRAW_GAME:
        writeLcd("*    EMPATE    *", "");
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
  tone(BUZZ, TONE);
}

void stopBuzz() {
  noTone(BUZZ);
}




