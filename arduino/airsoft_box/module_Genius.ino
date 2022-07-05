int level = 3;
boolean show = true;

void loop_Genius() {
  key = mKeypad.getKey();

  switch (bombState) {

    case BOMB_CONFIG:
      writeLcd("Nivel do desafio", "");
      lcd.setCursor(0, 1);
      lcd.print("Nivel:");

      if (isDigit(key)) {
        configBombTime += String(key);
        writeLcd("", "Nivel: " + String(configBombTime));
      } else if (key == KEY_ENTER) {
        level = configBombTime.toInt();
        writeLcd(" ", alignText("Configurada!", 'C'));
        progress = 1;
        passToArm = getKeyPass(level);
        delay(1000);
        clearDisplay();
        bombState = BOMB_OFF;
      } else if (key == KEY_DEL) {
        configBombTime = "";
        writeLcd("Nivel do desafio", "Nivel:");
      }
      break;

    case BOMB_OFF:
      writeLcd("Pressione qualquer", "tecla p iniciar");

      if (!isEmpty(String(key))) {
        writeLcd("Iniciando GENIUS", alignText("Nivel " + String(level), 'R'));
        delay(2000);
        bombState = BOMB_ACTIVE;
      }
      break;

    case BOMB_ACTIVE:
      genius();
      break;

    case BOMB_DEFUSED:
      writeLcd("Desafio completo", alignText("Nivel " + String(level), 'R'));
      break;
  }

}

void genius() {

  showLed(ledBlue, 0);
  showLed(ledYellow, 0);

  if (progress > level) {
    bombState = BOMB_DEFUSED;
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

    } else if (key == KEY_ENTER ) {
      if (passToDisarm.equals(passToArm.substring(0, progress))) {
        showLed(ledBlue, 0);
        show = true;
        progress++;
        passToDisarm = "";
        writeLcd("CERTO",   "proximo nivel ...");
        delay(2000);

      } else {
        delay(500);
        writeLcd(alignText("Sequencia", 'C'), alignText("Incoreta", 'C'));
        progress = 1;
        show = true;
        passToDisarm = "";
        delay(2000);
      }

    } else if (key == KEY_DEL) {
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
