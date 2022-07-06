/*
  Bomba com tempo da explosao e senha configuravel.
  Se colocar tempo vazio, entra na demonstracao: 10 segundos.
  Se nao digitar senha, entra modo sem senha.
*/

void loop_BombPWD() {

  BLUE_READ = digitalRead(BLUE_BTN);
  YELLOW_READ = digitalRead(YELLOW_BTN);
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

      } else if (isBtnsConfirm(key)) {
        showLed(ledYellow, 1);
        showLed(ledBlue, 1);

        String statusSenha = " ";
        if (isEmpty(passToArm)) {
          hasPassToArm = false;
          statusSenha = "sem senha";
        }

        writeLcd("Verificando ...", "");
        delay(t_wait_device / 2);
        writeLcd(alignText("BOMBA ARMADA", 'C'), alignText(statusSenha, 'C'));
        delay(t_wait_device / 2);
        bombState = BOMB_ACTIVE;
        bombStarted = millis();
        bombFinished = bombStarted + configBombTime.toInt() ;
      } else if (key == KEY_DEL) {
        passToArm = "";
        writeLcd("Senha p Armar", " ");
      }

      break;

    case BOMB_ACTIVE:

      showLed(ledYellow, 2);
      showLed(ledBlue, 2);

      if (hasPassToArm) {
        writeLcd("Senha p Desarmar", "");
      } else {
        writeLcd("Pressione ENTER", "para Desarmar");
      }

      bombStarted = millis();
      if (bombStarted >= bombFinished) {
        bombState = BOMB_EXPLODED;
        return;
      }

      if (hasPassToArm && isDigit(key)) {
        passToDisarm += String(key);
        writeLcd("Senha p Desarmar", passToDisarm);

      } else if (isBtnsConfirm(key)) {
        showLed(ledYellow, 1);
        showLed(ledBlue, 1);
        clearDisplay();
        writeLcd("Verificando ...", "");
        delay(t_wait_device);

        if (passToArm.equals(passToDisarm)) {
          bombState = BOMB_DEFUSED;
        } else {
          passToDisarm = "";
          writeLcd("Nao confere ...",  " ");
          delay(t_wait_device);
        }

      } else if (hasPassToArm && key == KEY_DEL) {
        passToDisarm = "";
        writeLcd("Senha p Desarmar", " ");
      }
      break;

    case BOMB_DEFUSED:
      writeLcd(alignText("BOMBA DESARMADA", 'C'), " ");
      showLed(ledYellow, 0);
      showLed(ledBlue, 0);

      if (key == KEY_DEL) {
        returnToMenu();
      }

      break ;

    case BOMB_EXPLODED:
      writeLcd(alignText("BOMBA", 'C'), alignText("EXPLODIU", 'C'));
      showLed(ledYellow, 0);
      showLed(ledBlue, 0);

      if (key == KEY_DEL) {
        returnToMenu();
      }
      break;
  }

}
