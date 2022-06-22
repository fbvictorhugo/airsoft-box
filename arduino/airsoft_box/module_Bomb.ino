String passA = "";
String passD = "";
int DELAY_DEF = 5000;

void loop_BombPassword() {

  int BLUE_READ = digitalRead(BLUE_BTN);
  int YELLOW_READ = digitalRead(YELLOW_BTN);
  delay(100);

  key = cutomKeypad.getKey();

  switch (bombState) {

    case BOMB_OFF:
      writeLcd("Senha p Armar", "");

      if (isDigit(key)) {
        passA += String(key);
        writeLcd("Senha p Armar", passA);

      } else if (key == KEY_ENTER || BLUE_READ == LOW || YELLOW_READ == LOW) {
        writeLcd("Verificando ...", "");
        delay(DELAY_DEF / 2);
        writeLcd(alignText("BOMBA ARMADA", 'C'), " ");
        digitalWrite(BLUE_LED, HIGH);
        digitalWrite(YELLOW_LED, HIGH);
        bombState = BOMB_ACTIVE;
        delay(DELAY_DEF / 2);
      } else if (key == KEY_DEL) {
        passA = "";
        writeLcd("Senha p Armar", " ");
      }

      break;

    case BOMB_ACTIVE:
      digitalWrite(BLUE_LED, HIGH);
      digitalWrite(YELLOW_LED, HIGH);

      writeLcd("Senha p Desarmar", "");

      if (isDigit(key)) {
        passD += String(key);
        writeLcd("Senha p Desarmar", passD);

      } else if (key == KEY_ENTER || BLUE_READ == LOW || YELLOW_READ == LOW) {
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
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      break ;
  }
}
