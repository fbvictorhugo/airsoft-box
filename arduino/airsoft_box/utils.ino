//---[ LCD UTILS ]-----------------------------------

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

//---[ LED UTILS ]-----------------------------------
//unsigned long ledPrevMillis = 0;

//  LED_NAME {PIN, STATE, blinking, ledPrevBlink};
int LED_BLUE_VOBJ[4] = {10, LOW, 0, 0};
int LED_YELLOW_VOBJ[4] = {A2, LOW, 0, 0};
int ledVOBJ[4];

void showLed(PinLed l, int action) {
  if (l == ledBlue) {
    ledVOBJ[0] = LED_BLUE_VOBJ[0];
    ledVOBJ[1] = LED_BLUE_VOBJ[1];
    ledVOBJ[2] = LED_BLUE_VOBJ[2];
    ledVOBJ[3] = LED_BLUE_VOBJ[3];
  } else {
    ledVOBJ[0] = LED_YELLOW_VOBJ[0];
    ledVOBJ[1] = LED_YELLOW_VOBJ[1];
    ledVOBJ[2] = LED_YELLOW_VOBJ[2];
    ledVOBJ[3] = LED_YELLOW_VOBJ[3];
  }

  switch (action) {
    case 0:
      ledVOBJ[1] = LOW;
      ledVOBJ[2] = false;
      digitalWrite(ledVOBJ[0], ledVOBJ[1]);
      break;

    case 1:
      ledVOBJ[1] = HIGH;
      ledVOBJ[2] = false;
      digitalWrite(ledVOBJ[0], ledVOBJ[1]);
      break;

    default:

      unsigned long ledCrtMillis = millis();
      if (ledCrtMillis - ledVOBJ[3] >= 800) {
        ledVOBJ[3] = ledCrtMillis;
        if (ledVOBJ[1]  == LOW) {
          ledVOBJ[1]  = HIGH;
        } else {
          ledVOBJ[1]  = LOW;
        }
        ledVOBJ[2] = true;
        digitalWrite(ledVOBJ[0], ledVOBJ[1]);
      }
      break;
  }
  if (l == ledBlue) {
    LED_BLUE_VOBJ[0] = ledVOBJ[0];
    LED_BLUE_VOBJ[1] = ledVOBJ[1];
    LED_BLUE_VOBJ[2] = ledVOBJ[2];
    LED_BLUE_VOBJ[3] = ledVOBJ[3];
  } else {
    LED_YELLOW_VOBJ[0] = ledVOBJ[0];
    LED_YELLOW_VOBJ[1] = ledVOBJ[1];
    LED_YELLOW_VOBJ[2] = ledVOBJ[2];
    LED_YELLOW_VOBJ[3] = ledVOBJ[3];
  }
}

/*void showLed(PinLed l, int action) {
  switch (action) {
    case 0:
      digitalWrite(l, LOW);
      break;

    case 1:
      digitalWrite(l, HIGH);
      break;

    default:

      digitalWrite(l, HIGH);
      delay(50);
      digitalWrite(l, LOW);
      delay(50);

      unsigned long ledCrtMillis = millis();

      if (ledCrtMillis - ledPrevMillis >= 100) {
        ledPrevMillis = ledCrtMillis;
        if (ledState == LOW) {
          ledState = HIGH;
        } else {
          ledState = LOW;
        }
        digitalWrite(l, ledState);
      }
      break;
  }
  }*/

//---[ BUZZ ]-----------------------------------
void playBuzzMenu() {
  tone(BUZZER, TONE, 100);
}

void playBuzzBombActive() {
    tone(BUZZER, TONE, 50);
}

void playBuzzBombDefused() {
  tone(BUZZER, TONE, t_wait_device);
}

void playBuzzBombExploded() {
   tone(BUZZER, TONE, 15000);
}

void stopBuzz() {
  noTone(BUZZER);
}

void playKeyTone(char key) {
  switch (key) {
    case KEY_ENTER:
      tone(BUZZER, TONE, 50);
      break;
    case KEY_DEL:
      tone(BUZZER, 404, 50);
      break;
    case 'A':
      tone(BUZZER, 131, 50);
      break;
    case 'B':
      tone(BUZZER, 196, 50);
      break;
    case 'C':
      tone(BUZZER, 294, 50);
      break;
    case 'D':
      tone(BUZZER, 440, 50);
      break;
  }
}
