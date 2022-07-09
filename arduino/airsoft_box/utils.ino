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
int ledState = LOW;
unsigned long ledPrevMillis = 0;

void showLed(PinLed l, int action) {
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

      //      unsigned long ledCrtMillis = millis();
      //
      //      if (ledCrtMillis - ledPrevMillis >= 100) {
      //        ledPrevMillis = ledCrtMillis;
      //        if (ledState == LOW) {
      //          ledState = HIGH;
      //        } else {
      //          ledState = LOW;
      //        }
      //        digitalWrite(l, ledState);
      //      }
      break;
  }
}

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
