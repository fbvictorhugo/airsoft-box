/*
  Descargue o capacitor apertando o botão antes que ele sobrecarregue.
*/
int maxcap = 12;
long timeFrac = 0;

void loop_Capacitor() {
  BLUE_READ = digitalRead(BLUE_BTN);
  YELLOW_READ = digitalRead(YELLOW_BTN);
  delay(100);

  long diff = 0;
  key = mKeypad.getKey();

  switch (bombState) {

    case BOMB_CONFIG:
      writeLcd("Tempo capacitor", "");
      lcd.setCursor(0, 1);
      lcd.print("Minutos:");

      if (isDigit(key)) {
        configBombTime += String(key);
        writeLcd("", "Minutos: " + String(configBombTime));
      } else if (isBtnsConfirm(key)) {
        writeLcd("Configurado!", String(configBombTime) + " minutos");
        configBombTime = String(configBombTime.toInt() * 60000);
        if (configBombTime.toInt() == 0) {
          configBombTime = String(10000);// DEMONSTRACAO
          writeLcd("Configurado!", "10 segundos");
        }
        timeFrac = configBombTime.toInt() / maxcap;
        delay(t_wait_menu / 2);
        clearDisplay();
        bombState = BOMB_OFF;
      } else if (isKeyDEL(key)) {
        configBombTime = "";
        writeLcd("Tempo capacitor", "Minutos:");
      }

      break;

    case BOMB_OFF:
      writeLcd("Pressione qualquer", "tecla p iniciar");

      if (!isEmpty(String(key))) {
        writeLcd("Preparando", "Capacitor");
        delay(t_wait_device);
        bombState = BOMB_ACTIVE;
        playBuzzBombActive();
        writeLcd("Capacitor", "-[            ]-");
        bombStarted = millis();
      }
      break;

    case BOMB_ACTIVE:

      captureTime = millis();
      diff = captureTime - bombStarted;

      if (isBtnsConfirm(key)) {
        writeLcd("Capacitor", "-[            ]-");
        progress = 0;
        delay(t_wait_device);
      }

      if (diff >= timeFrac)  {

        if (progress < maxcap) {
          lcd.setCursor(progress + 2, 1);
          lcd.print("=");

          bombStarted = millis();
          captureTime = 0;
          progress++;
          playBuzzBombActive();

          if (progress == maxcap) {
            bombState = BOMB_EXPLODED;
            playBuzzBombExploded();
          }
        }
      }

      break;

    case BOMB_EXPLODED:
      writeLcd("Capacitor", alignText("EXPLODIU", 'C'));
      if (isKeyDEL(key)) {
        returnToMenu();
      }

      break;
  }
}
