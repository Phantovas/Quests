/*
 * gamefield.h - header unit for TGameField class
 * @Author Vasiliy A. Ponomarjov March 16, 2015
 * @modified Vasiliy A. Ponomarjov March 25, 2015
 * @email vas@vingrad.ru
*/

#ifndef MMRS_GAMEFIELD_H
#define MMRS_GAMEFIELD_H

/**
 * Структура игрового поля
 */
struct TGameField {
private:
  uint8_t _latchPin;
  uint8_t _clockPin;
  uint8_t _dataPin;
  //структура состояний 
  byte MatrixState[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  //маска состояний 0 - не маскируем ячейку, 1 - маскируем
  byte MatrixMask[8] =  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  //длина матрицы
  uint8_t _matrixLength;
public:
  TGameField(uint8_t ALatchPin, uint8_t AClockPin, uint8_t ADataPin);
  //массив значений 8x8 с первоначальными состояниями пар
  byte Matrix[64] = {
    1,  1,  2,  2,  3,  3,  4,  4,
    5,  5,  6,  6,  7,  7,  8,  8,
    9,  9,  10, 10, 11, 11, 12, 12,
    13, 13, 14, 14, 15, 15, 16, 16,
    17, 17, 18, 18, 19, 19, 20, 20,
    21, 21, 22, 22, 23, 23, 24, 24,
    25, 25, 26, 26, 27, 27, 28, 28,
    29, 29, 30, 30, 31, 31, 32, 32
  };
  uint8_t countGameCells;   //число игровых ячеек  
  void loadMask();
  void startWrite();
  void stopWrite();
  void writeState();
  void changeStateAll(int AState);
  void changeStatePin(byte APin, int AState);
  void blinking(uint8_t ACount);
  void clearMask();
  uint8_t lengthMatrix();
  bool getPinState(byte APin);
};

/**
 * Constructor
 */
TGameField::TGameField(uint8_t ALatchPin, uint8_t AClockPin, uint8_t ADataPin) {
  this->_latchPin = ALatchPin;
  this->_clockPin = AClockPin;
  this->_dataPin = ADataPin;
  //т.к. матрица по-умолчанию полная, то число игровых ячеек равно размерности матрицы
  this->_matrixLength = sizeof(this->Matrix)/sizeof(this->Matrix[0]);
  this->countGameCells = TGameField::lengthMatrix();
}

/**
 * Функция чтения маски матрицы
 */
void TGameField::loadMask() {
  uint8_t _num;
  //сброс числа игровых полей
  this->countGameCells = 0;
  //считаем игровые поля и готовим маску
  for (int8_t i=0;i<TGameField::lengthMatrix();i++) {
    _num = i/8;
    // устанавливаем значение в соответствующий бит (whichPin - _num * 8 --> 63 - 7*8 = 63 - 56 = 7) 
    if (this->Matrix[i] != 0) {
      bitWrite(this->MatrixMask[_num], i - _num * 8, 0);
      this->countGameCells++;
    } else {
      bitWrite(this->MatrixMask[_num], i - _num * 8, 1);
    }
  }
}

/**
 * Функция отключения изменения состояний на время передачи битов
 */
void TGameField::startWrite() {
  digitalWrite(this->_latchPin, LOW);
}

/**
 * Защелкиваем регистр для применения состояний
 */
void TGameField::stopWrite() {
  digitalWrite(this->_latchPin, HIGH);
}

/**
 * Записываем в регистр состояние матрицы
 */
void TGameField::writeState() {
  TGameField::startWrite();
  //загоняем байты в регистры поочередно в обратном порядке
  for (int i=7;i>=0;i--) {
    shiftOut(this->_dataPin, this->_clockPin, MSBFIRST, this->MatrixState[i]);
  }
  TGameField::stopWrite();
}

/**
 * Функция сброса состояний регистров
 * @param AState устанавливаемо значение LOW (погасить) !LOW зажечь
 */
void TGameField::changeStateAll(int AState) {
  //для установки состояний всех выходов
  int _state;
  //проверяем заданное состояние, если LOW тушим все, иначе зажигаем
  if (AState == LOW)
    _state = 0x00;
  else
    _state = 0xFF;
  //устанавливаем новое значение массива состояний с маской
  for (int i=7;i>=0;i--) {
    this->MatrixState[i] = _state & ~this->MatrixMask[i];  
  }
  //загоняем в регистры
  TGameField::writeState();
}

/**
 * Отсылает бит на сдвиговый регистр
 * @param APin номер пина с 0..63
 * @param AState состоянеи LOW - выкл, HIGH - вкл
 */
void TGameField::changeStatePin(byte APin, int AState) {
  //считаем номер строки, в которой надо поменять бит
  uint8_t _num = APin/8;
  // устанавливаем значение в соответствующий бит (whichPin - _num * 8 --> 63 - 7*8 = 63 - 56 = 7) 
  bitWrite(this->MatrixState[_num], APin - _num * 8, AState);
  //применяем маску состояний
  this->MatrixState[_num] &= ~this->MatrixMask[_num]; 
  //записываем в регистры
  TGameField::writeState();
}

/**
 * Функция моргания 
 * @param ACount число раз 
 */
void TGameField::blinking(uint8_t ACount) {
  for (int8_t i=0;i<ACount*2;i++) {
    if (i & 1) {
      //нечетное тушим
      TGameField::startWrite();
      for (int i=7;i>=0;i--) {
        shiftOut(this->_dataPin, this->_clockPin, MSBFIRST, 0x00);
      }
      TGameField::stopWrite();
    } else {
      //четное зажигаем ( 0 - четное)
      //причем только те, которые во включенном состоянии
      TGameField::writeState();
    }
    delay(100);
  }  
}

/**
 * Заполнения матрицы одинаковыми числами
 * @param AValue byte
 */
void TGameField::clearMask() {
  for (int i=0;i<8;i++) {
    this->MatrixMask[i] = 0x00;
  }
}

/**
 * Функция возврата длины матрицы
 * @return 
 */
uint8_t TGameField::lengthMatrix() {
  return this->_matrixLength;
}

/**
 * Функция получения состояния пина
 * 1 - включен или замаскирован, 0 - погашен
 */
bool TGameField::getPinState(byte APin) {
  //номер байта маски
  uint8_t _num = APin/8;
  // читаем значение соответствующего бита, если маска бита равна 0, то бит не используется вернем 1 типа он уже выбран
  return bitRead(this->MatrixState[_num], APin - _num * 8) | bitRead(this->MatrixMask[_num], APin - _num * 8);
}

#endif