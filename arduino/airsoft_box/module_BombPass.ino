
void loop_BombPassword() {

  int BLUE_READ = digitalRead(BLUE_BTN);
  int YELLOW_READ = digitalRead(YELLOW_BTN);
  delay(100);

  key = mKeypad.getKey();

  switch (bombState) {

    case BOMB_CONFIG:
      writeLcd("Tempo bomba", "");
      lcd.setCursor(0, 1);
      lcd.print("Minutos:");

      if (isDigit(key)) {
        configBombTime += String(key);
        writeLcd("", "Minutos: " + String(configBombTime));
      } else if (key == KEY_ENTER) {
        writeLcd(" ", alignText("Configurada!", 'C'));
        delay(1000);
        clearDisplay();
        bombState = BOMB_OFF;
      } else if (key == KEY_DEL) {
        configBombTime = "";
        writeLcd("Tempo bomba", "Minutos:");
      }

      break;
    case BOMB_OFF:
      writeLcd("Senha p Armar", "");

      if (isDigit(key)) {
        passToArm += String(key);
        writeLcd("Senha p Armar", passToArm);

      } else if (key == KEY_ENTER) {
        writeLcd("Verificando ...", "");
        delay(DELAY_DEF / 2);
        writeLcd(alignText("BOMBA ARMADA", 'C'), " ");
        showLed(ledYellow, 1);
        showLed(ledBlue, 1);
        bombState = BOMB_ACTIVE;
        delay(DELAY_DEF / 2);
        bombStarted = millis();
        bombFinished = bombStarted + (configBombTime.toInt() * 60000);
      } else if (key == KEY_DEL) {
        passToArm = "";
        writeLcd("Senha p Armar", " ");
      }

      break;

    case BOMB_ACTIVE:
      showLed(ledYellow, 1);
      showLed(ledBlue, 1);
      writeLcd("Senha p Desarmar", "");

      bombStarted = millis();
      if (bombStarted >= bombFinished) {
        bombState = BOMB_EXPLODED;
        return;
      }

      if (isDigit(key)) {
        passToDisarm += String(key);
        writeLcd("Senha p Desarmar", passToDisarm);

      } else if (key == KEY_ENTER) {
        writeLcd("Verificando ...", "");
        delay(DELAY_DEF);

        if (passToArm.equals(passToDisarm)) {
          bombState = BOMB_DEFUSED;
        } else {
          passToDisarm = "";
          writeLcd("Nao confere ...",  " ");
          delay(DELAY_DEF / 4);
        }

      } else if (key == KEY_DEL) {
        passToDisarm = "";
        writeLcd("Senha p Desarmar", " ");
      }

      break;

    case BOMB_DEFUSED:
      writeLcd(alignText("BOMBA DESARMADA", 'C'), " ");
      showLed(ledYellow, 0);
      showLed(ledBlue, 0);
      break ;

    case BOMB_EXPLODED:
      writeLcd(alignText("BOMBA", 'C'), alignText("EXPLODIU", 'C'));
      break;
  }
}
