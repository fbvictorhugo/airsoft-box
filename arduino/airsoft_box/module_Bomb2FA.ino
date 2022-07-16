int btnBlueActive = 0;
int btnYellowActive = 0;

void loop_Bomb2FA() {

  int BLUE_READ = digitalRead(BLUE_BTN);
  int YELLOW_READ = digitalRead(YELLOW_BTN);
  delay(100);

  key = mKeypad.getKey();

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
      writeLcd("Tempo bomba", "");
      lcd.setCursor(0, 1);
      lcd.print("Minutos:");

      if (isDigit(key)) {
        configBombTime += String(key);
        writeLcd("", "Minutos: " + String(configBombTime));
      } else if (isBtnsConfirm(key)) {
        writeLcd("Configurada!", String(configBombTime) + " minutos");
        configBombTime = String(configBombTime.toInt() * 60000);
        if (configBombTime.toInt() == 0) {
          configBombTime = String(10000);// DEMONSTRACAO
          writeLcd("Configurada!", "10 segundos");
        }
        delay(t_wait_menu / 2);
        writeLcd("Iniciando ", "  dispositivo ...");
        delay(t_wait_menu);
        clearDisplay();
        bombState = BOMB_OFF;
      } else if (isKeyDEL(key)) {
        configBombTime = "";
        writeLcd("Tempo bomba", "Minutos:");
      }

      break;
    case BOMB_OFF:
      if (btnBlueActive && btnYellowActive) {
        writeLcd("Verificando ...", "");
        delay(t_wait_device / 2);
        writeLcd(alignText("BOMBA ARMADA", 'C'), " ");
        delay(t_wait_device / 2);
        bombState = BOMB_ACTIVE;
        playBuzzBombActive();
        bombStarted = millis();
        bombFinished = bombStarted + configBombTime.toInt();
        btnBlueActive = 0;
        btnYellowActive = 0;
      } else {
        writeLcd(alignText("Bomb 2FA", 'C'), alignText("Arme a bomba!", 'C'));
      }
      break;

    case BOMB_ACTIVE:
      bombStarted = millis();
      if (bombStarted >= bombFinished) {
        bombState = BOMB_EXPLODED;
        playBuzzBombExploded();
        return;
      }

      if (btnBlueActive && btnYellowActive) {
        writeLcd("Verificando ...", "");
        delay(t_wait_device);
        bombState = BOMB_DEFUSED;
        playBuzzBombDefused();
        btnBlueActive = 0;
        btnYellowActive = 0;

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
      writeLcd(alignText("BOMBA DESARMADA", 'C'), " ");
      showLed(ledYellow, 0);
      showLed(ledBlue, 0);
      if (isKeyDEL(key)) {
        returnToMenu();
      }
      break ;

    case BOMB_EXPLODED:
      writeLcd(alignText("BOMBA", 'C'), alignText("EXPLODIU", 'C'));
      showLed(ledYellow, 0);
      showLed(ledBlue, 0);

      if (isKeyDEL(key)) {
        returnToMenu();
      }
      break;
  }

}
