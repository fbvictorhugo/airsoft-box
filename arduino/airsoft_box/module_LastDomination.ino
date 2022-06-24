
unsigned long started = 0;
unsigned long captureTime = 0;
long GAME_TIME = 10000;
bool capturing = false;
int lastDomination = NONE;
bool ledBlink = false;

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

void reset() {
  lastDomination = NONE;
  started = 0;
  capturing = false;
  captureTime = 0;
}
