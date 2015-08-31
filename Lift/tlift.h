/*
 * tlift.h - header unit for TLift class
 * @Author Vasiliy A. Ponomarjov August 8, 2015
 * @modified Vasiliy A. Ponomarjov August 8, 2015
 * @email vas@vingrad.ru
*/

#ifndef LIFT_TLIFT_H
#define LIFT_TLIFT_H

#define SHAKE_COUNT 5    //число раскачек
#define SHAKE_TIME  1000 //задержка между раскачками
#define WAIT_STOP   2000 //ожидание срабатывания датчика остановки

/**
 * Структура состояний лифта
 */
struct TLift {
private:
  uint8_t _pin;
  int _pinStop;
  bool _state;
  uint8_t _level;
  unsigned int _moveTime;
  void _move();
public:
  int8_t ledDown;
  int8_t ledUp;
  TLift(uint8_t APin, unsigned int AMoveTime, int APinStop);
  bool getState();
  uint8_t getLevel();
  void goDown();
  void goUp();
  void stop();
  uint8_t getStateStopPin();
  void doShake(uint8_t ACount, unsigned int ATime);
};

/**
 * Constructor
 */
TLift::TLift(uint8_t APin, unsigned int AMoveTime, int APinStop = -1) {
  this->_pin = APin;
  this->_pinStop = APinStop;
  this->_state = false;
  this->_level = 0;
  this->_moveTime = AMoveTime;
  //подсветка напраления движения, по-умолчанию 0 - не используется
  this->ledUp = -1;
  this->ledDown = -1;
}

/**
 * Состояние вращения
 * @return 
 */
bool TLift::getState() {
  return _state;
}

/**
 * Состояние уровня
 * @return 
 */
uint8_t TLift::getLevel() {
  return this->_level;
}

/**
 * Едем вверх
 */
void TLift::goUp() {
  Serial.println("LIFT GO UP");
  this->_state = true;
  this->_level = 0;
  if (this->ledUp > -1) {
    digitalWrite(this->ledUp, HIGH);
  }  
  this->_move();
}

/**
 * Едем вниз
 */
void TLift::goDown() {
  Serial.println("LIFT GO DOWN");
  this->_state = true;
  this->_level = 1;
  if (this->ledDown > -1) {
    digitalWrite(this->ledDown, HIGH);
  }  
  this->_move();
}

/**
 * Запуск движения
 */
void TLift::_move() {
  Serial.println("LIFT MOVE");
  
  //РАСКАЧИВАЕМ, ЕСЛИ РАСКАЧКА НЕ НУЖНА, КОММЕНТИРУЙ СТРОКУ
  this->doShake(SHAKE_COUNT, SHAKE_TIME);
  
  digitalWrite(this->_pin, HIGH); 
  delay(this->_moveTime);
  //если пин остановки задан, то крутим до его замыкания, но не более 2 секунд
  unsigned long _start = millis();
  while ((millis() - _start < WAIT_STOP) && (this->getStateStopPin())) {
    //пустой цикл, просто ждем окончания времени или замыкания контакта на массу
  }
  this->stop();
}

/**
 * Остановка
 */
void TLift::stop() {
  Serial.println("LIFT STOP");
  this->_state = false;
  if (this->ledUp > -1) {
    digitalWrite(this->ledUp, LOW);
  }  
  if (this->ledDown > -1) {
    digitalWrite(this->ledDown, LOW);
  }  
  digitalWrite(this->_pin, LOW);
}

/**
 * Раскачка
 * @param ACount число дерганий
 * @param ATime задержка между дерганиями
 */
void TLift::doShake(uint8_t ACount, unsigned int ATime) {
  uint8_t _command = LOW;
  for (uint8_t i=1;i<ACount;i++) {
    _command = !_command;
    digitalWrite(this->_pin, _command);
    delay(ATime);
  }
  digitalWrite(this->_pin, LOW);
  delay(ATime);
}

/**
 * Возвращает состояние пина контакта верхней точки
 * @return 
 */
uint8_t TLift::getStateStopPin() {
  if (this->_pinStop > -1) {
    return digitalRead(this->_pinStop);
  } else {
    return LOW;
  }
}

#endif