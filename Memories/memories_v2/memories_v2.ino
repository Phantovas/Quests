/*
 * memories.ino - main unit for game "Memories"
 * @Author Vasiliy A. Ponomarjov March 15, 2015
 * @modified Vasiliy A. Ponomarjov March 25, 2015
 * @email vas@vingrad.ru
*/

//#define DEBUG

/**
* Системные модули
*/
#include <Arduino.h>

/**
 * Библиотеки
 */
#include <EEPROM.h>
#include <Bounce2.h>
#include <PS2Keyboard.h>
/**
 * Модули
 */
#include "gamefield.h"
#include "vars.h"
#include "feeprom.h"
#include "functions.h"

/**
 * Функция инициализации
 */
void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif
  //инициализируем клавиатуру
  Keyboard.begin(KBRD_DATA_PIN, KBRD_IRQ_PIN);
  Keyboard.setKeyCode(true);
  //устанавливаем режим OUTPUT
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  //кнопки режим INPUT
  pinMode(PROGRAM_BTN, INPUT);
  //сигналы успешного прохождения и ожидания
  pinMode(LED_STRIP_PIN, OUTPUT);
  pinMode(LOCK_OPEN_PIN, OUTPUT);
  //перноначальные состояния контактов
  digitalWrite(PROGRAM_BTN, HIGH);
  //включаем антидребезг программный
  programBtn.attach(PROGRAM_BTN);
  programBtn.interval(TIME_BOUNCE_PROGRAM);
  //вывод сообщения при успешном завершении игры
  digitalWrite(LED_STRIP_PIN, LOW);
  digitalWrite(LOCK_OPEN_PIN, LOW);
  //проверяем состояние EEPROM
  uint8_t _clearEeprom = EEPROMRead(EA_CLEAR);
  //если зажата кнопка PROGRAM, то надо сбросить EEPROM
  _clearEeprom = programBtn.read();
  if (_clearEeprom != 1) {
    saveEEPROM();
  } else {
    readEEPROM();
  }  
  //читаем маску
  GameField.loadMask();
  //заполняем нулями регистры
  GameField.changeStateAll(LOW);
  //первоначальное состояние 
  state = WAITING;
#ifdef DEBUG
  Serial.println("matrix ");
  for (int8_t i=0;i<GameField.lengthMatrix();i++) {
    Serial.print(i);
    Serial.print("=");
    Serial.println(GameField.Matrix[i]);
  }
#endif
}

/**
 * Основной цикл
 */
void loop() {
//РАБОТА С КЛАВИШАМИ ***********************************************************
  if (Keyboard.available()) {
    //читаем клавиши
    key = Keyboard.read();
    openCell = cells[key];
    //если нажата кнопка любая, а мы не в режиме игры, то начинаем игру
    if ((state == WAITING) && (openCell > -1))
      TApplication::starting();
#ifdef DEBUG  
    Serial.print("press key ");
    Serial.println(key);
    Serial.print("open cell ");
    Serial.println(openCell);
#endif
  } else {
    //иначе пусто
    key = 0;
    openCell = -1;
  }
  
//РАБОЧИЙ РЕЖИМ*****************************************************************
  //нормальное состояние работы
  switch (state) {
    case WAITING: {
      TApplication::waiting();
      break;
    };
    case PLAYING: {
      //если выбрана ячейки и она еще не открыта
      if ((openCell > -1) && !GameField.getPinState(openCell)) { 
        //если первая картинка из пары открыта
        if (isOpenFirstCell) {
          if (lastNumber == GameField.Matrix[openCell]) {
            //угадали, подсвечиваем
            GameField.changeStatePin(openCell, HIGH);            
            //плюс одна ячейка открытая
            countOpenCells++;
          } else {
            //не угадали, подсвечиваем на TIME_LIGHT_MISTAKE
            GameField.changeStatePin(openCell, HIGH);
            delay(TIME_LIGHT_MISTAKE);
            GameField.changeStatePin(openCell, LOW);
            GameField.changeStatePin(prevOpenCell, LOW);
            //минус одна открытая ячейка
            countOpenCells--;
          }
          //хоть угадали, хоть не угадали, сбразываем номер пары
          lastNumber = 0;
          //первую из пары или открыли, или закрыли
          //без разницы, следующее нажатие открытие первой из пары
          isOpenFirstCell = false;
        } else {
          if (GameField.Matrix[openCell] != 0) {
            lastNumber = GameField.Matrix[openCell];
            GameField.changeStatePin(openCell, HIGH);
            //выстаявляем флаги
            isOpenFirstCell = true;
            prevOpenCell = openCell;
            //+1 к открытым ячейкам
            countOpenCells++;
          }
        } //if isOpenFirstCell
      } // if key
      //если все прошли успешно
      if (countOpenCells == GameField.countGameCells) {
        //надо сбросить все предыдущме значения
        TApplication::reset();
        //сообщаем об успешном прохождении игры
        digitalWrite(LOCK_OPEN_PIN, HIGH);
        //сначала все зажигаем, иначе поморгать не получится
        GameField.changeStateAll(HIGH);
        //моргаем COUNT_BLINK_SUCCESS раз
        GameField.blinking(COUNT_BLINK_SUCCESS);
        //теперь все тушим
        GameField.changeStateAll(LOW);
        //задерживаем открытым замок
        delay(TIME_OPEN_DOOR);
        //закрываем замок
        digitalWrite(LOCK_OPEN_PIN, LOW);
        //переход в состояние ожидания
        state = WAITING;
      }
      break;
    };
    case PROGRAMMING: {
      //если попали в ячейку и ячейка еще активна
      if (openCell > -1) {  
        //открыта первая из пары
        if (isOpenFirstCell) {
          //если не потушена
          if (GameField.getPinState(openCell)) {
            GameField.Matrix[openCell] = lastNumber;
            //увеличиваем счетчик
            lastNumber++;
            prevOpenCell = openCell;
            //тушим диоды
            GameField.changeStatePin(openCell, LOW);
            //есть пара, можно сохранять
            canSave = true;
          } else {
            //сброс флага занятой ячейки
            GameField.changeStatePin(prevOpenCell, HIGH);
          }
          //сброс флага открытой первой ячейки пары
          isOpenFirstCell = false;
        } else {
          //если не потушена
          if (GameField.getPinState(openCell)) {
            GameField.Matrix[openCell] = lastNumber;
            GameField.changeStatePin(openCell, LOW);
            isOpenFirstCell = true;
            prevOpenCell = openCell;
          }
        } //if isOpenFirstCell
      } //if key
    break;
    }
  }
  
//ОПРОС КНОПОК *****************************************************************  
  //если нажата кнопка PROGRAMM
  if (programBtn.update()) {
    if (programBtn.read() == LOW)
      TApplication::programming();
  }

}