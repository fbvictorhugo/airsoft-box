
void loop_LastDomination() {

  long diff = captureTime - started;

  int BLUE_READ = digitalRead(BLUE_BTN);
  int YELLOW_READ = digitalRead(YELLOW_BTN);
  ledBlink = diff % 100;

  if (BLUE_READ == LOW ) {
    capture(BLUE);
  } else if (YELLOW_READ == LOW) {
    capture(YELLOW);
  } else {
    writeLcd(alignText("LAST", 'C'), alignText("DOMINATION", 'C'));
    reset();
  }

  if ( diff >= GAME_TIME) {
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
    reset();
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
      ledBlinkBlue();
      break;
    case YELLOW:
      ledBlinkYellow();
      break;
  }
}

void ledBlinkYellow() {
  digitalWrite(YELLOW_LED, HIGH); // LED ON
  delay(50);
  digitalWrite(YELLOW_LED, LOW); // LED ON
}

void ledBlinkBlue() {
  digitalWrite(BLUE_LED, HIGH); // LED ON
  delay(50);
  digitalWrite(BLUE_LED, LOW); // LED ON
}

void dominationBLUE() {
  digitalWrite(YELLOW_LED, LOW); // LED OFF
  digitalWrite(BLUE_LED, HIGH); // LED ON
}

void dominationYELLOW() {
  digitalWrite(BLUE_LED, LOW); // LED OFF
  digitalWrite(YELLOW_LED, HIGH); // LED ON
}
