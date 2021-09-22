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
  
  //тест кнопки
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

#ifdef DEBUG
  Serial.begin(9600);
#endif  
  //первый мотор
  Stm1.setRunMode(KEEP_SPEED);
  //устанавливаем скорость
  Stm1Speed = constrain(Stm1Speed, _MIN_SPEED_FP, MAX_STEP_SPEED);
  Stm1.setSpeed(0);
  //ускорение без него как-то странно себя ведет двиг
  Stm1.setAcceleration(500);
}

void loop() {
  uint16_t _tempKeyCode = 0;
  
  //тут опрос всех моторов на движение
  Stm1.tick();
  // Stm2.tick();
  // Stm3.tick();

  if (Serial.available()) {
    char ch = Serial.read();
    if (ch == 'q') _tempKeyCode = 2;
    if (ch == 'w') _tempKeyCode = 3;
  }

  switch (_tempKeyCode) {
    case 2:
    case 3:
      Stm1Working(_tempKeyCode);
      break;
    
    default:
      break;
  }
  
}