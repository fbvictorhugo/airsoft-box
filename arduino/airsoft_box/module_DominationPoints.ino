/*
  Ideia e fazer por tempo de dominacao, vai somandos pontos, ex.: a cada x minutos 1 ponto.
  Retomada de dominacao 1 ponto

  Se empatar no final do tempo, quem estiver por ultimo vence.
*/

void loop_DominationPoints() {
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
      lastDominationACTIVE();
      break;

  }
}

unsigned long x_started = 0;

int x_lastDomination = NONE;

void lastDominationACTIVE() {

  long diff =  captureTime - x_started;

  if (BLUE_READ == LOW ) {
    toCapturing(BLUE);
  } else if (YELLOW_READ == LOW) {
    toCapturing(YELLOW);
  } else {
    toCapturing(NONE);
  }

  if ( diff >= configCaptureTime.toInt()) {
    if (x_lastDomination == BLUE) {
      // ledBLUE();
    } else if (x_lastDomination == YELLOW) {
      // ledYELLOW();
    }
    x_started = 0;
    captureTime = 0;
  }

}

void toCapturing(Teams team) {

  if (team == NONE) {
    writeLcd(alignText("Pressione para", 'C'), alignText("capturar", 'C'));
    showLed(ledBlue, 0);
    showLed(ledYellow, 0);

  } else  if (team != NONE) {

    writeLcd(alignText("Capturando " + String(team), 'C'), " ");
    captureTime = millis();

    if (x_started == 0) {
      x_started = millis();
    }

    switch (team) {
      case BLUE:
        showLed(ledBlue, 1);
        break;
      case YELLOW:
        showLed(ledYellow, 1);
        break;
    }

  }
}
/*
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
*/
