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
// #include "functions.h"

void setup() {
  pinMode(2, INPUT_PULLUP);
#ifdef DEBUG
  Serial.begin(9600);
#endif  
  //первый мотор
  Stm1.setRunMode(KEEP_SPEED);
  //устанавливаем скорость
  Stm1Speed = constrain(Stm1Speed, _MIN_SPEED_FP, MAX_STEP_SPEED);
  Stm1.setSpeed(Stm1Speed);    
  //установка ускорения в шагах/сек/сек
  Stm1.setAcceleration(0);
  //отключать мотор при достижении цели
  Stm1.autoPower(true);
  // //включить мотор (если указан пин en)
  // Stm1.enable();
}

bool _work;
double _speed;

void loop() {

  if (!Stm1.tick()) {
    Stm1.setSpeed(Stm1Speed);
  }

  if (digitalRead(2) == LOW) {
    Stm1.stop();
    Serial.println("break");
    Stm1Speed = Stm1Speed * -1;
    Serial.println(Stm1Speed);
  } else {
    Serial.print("tick = ");
    _speed = Stm1.getSpeed();
    Serial.println(_speed);
  }

}
