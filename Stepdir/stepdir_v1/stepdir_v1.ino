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
#ifdef DEBUG
  Serial.begin(9600);
#endif  
  //первый мотор
  Stm1.setRunMode(FOLLOW_POS);
  //устанавливаем скорость
  Stm1Speed = constrain(Stm1Speed, _MIN_SPEED_FP, MAX_STEP_SPEED);
  Stm1.setSpeed(Stm1Speed);    
  //установка макс. скорости 
  Stm1.setMaxSpeed(MAX_STEP_SPEED);
  //установка ускорения в шагах/сек/сек
  Stm1.setAcceleration(300);
  //отключать мотор при достижении цели
  Stm1.autoPower(true);
  // //включить мотор (если указан пин en)
  // Stm1.enable();
}

void loop() {
  
  if (!Stm1.tick()) {
    Stm1Dir = !Stm1Dir;
    Stm1.setTarget(Stm1Dir ? Stm1Steps : -Stm1Steps, RELATIVE);
  }
}
