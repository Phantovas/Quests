/*
  teknomac.ino - program for arduino.
  Created by Vasiliy A. Ponomarjov, August 06, 2016
  Email: vas@vingrad.ru
*/

#define DEBUG

/*
 * LIBRARIES
 */
//Sample for EEPROM
#include <EEPROM.h>
//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
//include menu
#include <LiquidMenu.h>

//inclure const and variables
#include "vars.h"
//include eeprom functions
#include "feeprom.h"
//include menu functions
#include "fmenu.h"

#define DFROBOT 1
/*
 * FUNCTIONS
 */
// read the buttons
int readBtn() {
  adcKeyIn = analogRead(0);      // read the value from the sensor
  delay(150);                     //ставим задержку на нажатие клавиш
#if (DFROBOT)
  //значения кнопок: 0-вправо, 131-вверх, 308-вниз, 481-влево, 720-выбор
  if (adcKeyIn > 1000) return BTN_NONE; // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 us this threshold
  if (adcKeyIn < 50)   return BTN_RIGHT;
  if (adcKeyIn < 200)  return BTN_UP;
  if (adcKeyIn < 300)  return BTN_DOWN;
  if (adcKeyIn < 500)  return BTN_LEFT;
  if (adcKeyIn < 700)  return BTN_SELECT;
#else
  //значения кнопок: 0-вправо, 131-вверх, 308-вниз, 481-влево, 720-выбор, ??-сброс
  // we add approx 50 to those values and check to see if we are close
  if (adcKeyIn > 1000) return BTN_NONE; // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 us this threshold
  if (adcKeyIn < 50)   return BTN_RIGHT;
  if (adcKeyIn < 250)  return BTN_UP;
  if (adcKeyIn < 450)  return BTN_DOWN;
  if (adcKeyIn < 650)  return BTN_LEFT;
  if (adcKeyIn < 750)  return BTN_SELECT;
#endif
  return BTN_NONE;  // when all others fail, return this...
}

//рисуем главный экран
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
          lcd.print(ST_WORK);
        } else {
          lcd.print(ST_STOP);
        }
        break;
      };
    }
    //убираем флаг перерисовки экрана
    _repaintScreen = false;
  }
}

/*
 * PROGRAM
 */
void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif
  //инициализируем размерность дисплея
  lcd.begin(LCD_COL, LCD_ROW);
  //pinout initialize

  //читаем значения из EEPROM
  uint8_t _clearEeprom = EEPROMRead(EA_CLEAR);
  if (_clearEeprom != 1) {
    saveEEPROM();
  } else {
    readEEPROM();
  }

  //создаем меню
  //menu.addMenuItem(ID, ParentID, Title, IsParam, Name function for edit param or NULL);
  menu.addMenuItem(1, 0, "Setup", false, NULL);
  //setup param
  menu.addMenuItem(2, 1, "Times", false, NULL);
  menu.addMenuItem(3, 1, "Clear EEPROM", true, lmClearEeprom);
  //строим меню в памяти
  menu.start();

  //рисуем первоначальный экран
  showMainScreen(_numScreen);

  //инициализируем переменную послдней проверки
  previousMillis = millis();
}


void loop() {
  // read the buttons
  clickKey = readBtn();

  //если что-то нажато, а не btnNone
  if (clickKey != BTN_NONE) {
    if (_mainScreen) {
      if (clickKey == BTN_SELECT) {
        _mainScreen = false;
        //очищаем перед показом меню
        lcd.clear();
        menu.show(BTN_NONE, print);
      } else {
        //обрабатываем смену главного экрана
        switch (clickKey) {
          case BTN_DOWN: {
            _numScreen++;
            break;
          }
          case BTN_UP: {
            _numScreen--;
            break;
          }
        }
        _numScreen = constrain(_numScreen, 0, SCR_COUNT-1);
        _repaintScreen = true;
      }
    } else {
      //мы уже в меню, работаем с ним
      if (clickKey == BTN_SELECT) {
        //в меню нажали кнопку select, значит выходим в главный экран
        _mainScreen = true;
        _repaintScreen = true;
        //отключаем редактирование параметра
        menu.show(BTN_LEFT, print);
      } else {
        lcd.clear();
        menu.show(clickKey, print);
      }
    }
  }

  //ничего не нажато, проверяем что показываем и
  //если главный экран. то перерисовываем при установленном флаге
  if (_mainScreen ) {
    showMainScreen(_numScreen);
  } else {
    //перерисовываем меню, если получили repaintScreen
    //обрабатываем только если не нажата клавиша, иначе мы уже могли перерисовать меню,
    //еще раз перерисовывать смысла нет
    if (_repaintScreen && !_mainScreen) { 
      lcd.clear();
      menu.show(BTN_NONE, print);
      _repaintScreen = false;
    }  
  }
}