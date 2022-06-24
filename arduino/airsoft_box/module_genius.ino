int level = 5;
String keyPass;
int step = 1;
boolean show = true;
String pass = "";

void loop_Genius() {

  switch (bombState) {

    case BOMB_CONFIG:
      keyPass = getKeyPass(level);
      bombState = BOMB_OFF;
      break;

    case BOMB_OFF:
      writeLcd("Pressione qualquer", "tecla p iniciar");
      key = mKeypad.getKey();

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

  key = mKeypad.getKey();

  showLed(ledBlue, 0);
  showLed(ledYellow, 0);

  if (step > level) {
    bombState = BOMB_DEFUSED;
    return;
  }

  if (show) {

    writeLcd("Nivel " + String(step), keyPass.substring(0, step));
    showLed(ledYellow, 1);
    delay(1000);

    writeLcd("Nivel " + String(step) + "  Repita: ", " ");
    show = false;

  } else {
    showLed(ledBlue, 1);

    if (isAlpha(key)) {
      pass += String(key);
      writeLcd("Nivel " + String(step) + "  Repita: ",  pass);

    } else if (key == KEY_ENTER ) {
      if (pass.equals(keyPass.substring(0, step))) {
        showLed(ledBlue, 0);
        show = true;
        step++;
        pass = "";
        writeLcd("CERTO",   "proximo nivel ...");
        delay(2000);

      } else {
        delay(500);
        writeLcd(alignText("Sequencia", 'C'), alignText("Incoreta", 'C'));
        step = 1;
        show = true;
        pass = "";
        delay(2000);
      }

    } else if (key == KEY_DEL) {
      pass = "";
      writeLcd("Nivel " + String(step) + "  Repita: ",  " " );
    }
  }
}

String getKeyPass(int level) {
  String array[4] = {"A", "B", "C", "D"};
  String keyPass = "";
  for (int i = 0; i < level; i++) {
    keyPass += array[random(4)];
  }
  Serial.println("KeyPass:" + keyPass);
  return keyPass;
}
