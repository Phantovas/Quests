/**
   Chudesa.ino - main program file for the game Mediateka
 * @Author Vasiliy A. Ponomarjov November 13, 2015
 * @modified Vasiliy A. Ponomarjov November 13, 2015
 * @email vas@vingrad.ru
*/

//#define DEBUG

/**
* Системные модули
*/

/**
 * Библиотеки
 */
#include <Bounce2.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <OneWire.h>

/**
 * Модули
 */
#include "vars.h"

/**
 * Функция установки значений
 */
void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif
}

/**
 * Основная функция 
 */
void loop() {
  
}
