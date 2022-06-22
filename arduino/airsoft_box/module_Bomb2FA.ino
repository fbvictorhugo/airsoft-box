
void loop_Bomb2FA() {

  int BLUE_READ = digitalRead(BLUE_BTN);
  int YELLOW_READ = digitalRead(YELLOW_BTN);
  delay(100);

  if (BLUE_READ == LOW) {
    digitalWrite(BLUE_LED, HIGH);
    btnBlueActive = 1;
  }

  if (YELLOW_READ == LOW) {
    digitalWrite(YELLOW_LED, HIGH);
    btnYellowActive = 1;
  }

  switch (bombState) {

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
        digitalWrite(BLUE_LED, HIGH);
        ledBlinkTeam(YELLOW);

      } else if (btnYellowActive && !btnBlueActive) {
        digitalWrite(YELLOW_LED, HIGH);
        ledBlinkTeam(BLUE);
      } else {
        ledBlinkTeam(BLUE);
        ledBlinkTeam(YELLOW);
      }

      break ;

    case BOMB_DEFUSED:
      delay(500);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      break ;
  }

}
