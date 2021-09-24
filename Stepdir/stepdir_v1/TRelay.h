/*
   TRelay.h 
 * @Author Vasiliy A. Ponomarjov  March 04, 2014
 * @modified Vasiliy A. Ponomarjov  September 20, 2021
 * @email vas@vingrad.ru  
*/

#ifndef STEPDIR_TRELAY_H
#define STEPDIR_TRELAY_H

#define FE_START  0
#define FE_STOP   1

/**
 * Структура работы обычного реле
 */
struct TRelay {
private:
  uint8_t _pin;
  bool _state;
  void (*onUpdateState)();
public:
  TRelay(uint8_t APin, void (*AUpdateState)()):  _state(false) { _pin = APin; onUpdateState = AUpdateState; };
  void begin(uint8_t StartState = FE_STOP);
  void start();
  void stop();
  void changeState();
  bool getState() { return _state; };
  //событие опроса температуры
  void setUpdateState(void (*_onUpdateState)()) { onUpdateState = _onUpdateState; }
};

/**
 * Функция инициализации реле
 */
void TRelay::begin(uint8_t StartState = FE_STOP) {
  pinMode(_pin, OUTPUT);
  //почему не исмпользуем функции start и stop. А потому что мы не знаем состояние _state 
  //поэтому дергаем гнепосредственно ногу, а еще нам не нужна реакцияна пользовательскую функцию 
  //это же все-таки инициализация, а не управление  
  if (StartState == FE_STOP) {
    _state = false;
    digitalWrite(_pin, FE_STOP);
  } else {
    _state = true;
    digitalWrite(_pin, FE_START);
  }
}

/**
 * Функция включения реле
 */
void TRelay::start() {
  if (!_state) {
    _state = true;
    digitalWrite(_pin, FE_START);
    //вызываем пользовательскую функцию перерисовки
    if (onUpdateState) {
      (*onUpdateState)();
    }
  }
}

/**
 * Функция остановки реле
 */
void TRelay::stop() {
  if (_state) {
    _state = false;
    digitalWrite(_pin, FE_STOP);
    if (onUpdateState) {
      (*onUpdateState)();
    }
  }
}

/**
 * Функция изменения состояния реле
 */
void TRelay::changeState() {
  if (_state)
    TRelay::stop();
  else
    TRelay::start();
}

#endif 