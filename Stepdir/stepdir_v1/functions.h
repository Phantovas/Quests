/*
   functions.h 
 * @Author Vasiliy A. Ponomarjov  March 04, 2014
 * @modified Vasiliy A. Ponomarjov  September 20, 2021
 * @email vas@vingrad.ru  
*/


#ifndef STEPDIR_FUN_H
#define STEPDIR_FUN_H

#include "vars.h"

/**
 * Функция работы первого мотора
 * @param uint16_t код нажатия кнопки
 * @return bool состояние мотора, крутит или стоит
 */
bool Stm1Working( uint16_t KeyCode) {
  //запоминаем последнюю нажатую клавишу
  //делаем это для одноразового изменения его параметров
  static uint16_t _prevKeyCode = 0;

#ifdef DEBUG
  Serial.print("KeyCode = ");
  Serial.println(KeyCode);
  Serial.print("_prevKeyCode = ");
  Serial.println(_prevKeyCode);
  Serial.print("Stm1Speed = ");
  Serial.println(Stm1Speed);
#endif
  
  bool _state = Stm1.getState();

  // если нажали клавишу и не ехали, то запуск
  // иначе стоп
  switch (KeyCode) {
    case 2:
      if (KeyCode == _prevKeyCode && _state) {
        Stm1.stop(); 
      } else {
        Stm1.setSpeed(Stm1Speed);
      } 
    break;
    
    case 3:
      if (KeyCode == _prevKeyCode && _state) {
        Stm1.stop(); 
      } else {
        Stm1.setSpeed(Stm1Speed * -1);
      } 
    break;
  }

  _prevKeyCode = KeyCode; //запоминаем последнюю нажатую клавишу

   return Stm1.getState();
}




#endif