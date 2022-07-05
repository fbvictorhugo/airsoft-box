String passA = "";
String passD = "";
int DELAY_DEF = 5000;
String btempoBomba = "";
long bstarted = 0;
long bfinished = 0;

void loop_BombPassword() {

  int BLUE_READ = digitalRead(BLUE_BTN);
  int YELLOW_READ = digitalRead(YELLOW_BTN);
  delay(100);

  key = mKeypad.getKey();

  switch (bombState) {

    case BOMB_CONFIG:
      writeLcd("Configure tempo", "");
      lcd.setCursor(0, 1);
      lcd.print("Minutos:");

      if (isDigit(key)) {
        btempoBomba += String(key);
        writeLcd("", "Minutos: " + String(btempoBomba));
      } else if (key == KEY_ENTER) {
        clearDisplay();
        delay(2000);
        bombState = BOMB_OFF;
      } else if (key == KEY_DEL) {
        btempoBomba = "";
        writeLcd("Configure tempo", "Minutos:");
      }

      break;
    case BOMB_OFF:
      writeLcd("Senha p Armar", "");

      if (isDigit(key)) {
        passA += String(key);
        writeLcd("Senha p Armar", passA);

      } else if (key == KEY_ENTER) {
        writeLcd("Verificando ...", "");
        delay(DELAY_DEF / 2);
        writeLcd(alignText("BOMBA ARMADA", 'C'), " ");
        showLed(ledYellow, 1);
        showLed(ledBlue, 1);
        bombState = BOMB_ACTIVE;
        delay(DELAY_DEF / 2);
        bstarted = millis();
        bfinished = bstarted + (btempoBomba.toInt() * 60000);
      } else if (key == KEY_DEL) {
        passA = "";
        writeLcd("Senha p Armar", " ");
      }

      break;

    case BOMB_ACTIVE:
      showLed(ledYellow, 1);
      showLed(ledBlue, 1);
      writeLcd("Senha p Desarmar", "");

      bstarted = millis();
      if (bstarted >= bfinished) {
        bombState = BOMB_EXPLODED;
        return;
      }

      if (isDigit(key)) {
        passD += String(key);
        writeLcd("Senha p Desarmar", passD);

      } else if (key == KEY_ENTER) {
        writeLcd("Verificando ...", "");
        delay(DELAY_DEF);

        if (passA.equals(passD)) {
          bombState = BOMB_DEFUSED;
        } else {
          passD = "";
          writeLcd("Nao confere ...",  " ");
          delay(DELAY_DEF / 4);
        }

      } else if (key == KEY_DEL) {
        passD = "";
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
