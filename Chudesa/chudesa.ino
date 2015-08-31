/**
   Chudesa.ino - main program file for the game Chudesa
 * @Author Vasiliy A. Ponomarjov August 31, 2015
 * @modified Vasiliy A. Ponomarjov August 31, 2015
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
#include "vars.h"

/**
 * Функция установки значений
 */
void setup() {
  Serial.begin(9600);
#ifdef DEBUG
  Serial.println("start");
#endif
  //инициализируем клавиатуру
  Keyboard.begin(KBRD_DATA_PIN, KBRD_IRQ_PIN);
  Keyboard.setKeyCode(true);
  //устанавливаем режим OUTPUT
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
}

/**
 * Основная функция 
 */
void loop() {
//РАБОТА С КЛАВИШАМИ ***********************************************************
  if (Keyboard.available()) {
    //читаем клавиши
#ifdef DEBUG
    Serial.println(Keyboard.read());
#endif
  }  
  
}