/*
 * gamefield.h - header unit for TGameField class
 * @Author Vasiliy A. Ponomarjov August 31, 2015
 * @modified Vasiliy A. Ponomarjov September 01, 2015
 * @email vas@vingrad.ru
*/

#ifndef CHDS_GAMEFIELD_H
#define CHDS_GAMEFIELD_H

#define SIZE_MATRIX 2  //кол-во сдвиговых регистров
#define SIZE_ELEMENT 8 //размерность одного сдвигового регистра

/**
 * Структура игрового поля
 */
struct TDreamField {
private:
  char* _word; //загаданное слово
  uint8_t _latchPin;  //пин закрытия
  uint8_t _clockPin;  //пин синхронизации
  uint8_t _dataPin;   //пин данных
  //структура состояний 
  byte MatrixState[SIZE_MATRIX] = { 0x00, 0x00 };
  //маска состояний 0 - не маскируем ячейку, 1 - маскируем
  byte MatrixMask[SIZE_MATRIX] =  { 0x00, 0x00 };
  //число открытых букв
  uint8_t _countOpenLetter;
  //функции
  void _loadMask();
  void _clearMask();
  void _setStatePin(byte APin, int AState);
  bool _getPinState(byte APin);
public:
  //функции
  TDreamField(char* AWord, uint8_t ALatchPin, uint8_t AClockPin, uint8_t ADataPin);
  void reset();
  void startWrite();
  void stopWrite();
  void writeState();
  void changeStateAll(int AState);
  char* getWord();
  uint8_t getWordLength();
  uint8_t getCountOpenLetter();
  bool checkLetter(char AValue);
  void blinking(uint8_t ACount);
};

/**
 * Constructor
 */
TDreamField::TDreamField(char* AWord, uint8_t ALatchPin, uint8_t AClockPin, uint8_t ADataPin) {
  //слово
  this->_word = AWord;
  //необходимые пиня для управления регистром
  this->_latchPin = ALatchPin;
  this->_clockPin = AClockPin;
  this->_dataPin = ADataPin;
  //сбрасываем
  this->_countOpenLetter = 0;
  this->_loadMask();  
}

/**
 * Сброс значений и состояний
 */
void TDreamField::reset() {
  //тушим все
  this->_countOpenLetter = 0;
  this->_loadMask();  
  this->changeStateAll(LOW);
}

//<editor-fold defaultstate="collapsed" desc="PRIVATE METHODS">
/**
 * Функция чтения маски матрицы
 */
void TDreamField::_loadMask() {
  uint8_t _num;
  //стираем существующую маску
  this->_clearMask();
  //считаем игровые поля и готовим маску
  for (int i = 0; i < SIZE_MATRIX * SIZE_ELEMENT; i++) {
    _num = i/SIZE_ELEMENT; 
    if (i < TDreamField::getWordLength()) {
      bitWrite(this->MatrixMask[_num], i - _num * SIZE_ELEMENT, 0);
    } else {
      bitWrite(this->MatrixMask[_num], i - _num * SIZE_ELEMENT, 1);
    }
  }
}

/**
 * Заполнения матрицы одинаковыми числами
 * @param AValue byte
 */
void TDreamField::_clearMask() {
  for (int i = 0; i < SIZE_MATRIX; i++) {
    this->MatrixMask[i] = 0x00;
  }
}

/**
 * Отсылает бит на сдвиговый регистр
 * @param APin номер пина с 0..15
 * @param AState состоянеи LOW - выкл, HIGH - вкл
 */
void TDreamField::_setStatePin(byte APin, int AState) {
  //считаем номер строки, в которой надо поменять бит
  uint8_t _num = APin/SIZE_ELEMENT;
  // устанавливаем значение в соответствующий бит (whichPin - _num * SIZE_ELEMENT --> 15 - 1*8 = 15 - 8 = 7) 
  bitWrite(this->MatrixState[_num], APin - _num * SIZE_ELEMENT, AState);
  //применяем маску состояний
  this->MatrixState[_num] &= ~this->MatrixMask[_num]; 
  //записываем в регистры
  TDreamField::writeState();
}

/**
 * Функция получения состояния пина
 * 1 - включен или замаскирован, 0 - погашен
 */
bool TDreamField::_getPinState(byte APin) {
  //номер байта маски
  uint8_t _num = APin/SIZE_ELEMENT;
  // читаем значение соответствующего бита, если маска бита равна 0, то бит не используется вернем 1 типа он уже выбран
  return bitRead(this->MatrixState[_num], APin - _num * SIZE_ELEMENT) | bitRead(this->MatrixMask[_num], APin - _num * SIZE_ELEMENT);
}
//</editor-fold>

/**
 * Функция отключения изменения состояний на время передачи битов
 */
void TDreamField::startWrite() {
  digitalWrite(this->_latchPin, LOW);
}

/**
 * Защелкиваем регистр для применения состояний
 */
void TDreamField::stopWrite() {
  digitalWrite(this->_latchPin, HIGH);
}

/**
 * Записываем в регистр состояние матрицы
 */
void TDreamField::writeState() {
  TDreamField::startWrite();
  //загоняем байты в регистры поочередно в обратном порядке
  for (int i = SIZE_MATRIX - 1; i >= 0; i--) {
    shiftOut(this->_dataPin, this->_clockPin, MSBFIRST, this->MatrixState[i]);
  }
  TDreamField::stopWrite();
}

/**
 * Функция сброса состояний регистров
 * @param AState устанавливаемо значение LOW (погасить) !LOW зажечь
 */
void TDreamField::changeStateAll(int AState) {
  //для установки состояний всех выходов
  int _state;
  //проверяем заданное состояние, если LOW тушим все, иначе зажигаем
  if (AState == LOW)
    _state = 0x00;
  else
    _state = 0xFF;
  //устанавливаем новое значение массива состояний с маской
  for (int i = SIZE_MATRIX - 1; i >= 0; i--) {
    this->MatrixState[i] = _state & ~this->MatrixMask[i];  
  }
  //загоняем в регистры
  TDreamField::writeState();
}

/**
 * Функция возвращает загаданное слово
 * @return 
 */
char* TDreamField::getWord() {
  return this->_word;
}

/**
 * Функция возвращает длину загаданного слова
 * @return 
 */
uint8_t TDreamField::getWordLength() {
  return strlen(this->_word);
}

/**
 * Функция возвращает число открытых букв в слове
 * @return 
 */
uint8_t TDreamField::getCountOpenLetter() {
  return this->_countOpenLetter;
}

/**
 * Функция проверки вхождения буквы в загаданное слово
 * @param AValue
 * @return 
 */
bool TDreamField::checkLetter(char AValue) {
  bool result = false;
  //проверяем все буквы
  for (int i = 0; i < this->getWordLength(); i++) {
    if ((AValue == this->_word[i]) && !this->_getPinState(i)) {
      this->_setStatePin(i, 1);
      this->_countOpenLetter++;
      result |= true;
    }
  } 
  return result;
}

/**
 * Функция моргания 
 * @param ACount число раз 
 */
void TDreamField::blinking(uint8_t ACount) {
  //моргаем
  for (int8_t i = 0; i < ACount * 2; i++) {
    if (i & 1) {
      //нечетное тушим
      TDreamField::startWrite();
      for (int i = SIZE_MATRIX - 1; i >= 0; i--) {
        shiftOut(this->_dataPin, this->_clockPin, MSBFIRST, 0x00);
      }
      TDreamField::stopWrite();
    } else {
      //четное зажигаем ( 0 - четное)
      //причем только те, которые во включенном состоянии
      TDreamField::writeState();
    }
    delay(100);
  }  
}

#endif