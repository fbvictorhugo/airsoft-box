/*
  KTNE Descarga do Capacitor
  "Descargue o capacitor antes que ele sobrecarregue forçando a alavanca para baixo."

  Um dispositivo que de tempos em tempos é necessário, aprtar o botão para que não "encha" e exploda.
  Tipo uma barra de loading, porem ela nao pode chegar no 100%.
  Essa taxa de atualização pode ser aleatoria,  1x demorar 1 minuto para encher .. 2x 8minutos... 3x 4minutos.
  4x 1.minutos
*/

int maxcap = 12;
int progress = 0;

String tempoTotal = "";
long tfrac = 0;
long astarted = 0;
long acaptureTime = 0;

void loop_Capacitor() {
  key = mKeypad.getKey();
  long diff = 0;

  switch (bombState) {

    case BOMB_CONFIG:
      writeLcd("Configure tempo", "");
      lcd.setCursor(0, 1);
      lcd.print("Minutos:");

      if (isDigit(key)) {
        tempoTotal += String(key);
        writeLcd("", "Minutos: " + String(tempoTotal));
      } else if (key == KEY_ENTER) {
        tfrac = (tempoTotal.toInt() * 60 * 1000) / maxcap;
        bombState = BOMB_OFF;
      } else if (key == KEY_DEL) {
        tempoTotal = "";
        writeLcd("Configure tempo", "Minutos:");
      }

      break;

    case BOMB_OFF:
      writeLcd("Pressione qualquer", "tecla p iniciar");

      if (!isEmpty(String(key))) {
        writeLcd("Preparando", "Capacitor");
        delay(2000);
        bombState = BOMB_ACTIVE;
        writeLcd("Capacitor", "-[            ]-");
        astarted = millis();
      }
      break;

    case BOMB_ACTIVE:

      acaptureTime = millis();
      diff = acaptureTime - astarted;

      if (key == KEY_ENTER) {
        lcd.setCursor(2, 1);
        lcd.print("            ");
        progress = 0;
        delay(2000);
      }

      if (diff >= tfrac)  {

        if (progress < maxcap) {
          lcd.setCursor(progress + 2, 1);
          lcd.print("=");

          astarted = millis();
          acaptureTime = 0;
          progress++;
        } else if (progress == maxcap) {
          writeLcd("Capacitor", "EXPLODIU");
          bombState = BOMB_EXPLODED;
        }
      }

      break;

    case BOMB_EXPLODED:

      break;
  }
}
