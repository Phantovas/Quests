/**
   stepdir.ino
 * @Author Vasiliy A. Ponomarjov September 20, 2021
 * @modified Vasiliy A. Ponomarjov September 21, 2021
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
#include <GyverStepper.h>

/**
 * Модули
 */
#include "TRelay.h"
#include "vars.h"
// #include "functions.h"


void setup() {
  Serial.begin(9600);
  // режим поддержания скорости
  Stm1.setRunMode(KEEP_SPEED);
  // можно установить скорость
  Stm1.setSpeed(60);    // в шагах/сек
  // Stm1.setSpeedDeg(1);  // в градусах/сек
  // режим следования к целевй позиции
  Stm1.setRunMode(FOLLOW_POS);
  // можно установить позицию
   Stm1.setTarget(0);           // в шагах
  // Stm1.setTargetDeg(-360);  // в градусах
  // // установка макс. скорости в градусах/сек
  // Stm1.setMaxSpeedDeg(400);
  
  // установка макс. скорости в шагах/сек
  Stm1.setMaxSpeed(400);
  // установка ускорения в шагах/сек/сек
  Stm1.setAcceleration(300);
  // отключать мотор при достижении цели
  Stm1.autoPower(true);
  // включить мотор (если указан пин en)
  Stm1.enable();
}

void loop() {
  // просто крутим туды-сюды
  if (!Stm1.tick()) {
    static bool dir;
    dir = !dir;
    Stm1.setTarget(dir ? -1024 : 1024);
  }
  // график положения
  static uint32_t tmr2;
  if (millis() - tmr2 > 20) {
    tmr2 = millis();
    Serial.println(Stm1.getCurrent());
  }

}
