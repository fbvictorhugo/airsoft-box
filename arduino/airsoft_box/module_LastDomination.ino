
unsigned long started = 0;

bool capturing = false;
int lastDomination = NONE;
bool ledBlink = false;

void loop_LastDomination() {

  BLUE_READ = digitalRead(BLUE_BTN);
  YELLOW_READ = digitalRead(YELLOW_BTN);
  delay(100);

  key = mKeypad.getKey();

  switch (bombState) {

    case BOMB_CONFIG:
      writeLcd("Tempo p capturar", "");
      lcd.setCursor(0, 1);
      lcd.print("Minutos:");

      if (isDigit(key)) {
        configCaptureTime += String(key);
        writeLcd("", "Minutos: " + String(configCaptureTime));
      } else if (isBtnsConfirm(key)) {
        if (isEmpty(configCaptureTime)) {
          configCaptureTime = String(5000);// DEMONSTRACAO
          writeLcd("Configurado!", "5 segundos");
        } else {
          writeLcd("Configurado!", String(configCaptureTime) + " minutos");
          configCaptureTime = String(configCaptureTime.toInt() * 60000);
        }
        delay(t_wait_menu / 2);
        writeLcd("Iniciando ", "  dispositivo ...");
        delay(t_wait_menu);
        clearDisplay();
        bombState = BOMB_OFF;
      } else if (isKeyDEL(key)) {
        configCaptureTime = "";
        writeLcd("Tempo p capturar", "Minutos:");
      }

      break;
    case BOMB_OFF:
      bombState = BOMB_ACTIVE;
      break;
    case BOMB_ACTIVE:
      lastDominaionACTIVE();
      break;

  }
}

void lastDominaionACTIVE() {

  long diff = captureTime - started;
  ledBlink = diff % 100;

  if (BLUE_READ == LOW ) {
    capture(BLUE);
  } else if (YELLOW_READ == LOW) {
    capture(YELLOW);
  } else {
    writeLcd(alignText("Pressione para", 'C'), alignText("capturar", 'C'));
    resetCapture(NONE);
  }

  if ( diff >= configCaptureTime.toInt()) {

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
    resetCapture(team);
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

void ledBlinkTeam(Teams team) {
  switch (team) {
    case BLUE:
      showLed(ledBlue, 2);
      break;
    case YELLOW:
      showLed(ledYellow, 2);
      break;
  }
}

void dominationBLUE() {
  showLed(ledYellow, 0);
  showLed(ledBlue, 1);
}

void dominationYELLOW() {
  showLed(ledYellow, 1);
  showLed(ledBlue, 0);
}

void resetCapture(Teams team) {
  lastDomination = NONE;
  started = 0;
  capturing = false;
  captureTime = 0;
  switch (team) {
    case BLUE:
      showLed(ledBlue, 0);
      break;
    case YELLOW:
      showLed(ledYellow, 0);
      break;
  }
}
