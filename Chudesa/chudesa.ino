/**
   Chudesa.ino - main program file for the game Chudesa
 * @Author Vasiliy A. Ponomarjov August 31, 2015
 * @modified Vasiliy A. Ponomarjov September 01, 2015
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
#include <Bounce2.h>
#include <PS2Keyboard.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <OneWire.h>

/**
 * Модули
 */
#include "cdreamfield.h"
#include "crfid.h"
#include "vars.h"
#include "functions.h"

/**
 * Функция установки значений
 */
void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif
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
  //пин открытия двери
  pinMode(DOOR_PIN, OUTPUT);
  digitalWrite(DOOR_PIN, LOW);
  //запускаем игру
  startGame();
}

/**
 * Основная функция 
 */
void loop() {
  if (GameField.getWordLength() != GameField.getCountOpenLetter()) {
    //если есть попытки то играем
    if (count_try > 0) {
      //опрашиваем клавиатуру
      if (Keyboard.available()) {
        key = Keyboard.read(); //читаем клавиши
        if (key != prevKey) {
          prevKey = key;
#ifdef DEBUG
          Serial.print("key "); Serial.println(key);
#endif
          //проверяем вхождение буквыв строку
          if (GameField.checkLetter(key)) {
            //угадали
            guessed_letters++; 
            playSound(MP3_FOUND_LETTER, true);
          } else {
          //не угадали
            playSound(MP3_NOTFOUND_LETTER, true);
          }
          //уменьшаем число попыток
          count_try--;
          playSound(count_try, true);
          if (count_try > 0) {
            playSound(MP3_CALL_LETTER, true);
          }
          //очищение буфера клавы
          clearPSBuffer();
        }
      }
    } else {
      //читаем matrixII
      if (Rfid.avalaible()) {
        byte* addr;
        addr = Rfid.read();
        count_try = getTryCount(addr);
#ifdef DEBUG
        Serial.println("trys ");
        Serial.println(count_try);
#endif
        //говорим считанное число попыток
        playSound(count_try, true);
        //если попытки есть, то и предланаем назвать букву
        if (count_try > 0) {
          playSound(MP3_CALL_LETTER, true);
        }
      }
    }
  } else {
#ifdef DEBUG
    Serial.println("game over");
#endif
    //конец иры, открыто все
    digitalWrite(DOOR_PIN, HIGH);
   //зажигаем все
    GameField.changeStateAll(HIGH);
    //моргаем COUNT_BLINK_SUCCESS раз
    GameField.blinking(COUNT_BLINK_SUCCESS);
    //зажигаем все
    GameField.changeStateAll(HIGH);
    //задержка на открыть дверь
    delay(OPEN_TIME);
    //теперь все тушим
    GameField.reset();
  }  

}