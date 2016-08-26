/**
 * screens.h - экранные функции для teknomac
 * Created by Vasiliy A. Ponomarjov, August 09, 2016
 * Email: vas@vingrad.ru
*/

#ifndef FSCREENS_H
#define FSCREENS_H

/**
 * Показываем и отрисовываем эран тестов
 * @param aScreen номер экрана
 */
void showTestingScreen(uint8_t aScreen) {
  register uint8_t i;
  //если флаг перерисовки, то рисуем, иначе ничего не делаем
  if (_repaintScreen) {
    lcd.clear();
    lcd.setCursor(0, 0);
    switch (aScreen) {
      case 1: {
        lcd.print(C_TEST_PERFOMED);
        lcd.setCursor(0, 1);
        lcd.print(strStop);
        break;
      };
      case 2: {
        lcd.print(C_TEST_SUCCESS);
        lcd.setCursor(0, 1);
        lcd.print(strOk);
        break;
      };
      case 3: {
        lcd.print(C_TEST_FAILURE);
        lcd.setCursor(0, 1);
        lcd.print(strError);
        break;
      }
      //main screen testing
      default: {
        lcd.print(C_TEST_START);
        lcd.setCursor(0, 1);
        lcd.print(strPressStart);
        break;
      };
    }
    //убираем флаг перерисовки экрана
    _repaintScreen = false;
  }
}

/**
 * Отрисовка главного экрана
 * @param aScreen
 */
void showMainScreen(uint8_t aScreen) {
  register uint8_t i;
  //если флаг перерисовки, то рисуем, иначе ничего не делаем
  if (_repaintScreen) {
    lcd.clear();
    lcd.setCursor(0, 0);
    switch (aScreen) {
      case 1: {
        //pump
        break;
      };
      case 2: {
        //auger
        break;
      };
      case 3: {
        break;
      }
      //служебные экраны, недостуапны при пролистывании
      case 4: {
        lcd.print("Attention!");
        break;
      }
      //критическая температура, кажем с задержкой 1 с
      case 5: {
        lcd.print("Attention!");
        break;
      }
      default: {
        if (state = WORKING) {
          //lcd.print(ST_WORK);
        } else {
          //lcd.print(ST_STOP);
        }
        break;
      };
    }
    //убираем флаг перерисовки экрана
    _repaintScreen = false;
  }
}

/**
 * Функция прорисовки экрана остановки
 * @param aScreen
 */
void showStopScreen() {
  if (_repaintScreen) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(C_STOP_FORCED);
    lcd.setCursor(0, 1);
    lcd.print(strPressStart);      
    _repaintScreen = false;
  }
}


#endif FSCREENS_H