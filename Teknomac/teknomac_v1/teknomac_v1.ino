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
//include Bounce
#include <Bounce2.h>

//include strings
#include "fstring.h"
//inclure const and variables
#include "vars.h"
//include eeprom functions
#include "feeprom.h"
//include menu functions
#include "fmenu.h"
//include screen function
#include "fscreen.h"
//include functions
#include "functions.h"

#define DFROBOT 1

/*
 * PROGRAM
 */
void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif
  //инициализируем размерность дисплея
  lcd.begin(LCD_COL, LCD_ROW);
  
  //START STATE
  state = TESTING;
  
  //<editor-fold defaultstate="collapsed" desc="Input pins initialize">
  //инициализация входных пинов
  pinMode(STOPBTN_PIN, INPUT);
  pinMode(STARTBTN_PIN, INPUT);
  pinMode(FEED_PIN, INPUT);
  pinMode(EMISSION_PIN, INPUT);
  pinMode(SHAFT_DOWN_PIN, INPUT);
  pinMode(SHAFT_UP_PIN, INPUT);
  pinMode(IRON_DOWN_PIN, INPUT);
  pinMode(IRON_UP_PIN, INPUT);
  //устанавливаем высокие уровни
  digitalWrite(STOPBTN_PIN, HIGH);
  digitalWrite(STARTBTN_PIN, HIGH);
  digitalWrite(FEED_PIN, HIGH);
  digitalWrite(EMISSION_PIN, HIGH);
  digitalWrite(SHAFT_DOWN_PIN, HIGH);
  digitalWrite(SHAFT_UP_PIN, HIGH);
  digitalWrite(IRON_DOWN_PIN, HIGH);
  digitalWrite(IRON_UP_PIN, HIGH);  
  //активируем антидребезг
  startBtn.attach(STARTBTN_PIN);
  startBtn.interval(TIME_BOUNCE_BTN);
  stopBtn.attach(STOPBTN_PIN);
  stopBtn.interval(TIME_BOUNCE_BTN);
  //</editor-fold>
  
    //<editor-fold defaultstate="collapsed" desc="Output pins initialize">
  //инициализация входных пинов
  pinMode(STOPBTN_PIN, OUTPUT);
  pinMode(STARTBTN_PIN, OUTPUT);
  pinMode(FEED_PIN, OUTPUT);
  //устанавливаем низкие уровни
  digitalWrite(MOTOR_PIN, LOW);
  digitalWrite(SHAFTPNEVMO_PIN, LOW);
  digitalWrite(EMISSIONPNEVMO_PIN, LOW);
  //</editor-fold>
  
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
  menu.addMenuItem(2, 0, "Statistics", false, NULL);
  menu.addMenuItem(3, 0, "Tools", false, NULL);
  //setup param
  menu.addMenuItem(4, 1, "Length", true, NULL);
  menu.addMenuItem(5, 1, "Time filing", true, NULL);
  menu.addMenuItem(6, 2, "Amount", true, NULL);
  menu.addMenuItem(7, 2, "Total", true, NULL);
  menu.addMenuItem(8, 2, "Errors", true, NULL);
  menu.addMenuItem(9, 3, "Clear EEPROM", true, lmClearEeprom);
  //строим меню в памяти
  menu.start();

  //рисуем первоначальный экран
  showTestingScreen(_numScreen);

  //инициализируем переменную послдней проверки
  previousMillis = millis();
}


void loop() {
  // read the buttons
  clickKey = readBtn();
  
  //work
  switch (state) {
    //в режиме теста
    case TESTING: {
      
      break;
    }
    //работаем, но ждем пакет
    case WAITING: {
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
      if (_mainScreen) {
        showMainScreen(_numScreen);
      } else {
        //перерисовываем меню, если получили repaintScreen
        //обрабатываем только если не нажата клавиша, иначе мы уже могли перерисовать меню,
        //еще раз перерисовывать смысла нет
        if (_repaintScreen) { 
          lcd.clear();
          menu.show(BTN_NONE, print);
          _repaintScreen = false;
        }  
      }
      break;
    }
    //работаем
    case WORKING: {
      break;
    }
    //остановлен
    case STOPED: {
      break;
    }
  }

}