/**
   Chudesa.ino - main program file for the game Chudesa
 * @Author Vasiliy A. Ponomarjov August 31, 2015
 * @modified Vasiliy A. Ponomarjov September 01, 2015
 * @email vas@vingrad.ru
*/

#define DEBUG

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
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

/**
 * Модули
 */
#include "gamefield.h"
#include "vars.h"

/**
 * Функция установки значений
 */
void setup() {
  Serial.begin(9600);
  //инициализируем клавиатуру
  Keyboard.begin(KBRD_DATA_PIN, KBRD_IRQ_PIN);
  //устанавливаем режим OUTPUT
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  //инициализируем плеер
	mSerial.begin(9600);
	mp3_set_serial(mSerial); 
  //пин занятости плеера
  pinMode(BUSY, INPUT);
  digitalWrite(BUSY, HIGH);
}

/**
 * Основная функция 
 */
void loop() {
  //если есть попытки то играем
  if (count_try > 0) {
    //опрашиваем клавиатуру
    if (Keyboard.available()) {
      key = Keyboard.read(); //читаем клавиши
      //проверяем вхождение буквыв строку
      if (GameField.checkLetter(key)) {
        //угадали
        guessed_letters++; 
        mp3_play(2);
      } else {
        //не угадали
      }
      //уменьшаем число попыток
      count_try--;
  #ifdef DEBUG
    Serial.print("try ");
    Serial.println(count_try);
  #endif
    }
  } else {
    //читаем matrixII
    mp3_play(3);
  }
}