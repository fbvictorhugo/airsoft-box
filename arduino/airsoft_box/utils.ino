//---[ LCD UTILS ]-----------------------------------

void clearDisplay() {
  writeLcd(" ", " ");
}

void writeLcd(String primary, String second) {
  if (!isEmpty(primary)) {
    lcd.setCursor(0, 0);
    lcd.print(fillText(primary));
  }
  if (!isEmpty(second)) {
    lcd.setCursor(0, 1);
    lcd.print(fillText(second));
  }
}

bool isEmpty(String text) {
  return text.length() == 0;
}

String fillText(String text) {
  int remaning =  16 - text.length() ;
  if (remaning > 0) {
    for (int i = 0; i <= remaning; i++) {
      text += " ";
    }
  }
  return text;
}

String alignText(String text, char align) {
  int remaning =  16 - text.length();
  if (align == 'C') {
    remaning = remaning / 2;
  } else if (align == 'R' ) {
    remaning = remaning;
  } else {
    return text;
  }
  String preText = "";
  for (int i = 0; i < remaning; i++) {
    preText += " ";
  }
  return preText + text;
}

//---[ LED UTILS ]-----------------------------------

void showLed(PinLed l, int action) {
  switch (action) {
    case 0:
      digitalWrite(l, LOW);
      break;

    case 1:
      digitalWrite(l, HIGH);
      break;

    default:
      digitalWrite(l, HIGH);
      delay(50);
      digitalWrite(l, LOW);
      delay(50);
      break;
  }
}

//---[          ]-----------------------------------
