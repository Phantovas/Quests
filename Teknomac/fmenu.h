/*
 * MENU FUNCTIONS
 */

//DONE: -o Vas -u fmenu.h Написать процедуру растопки

#ifndef FMENU_H
#define FMENU_H

extern void startAuger();
extern void stopAuger();
extern void startFanSmooth(uint8_t);
extern void stopFan();
extern void stopAll();

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
 * Функция растопки котла
 * @param aBtn
 */
void lmRastopka(int aBtn) {
  bool _exit = false;
  if (!_working && !_critical) {
    startFanSmooth(fanKinding);
  }
  switch (aBtn) {
    case BTN_RIGHT: {
      //запускаем в работу
      if (ssTemperature[1] >= TEMPERATURE_KINDING) {
        _working = true;
        EEPROMWrite(EA_STATE, _working);
        _exit = !_exit;
      }
      break;
    };
    case BTN_UP: {
      if (!_working) {
        startAuger();
      }
      break;
    };
    case BTN_DOWN: {
      if (!_working) {
        stopAuger();
      }
      break;
    };
    case BTN_LEFT:  {
      _exit = !_exit;
      break;
    };
  }
  if (_exit) {
    //а вдруг включили и просто выходим
    stopAuger();
    stopFan();
    menu.exit();
    return;
  }
  message(strRastopka, 0, 0, true);
  //выводим состояние шнека
  if (!_working) {
    message(strActuator[0], 0, 1, false);
    lcd.print(state[_wAuger]);
  } else {
    message(ST_DISABLE, 0, 1, false);
  }
  lcd.print(ssTemperature[1]);
  lcd.print("C");
}

/**
 * Функция установки вида топлива
 * @param aBtn
 */
void lmVidTopliva(int aBtn) {
  static int8_t i = typeFuel;
  switch (aBtn) {
    case BTN_RIGHT: {
      typeFuel = saving(i, EA_TYPEFUEL);
      return;
    };
    case BTN_UP: {
      i++;
      break;
    };
    case BTN_DOWN: {
      i--;
      break;
    };
    case BTN_LEFT: {
      i = typeFuel;
      menu.exit();
      return;
    };
  }
  //проверяем диспазон и рисуем на экране
  i = constrain(i, 0, sizeof(strFuel)/sizeof(strFuel[0])-1);
  message(strTypefuel, 0, 0, true);
  message(strFuel[i], 0, 1, false);
}

/**
 * Функция остановки котла
 * @param aBtn
 */
void lmStop(int aBtn) {
  static bool i = false;
  switch (aBtn) {
    case BTN_RIGHT: {
      if (i) {
        //выполняем код останоки
        _working = i = !i;
        EEPROMWrite(EA_STATE, _working); 
        message("Stoping...", 0, 0, true);
        delay(250);
        stopAll();
        menu.exit();
        return;
      }
      //восстанавливам значени i
      i = false;
      menu.exit();
      return;
    };
    case BTN_UP: {
      i = true;
      break;
    };
    case BTN_DOWN: {
      i = false;
      break;
    };
    case BTN_LEFT:  {
      //после выхода всегда i = 0
      i = false;
      menu.exit();
      return;
    };
  }
  message(strStopped, 0, 0, true);
  message(state[i+4], 0, 1, false);
}


/**
 * Функция очистки епрома
 * @param aBtn
 */
void lmClearEeprom(int aBtn) {
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
  message(state[i+4], 0, 1, false);
}

#endif