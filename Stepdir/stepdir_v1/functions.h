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
bool Stm1Working(uint16_t KeyCode) {
  //запоминаем последнюю нажатую клавишу
  //делаем это для одноразового изменения его параметров
  static uint16_t _prevKeyCode = 0;

  bool _state = Stm1.getState();

  // если нажали клавишу и не ехали, то запуск
  // иначе стоп
  switch (KeyCode) {
    case STM1_KEY_CW:
      if (KeyCode == _prevKeyCode && _state) {
        Stm1.stop(); 
      } else {
        Stm1.setSpeed(Stm1Speed);
      } 
      _prevKeyCode = KeyCode; //запоминаем последнюю СРАБОТАВШУЮ нажатую клавишу
    break;
    
    case STM1_KEY_CCW:
      if (KeyCode == _prevKeyCode && _state) {
        Stm1.stop(); 
      } else {
        Stm1.setSpeed(-1 * Stm1Speed);
      } 
      _prevKeyCode = KeyCode; //запоминаем последнюю нажатую клавишу
    break;
  }

   return Stm1.getState();

}

/**
 * Функция работы второго мотора
 * @param uint16_t код нажатия кнопки
 * @return bool состояние мотора, крутит или стоит
 * @doc Мотор движется вперед по нажатию кнопки STM2_KEY_CW на задданое STM1_STEPS количество шагов.
 * Во время движения нажатия клавиш игнорируются до остановки (косяк конечно, если что пойдет не так то ой).
 * Если мотор стоит в крайнем из положений, то повторное нажатие запускает работу от той же точки.
 */
bool Stm2Working(uint16_t KeyCode) {
  // сохраняем состяоние в переменную, чтобы два раза не дергать функцию
  bool _state = Stm2.getState();
    // если движемся то выходим из функции и возвращаем _state
  if (_state) return(_state); 

  // крутим моторы на заданное кол-во шагов
  switch (KeyCode) {
   case STM2_KEY_CW:
     Stm2Reverse = false;
     Stm2.reverse(Stm2Reverse);
     Stm2.setTarget(Stm2Steps, RELATIVE);
   break;
   case STM2_KEY_CCW:
     Stm2Reverse = true;
     Stm2.reverse(Stm2Reverse);
     Stm2.setTarget(Stm2Steps, RELATIVE);
   break;
  }

  return Stm1.getState();
}

/**
 * Функция работы третьего мотора
 * @param uint16_t код нажатия кнопки
 * @return long текущая позиция
 * @doc Мотор движется, только если зажата клавиша, как только бросили, то стоп.
 */
long Stm3Working(uint16_t KeyCode) {
  // если получили 0, то останавливаем мотор 
  if (KeyCode == 0) {
    Stm3.stop();
  } else {
    Stm3.setSpeed(Stm3Speed);
  }
  
  //крутим моторы на заданное кол-во шагов
  switch (KeyCode) {
   case STM3_KEY_CW:
     Stm3.setTarget(Stm3Steps, RELATIVE);
   break;
   case STM3_KEY_CCW:
     Stm3.setTarget(-1 * Stm3Steps, RELATIVE);
   break;
  }

  return Stm3.getTarget();
}

#endif