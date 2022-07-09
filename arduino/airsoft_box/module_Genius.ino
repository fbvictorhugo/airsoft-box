int level = 3;
boolean show = true;

void loop_Genius() {
  BLUE_READ = digitalRead(BLUE_BTN);
  YELLOW_READ = digitalRead(YELLOW_BTN);
  delay(100);

  key = mKeypad.getKey();

  switch (bombState) {

    case BOMB_CONFIG:
      writeLcd("Nivel do desafio", "");
      lcd.setCursor(0, 1);
      lcd.print("Nivel:");

      if (isDigit(key)) {
        configBombTime += String(key);
        writeLcd("", "Nivel: " + String(configBombTime));
      } else if (isBtnsConfirm(key)) {
        level = configBombTime.toInt();
        writeLcd(" ", alignText("Configurado!", 'C'));
        progress = 1;
        passToArm = getKeyPass(level);
        delay(t_wait_menu / 2);
        clearDisplay();
        bombState = BOMB_OFF;
      } else if (isKeyDEL(key)) {
        configBombTime = "";
        writeLcd("Nivel do desafio", "Nivel:");
      }
      break;

    case BOMB_OFF:
      writeLcd("Pressione qualquer", "tecla p iniciar");

      if (!isEmpty(String(key))) {
        writeLcd("Iniciando GENIUS", alignText("Nivel " + String(level), 'R'));
        delay(t_wait_menu);
        bombState = BOMB_ACTIVE;
        playBuzzBombActive();
      }
      break;

    case BOMB_ACTIVE:
      genius();
      break;

    case BOMB_DEFUSED:
      writeLcd("Desafio completo", alignText("Nivel " + String(level), 'R'));

      if (isKeyDEL(key)) {
        show = true;
        returnToMenu();
      }

      break;
  }

}

void genius() {

  showLed(ledBlue, 0);
  showLed(ledYellow, 0);

  if (progress > level) {
    bombState = BOMB_DEFUSED;
    playBuzzBombDefused();
    return;
  }

  if (show) {
    writeLcd("Nivel " + String(progress), passToArm.substring(0, progress));
    showLed(ledYellow, 1);
    delay(1000);

    writeLcd("Nivel " + String(progress) + "  Repita: ", " ");
    show = false;

  } else {
    showLed(ledBlue, 1);

    if (isAlpha(key)) {
      passToDisarm += String(key);
      writeLcd("Nivel " + String(progress) + "  Repita: ",  passToDisarm);

    } else if (isBtnsConfirm(key)) {
      writeLcd("Verificando ...", "");
      delay(t_wait_device);

      if (passToDisarm.equals(passToArm.substring(0, progress))) {
        showLed(ledBlue, 0);
        show = true;
        progress++;
        passToDisarm = "";
        writeLcd("CERTO",   "proximo nivel ...");
        playBuzzBombActive();
        delay(t_wait_device / 2);

      } else {
        writeLcd(alignText("Sequencia", 'C'), alignText("Incoreta", 'C'));
        progress = 1;
        show = true;
        passToDisarm = "";
        playBuzzBombActive();
        delay(t_wait_device);
      }

    } else if (isKeyDEL(key)) {
      passToDisarm = "";
      writeLcd("Nivel " + String(progress) + "  Repita: ",  " " );
    }
  }
}

String getKeyPass(int level) {
  String array[4] = {"A", "B", "C", "D"};
  String passToArm = "";
  for (int i = 0; i < level; i++) {
    passToArm += array[random(4)];
  }
  Serial.println("passToArm:" + passToArm);
  return passToArm;
}
