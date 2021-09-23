/**
   stepdir.ino
 * @Author Vasiliy A. Ponomarjov September 20, 2021
 * @modified Vasiliy A. Ponomarjov September 23, 2021
 * @email vas@vingrad.ru
*/

// #define DEBUG

/**
* Системные модули
*/
#include <Arduino.h>

/**
 * Библиотеки
 */
#include <GyverStepper.h>
#include <RCSwitch.h>

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

  //1-й мотор
  Stm1.setRunMode(Stm1Mode);
  //устанавливаем скорость
  Stm1Speed = constrain(Stm1Speed, _MIN_SPEED_FP, MAX_STEP_SPEED);
  //ускорение без него как-то странно себя ведет двиг
  Stm1.setAcceleration(STEP_ACCELEREATION);

  //2-й мотор
  Stm2.setRunMode(FOLLOW_POS);
  //устанавливаем скорость
  Stm1Speed = constrain(Stm2Speed, _MIN_SPEED_FP, MAX_STEP_SPEED);
  //ускорение без него как-то странно себя ведет двиг
  Stm1.setAcceleration(STEP_ACCELEREATION);

  //3-й мотор
  Stm3.setRunMode(KEEP_SPEED);
  //устанавливаем скорость
  Stm3Speed = constrain(Stm3Speed, _MIN_SPEED_FP, MAX_STEP_SPEED);
  //ускорение без него как-то странно себя ведет двиг
  Stm3.setAcceleration(0);

  // коллекторный мотор, т.к. управляется двумя реле, то активируем их в режиме оключено
  DcmIn1.begin(FE_STOP);
  DcmIn2.begin(FE_STOP);
  DcmTimeForward = constrain(DcmTimeForward, DCM_MIN_PERIOD, DCM_MAX_PERIOD);
  DcmTimeBackward = constrain(DcmTimeBackward, DCM_MIN_PERIOD, DCM_MAX_PERIOD);

  // активируем пульт управления 
  mainSwitch.enableReceive(0);
}

void loop() {
  unsigned long _intKeyCode = 0;
  
  // тут опрос всех моторов на движение
  Stm1.tick();
  Stm2.tick();
  Stm3.tick();

  // опрос клавиатуры
 if (mainSwitch.available()) {
#ifdef DEBUG
  Serial.print("Received ");
  Serial.print(mainSwitch.getReceivedValue()); 
#endif   
    _intKeyCode = mainSwitch.getReceivedValue();
    mainSwitch.resetAvailable();
  }   

  // работа моторов
  Stm1Working(_intKeyCode);
  Stm2Working(_intKeyCode);
  Stm3Working(_intKeyCode);
  DcmWorking(_intKeyCode);

  delay(10); //НЕ УДАЛЯТЬ!
 }