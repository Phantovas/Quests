/*
  teknomac.ino - program for arduino.
  Created by Vasiliy A. Ponomarjov, August 06, 2016
  Email: vas@vingrad.ru
*/

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

#define DFROBOT 
/*
 * FUNCTIONS
 */
// read the buttons
int readBtn() {
  adcKeyIn = analogRead(0);      // read the value from the sensor
  //Serial.println(adcKeyIn, DEC);

  delay(180); //ставим задержку на нажатие клавиш
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
        lcd.print(strActuator[2]);
        lcd.print(state[_wPump]);
        lcd.setCursor(0,1);
        lcd.print("T feed: ");
        lcd.print(ssTemperature[1]);
        break;
      };
      case 2: {
        //auger
        lcd.print(strActuator[0]);
        lcd.print(state[_wAuger]);
        //fan
        lcd.print(strActuator[1]);
        lcd.print(state[_wFan]);
        lcd.setCursor(0,1);
        lcd.print("T return: ");
        lcd.print(ssTemperature[2]);
        break;
      };
      case 3: {
        lcd.clear();
        lcd.print(augerStartTime / 1000);
        lcd.print(" ");
        lcd.print((millis()-augerStartTime) / 1000); //ms to s
        lcd.setCursor(0,1);
        lcd.print(augerStopTime / 1000);
        lcd.print(" ");
        lcd.print((millis()-augerStopTime) / 1000);  //ms to s
        break;
      }

      //служебные экраны, недостуапны при пролистывании
      case 4: {
        lcd.print("Attention!");
        lcd.setCursor(0,1);
        lcd.print(strActuator[4]);
        lcd.print(state[_wBank+2]);
        break;
      }
      //критическая температура, кажем с задержкой 1 с
      case 5: {
        lcd.print("Attention!");
        lcd.setCursor(0,1);
        lcd.print("Critical temp");
        break;
      }
      default: {
        if (_working) {
          lcd.print(ST_WORK);
        } else {
          lcd.print(ST_STOP);
        }
        lcd.setCursor(0, 1);
        for (i=0; i<ssCountDevice; i++) {
          lcd.print(ssTemperature[i]);
          lcd.print("C ");
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
  //Serial.begin(9600);
  //инициализируем размерность дисплея
  lcd.begin(LCD_COL, LCD_ROW);
  //pinout initialize
  pinMode(poFAN, OUTPUT);                //вывод для управления вертилятором
  pinMode(poAUGER, OUTPUT);              //вывод управления шнеком
  pinMode(poSPIRAL, OUTPUT);             //вывод управления спиралью шнека
  pinMode(poAUGER_HOT, INPUT);           //вход на перегрев
  digitalWrite(poAUGER_HOT, HIGH);
  pinMode(poPUMP, OUTPUT);               //вывод управления циркуляционным насосом
  //pinMode(poBACK_LIGHT, OUTPUT);       //вывод управления подсветкой

  pinMode(poCLOSE_BANK, INPUT);          //вход бункера
  digitalWrite(poCLOSE_BANK, HIGH);

  //настраиваем прерывания для бункера с Low на high
  //attachInterrupt(ioCLOSE_BANK, openBank, CHANGE);

  //устанавливаем уровни
  digitalWrite(poAUGER, HIGH);              //вывод шнека  
  digitalWrite(poSPIRAL, HIGH);             //вывод спирали шнека
  digitalWrite(poPUMP, HIGH);               //вывод управления циркуляционным насосом

  //читаем значения из EEPROM
  uint8_t _clearEeprom = EEPROMRead(EA_CLEAR);
  if (_clearEeprom != 1) {
    saveEEPROM();
  } else {
    readEEPROM();
  }

  //создаем меню
  //menu.addMenuItem(ID, ParentID, Title, IsParam, Name function for edit param or NULL);
  menu.addMenuItem(1, 0, "Rastopka", true, lmRastopka);
  menu.addMenuItem(2, 0, "Vid topliva", true, lmVidTopliva);
  menu.addMenuItem(3, 0, "Stop", true, lmStop);
  menu.addMenuItem(4, 0, "Temp. setup", false, NULL);
  menu.addMenuItem(5, 0, "Setup", false, NULL);
  //temp param
  menu.addMenuItem(6, 4, strTmpAuger, true, lmAuger);
  menu.addMenuItem(7, 4, strTmpFeed, true, lmFeed);
  menu.addMenuItem(8, 4, strTmpFeedCritical, true, lmFeedCritical);
  menu.addMenuItem(9, 4, strTmpReturn, true, lmReturn);
  //setup param
  menu.addMenuItem(10, 5, strFanSpeed, true, lmFanSpeed);
  menu.addMenuItem(11, 5, strFankinding, true, lmFanSpeedKinding);
  menu.addMenuItem(12, 5, strHysteresis, true, lmHysteresis);
  menu.addMenuItem(13, 5, "Time work shnek", false, NULL);
  menu.addMenuItem(14, 5, "Time unwork shnek", false, NULL);
  menu.addMenuItem(15, 5, "Clear EEPROM", true, lmClearEeprom);
  //time work shnek
  menu.addMenuItem(16, 13, strFuel[0], true, lmWorkAuger1);
  menu.addMenuItem(17, 13, strFuel[1], true, lmWorkAuger2);
  menu.addMenuItem(18, 13, strFuel[2], true, lmWorkAuger3);
  //time unwork shnek
  menu.addMenuItem(19, 14, strFuel[0], true, lmUnworkAuger1);
  menu.addMenuItem(20, 14, strFuel[1], true, lmUnworkAuger2);
  menu.addMenuItem(21, 14, strFuel[2], true, lmUnworkAuger3);
  //строим меню в памяти
  menu.start();

  //инициализируем датчики температуры и получаем температуры при загрузке
  dallasTmp.begin();
  tmpSensorInitialize();
  tmpSensorGetTemp();

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
    if (_wBank) {
      showMainScreen(SCR_COUNT);
    }
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

  //нормальный режим, проверяем температуры по времени и перерисовываем главный экран
  //причем не важно работает горелка или нет
  if (millis() - previousMillis > TEMPERATURE_INTERVAL ) {
    previousMillis = millis();
    //проверяем температуру последовательно
    if (ssCountDevice > 0)
      tmpSensorGetTempOne();
    //перерисовываем экран
    _repaintScreen = true;
  }

  //проверка состояния контакта бункера
  checkBank(_wBank);

  //если температура подачи выше заданной, то включаем насос
  if (ssTemperature[1] >= tmpFeed) {
    startPump();
  } else if ((tmpFeed - hysteresis ) > ssTemperature[1]) {
    //do something если температура подачи ниже заданной
    stopPump();
  }

  //проверяем критическую температуру подачи
  if (ssTemperature[1] >= tmpFeedCritical) {
    //выключить шнек
    stopAuger();
    //выключить вентилятор
    stopFan();
    if (!_critical) {
      //покажем сообщение
      _repaintScreen = true;
      //рисуем экран
      showMainScreen(SCR_COUNT+1);
      delay(1000);
      //если не главный экран, то перерисовываем меню
      if (!_mainScreen) {
        if ((menu.getSelectItem()->id == 1) && (menu.isEdit())) {
          //отключаем редактирование параметра
          menu.show(BTN_LEFT, print);
        } else {
          menu.show(BTN_NONE, print);
        }
      } else
        _repaintScreen = true;
      _critical = true;
    }
    //если не главный экран и текущее меню Растопка, то выйти
  } else if (ssTemperature[1] < (tmpFeedCritical - hysteresis)) {
    //если не критическая и без гистерезиса
    _critical = false;
    //проверяем работу горелки
    if (_working) {
      //запускаем в работу шнек, котел же растоплен
      startAugerHot(tmWorkAuger[typeFuel], tmUnworkAuger[typeFuel], checkAugerHot());
      //если обратка + гистерезис выше заданной
      if (ssTemperature[2] >= (tmpReturn + hysteresis)) {
        //выключить вентилятор
        stopFan();
      } else {
        //если обратка + гистерезис < заданной, то включаем вентилятор
        if ( ssTemperature[2] < tmpReturn) {
          startFanSmooth(speedFanStart);
        }
      }
    } else {
      //если не работаем
    }
  }
}


