
/*
  by Victor Hugo
  v1.0.0
*/

// ==============================================
//    RED CONFIGURATIONS
// ==============================================

const int RED_LED =  11;
const int  RED_BTN = 7;
unsigned long redPoint = 0;

// ==============================================
//    BLUE CONFIGURATIONS
// ==============================================

const int BLUE_LED =  10;
const int  BLUE_BTN = 6;
unsigned long bluePoint = 0;

// ==============================================
//    GAME CONFIGURATIONS
// ==============================================
const int LED_CONFIG =  12;
const long GAME_TIME = 1800000; //40min
const long GAME_SAFE = 180000; //3min

//const long GAME_TIME = 60000;//DEMO
//const long GAME_SAFE = 20000; //DEMO


const long CONFIG_BLINK = 200;

int ledConfigState = 0;

unsigned long started = 0;
unsigned long previousMillis = 0;

// ==============================================
//    PROGRAM
// ==============================================

void setup() {
  Serial.begin(9600);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(LED_CONFIG, OUTPUT);

  started = millis();

}

void loop() {

  //digitalWrite(LED_CONFIG, HIGH);
  unsigned long currentMillis = millis();

  if (currentMillis - started <= GAME_SAFE) {

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
      }
    }

  }
}

void dominationRED() {
  digitalWrite(BLUE_LED, LOW); // LED OFF
  digitalWrite(RED_LED, HIGH); // LED ON
  redPoint += 1;
}

void dominationBLUE() {
  digitalWrite(RED_LED, LOW); // LED OFF
  digitalWrite(BLUE_LED, HIGH); // LED ON
  bluePoint += 1;
}

void winRED() {

  Serial.println("RED WINS!");

  Serial.print(getPercent(redPoint));
  Serial.print("% contra o BLUE: ");
  Serial.println(getPercent(bluePoint));

  digitalWrite(BLUE_LED, LOW);

  digitalWrite(RED_LED, HIGH);
  delay(500);
  digitalWrite(RED_LED, LOW);
  delay(500);
}


void winBLUE() {

  Serial.print("BLUE WINS! ");
  Serial.print(getPercent(bluePoint));
  Serial.print("% contra o RED: ");
  Serial.println(getPercent(redPoint));

  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, HIGH);
  delay(500);
  digitalWrite(BLUE_LED, LOW);
  delay(500);
}

unsigned long getPercent(unsigned long actual ) {
  unsigned long total = bluePoint + redPoint;
  return  actual * 100 / total;
}

void drawGame () {
  Serial.println("DRAW GAME !!!!");

  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(RED_LED, HIGH);
  delay(500);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(RED_LED, LOW);
  delay(500);

}





