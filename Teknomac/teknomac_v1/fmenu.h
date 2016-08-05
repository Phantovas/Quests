/*
 * MENU FUNCTIONS
 */

#ifndef FMENU_H
#define FMENU_H

/**
 * Функция вывода одной строки дисплея
 * @param line номер строки
 * @param str строка
 */
void print(byte line, char *str) {
  lcd.setCursor(0, line);
  lcd.print(str);
  //затираем пробелами оставшиеся колонки
  for (uint8_t i = strlen(str)-1; i < LCD_COL; i++)
    lcd.print(" ");
}

/**
 * Функция вывода на экран сообщения
 * @param str
 * @param _col
 * @param _row
 * @param _clear
 */
void message(char *str, uint8_t _col = 0, uint8_t _row = 0, bool _clear = true) {
  if (_clear)
    lcd.clear();
  lcd.setCursor(_col, _row);
  lcd.print(str);
}

/**
 * Функция сохранения переменной в EEPROM
 * @param aValue
 */
int saving(int aValue, int aAddress = -1) {
  message("Saving...", 0, 0, true);
  if (aAddress > -1) {
    EEPROMWriteInt(aAddress, aValue);
  } 
  delay(250);
  menu.exit();
  return aValue;
}

/**
 * Функция проверки значения на диапазон и прорисовке на экране
 * @param aValue
 * @param minValue
 * @param maxValue
 * @param str
 * @param postfix
 * @return
 */
int checkValue(int aValue, int minValue, int maxValue, char *str, char *postfix) {
  aValue = constrain(aValue, minValue, maxValue);
  message(str, 0, 0, true);
  lcd.setCursor(0, 1);
  lcd.print(aValue);
  lcd.print(postfix);
  return aValue;
}

/**
 * Функция очистки епрома
 * @param aBtn
 */
void lmClearEeprom(int aBtn) {
#ifdef DEBUG
  Serial.println("lmClearEeprom");
#endif
  static uint8_t i;
  switch (aBtn) {
    case BTN_RIGHT: {
      if (i) {
        i = 0;
        message("Clearing...", 0, 0, true);
        delay(250);
        clearEEPROM();
        menu.exit();
        return;
      }
      //восстанавливам значени i
      i = 0;
      menu.exit();
      return;
    };
    case BTN_UP: {
      i = 1;
      break;
    };
    case BTN_DOWN: {
      i = 0;
      break;
    };
    case BTN_LEFT:  {
      //после выхода всегда i = 0
      i = 0;
      menu.exit();
      return;
    };
  }
  message(strClear, 0, 0, true);
  message(strState[i+4], 0, 1, false);
}

#endif