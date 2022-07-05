int btnBlueActive = 0;
int btnYellowActive = 0;

void loop_Bomb2FA() {

  int BLUE_READ = digitalRead(BLUE_BTN);
  int YELLOW_READ = digitalRead(YELLOW_BTN);
  delay(100);

  if (BLUE_READ == LOW) {

    showLed(ledBlue, 1);
    btnBlueActive = 1;
  }

  if (YELLOW_READ == LOW) {
    showLed(ledYellow, 1);
    btnYellowActive = 1;
  }

  switch (bombState) {

    case BOMB_CONFIG:
      break;
    case BOMB_OFF:
      if (btnBlueActive && btnYellowActive) {
        writeLcd("", alignText("Bomba Armada", 'C'));
        btnBlueActive = 0;
        btnYellowActive = 0;
        bombState = BOMB_ACTIVE;
      } else {
        writeLcd(alignText("Bomb 2FA", 'C'), alignText("Arme a bomba!", 'C'));
      }
      break;

    case BOMB_ACTIVE:
      if (btnBlueActive && btnYellowActive) {
        writeLcd("", alignText("Bomba Desarmada", 'C'));
        btnBlueActive = 0;
        btnYellowActive = 0;
        bombState = BOMB_DEFUSED;

      } else if (btnBlueActive && !btnYellowActive) {
        showLed(ledYellow, 2);
        showLed(ledBlue, 1);

      } else if (btnYellowActive && !btnBlueActive) {
        showLed(ledYellow, 1);
        showLed(ledBlue, 2);

      } else {
        showLed(ledYellow, 2);
        showLed(ledBlue, 2);
      }

      break ;

    case BOMB_DEFUSED:
      delay(500);
      showLed(ledYellow, 0);
      showLed(ledBlue, 0);
      break ;
  }

}
