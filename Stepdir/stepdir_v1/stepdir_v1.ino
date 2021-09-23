/**
   stepdir.ino
 * @Author Vasiliy A. Ponomarjov September 20, 2021
 * @modified Vasiliy A. Ponomarjov September 21, 2021
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
#include <GyverStepper.h>

/**
 * Модули
 */
#include "TRelay.h"
#include "vars.h"
#include "functions.h"

void setup() {

#ifdef DEBUG
  Serial.begin(9600);
#endif  

  //2-й мотор
  Stm1.setRunMode(KEEP_SPEED);
  //устанавливаем скорость
  Stm1Speed = constrain(Stm1Speed, _MIN_SPEED_FP, MAX_STEP_SPEED);
  //ускорение без него как-то странно себя ведет двиг
  Stm1.setAcceleration(0);

  //2-й мотор
  Stm2.setRunMode(FOLLOW_POS);
  //устанавливаем скорость
  Stm1Speed = constrain(Stm2Speed, _MIN_SPEED_FP, MAX_STEP_SPEED);
  //ускорение без него как-то странно себя ведет двиг
  Stm1.setAcceleration(500);
}

void loop() {

  uint16_t _intKeyCode = 0;
  
  //тут опрос всех моторов на движение
  Stm1.tick();
  Stm2.tick();
  // Stm3.tick();

  //опрос клавиатуры
  if (Serial.available()) {
    char ch = Serial.read();
    switch (ch) {
    case 'q':
      _intKeyCode = STM1_KEY_CW;
      break;
    case 'w':
      _intKeyCode = STM1_KEY_CCW;
      break;
    case 'e':
      _intKeyCode = STM2_KEY_CW;
      break;
    case 'r':
      _intKeyCode = STM2_KEY_CCW;
      break;
    }
  }

  Stm1Working(_intKeyCode);
  Stm2Working(_intKeyCode);
 
}