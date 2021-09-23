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
 * @doc Мотор может работать в двух режимах, как второй мотор, там и как 3-й.
 * Если нажата клавиша переключения режимов, то останавливаем мотор и меняем тип его работы.
 * От поддержания скорости до следования к цели.
 */

bool Stm1Working(uint16_t KeyCode) {
  // сохраняем состяоние в переменную, чтобы два раза не дергать функцию
  bool _state = Stm1.getState();

  // проверяем нажатие на смену режима работы
  if (KeyCode == STM1_KEY_MODE) {
    // если крутимся, то надо остановиться
    if (_state) 
      Stm1.brake();
    // меняем режим
    if (Stm1Mode == FOLLOW_POS) {
      Stm1Mode = KEEP_SPEED;
      // в этом режиме реверс не нужен 
      Stm1Reverse = false;
      Stm1.reverse(Stm1Reverse);
      //ускорение тоже не нужно, иначе с задержкой реагируем на нажатие
      Stm1.setAcceleration(0);
    } else {
      Stm1Mode = FOLLOW_POS;
      //ускорение восстанавливаем для режима следования
      Stm1.setAcceleration(STEP_ACCELEREATION);
    }
    //меняем тип движения и скорость
    Stm1.setRunMode(Stm1Mode);
    Stm1.setSpeed(Stm1Speed);
#ifdef DEBUG
    Serial.println("STM1_KEY_MODE ");
#endif 
    //выходим с текущим состоянием мотора 
    return Stm1.getState();
  }

  //отрабатываем нажатия кнопок, раз сюда попали, значит кнопка не изменения режима
  if (Stm1Mode == FOLLOW_POS) {
    //если крутимся, то ничего не делаем
    if (_state) 
      return _state;

    //обрабатываем кнопки  
    switch (KeyCode) {
      case STM1_KEY_CW:
        Stm1Reverse = false;
        Stm1.reverse(Stm1Reverse);
        Stm1.setTarget(Stm1StepsAuto, RELATIVE);
      break;
      case STM1_KEY_CCW:
        Stm1Reverse = true;
        Stm1.reverse(Stm1Reverse);
        Stm1.setTarget(Stm1StepsAuto, RELATIVE);
      break;
    }
  } else {
    // если кнопки не нажаты, то останавливаем двигатель
    if (KeyCode == 0) {
      Stm1.stop();
    } else {
      Stm1.setSpeed(Stm1Speed);
#ifdef DEBUG
    Serial.print("SetSpeed ");
#endif 
    }
    //крутим моторы на заданное кол-во шагов
    switch (KeyCode) {
      case STM1_KEY_CW:
#ifdef DEBUG
    Serial.println("CW ");
#endif 
        Stm1.setTarget(Stm1StepsManual, RELATIVE);
      break;
      case STM1_KEY_CCW:
#ifdef DEBUG
    Serial.println("CCW ");
#endif 
        Stm1.setTarget(-1 * Stm1StepsManual, RELATIVE);
      break;
    }
  }
  // возвращаем состояние мотора    
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
  if (_state) 
    return(_state); 

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
  //возвращаем состояние мотора
  return Stm2.getState();
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
  //возвращаем текущее положение мотора
  return Stm3.getTarget();
}

#endif