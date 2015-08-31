/*
 * functions.h - header unit for functions
 * @Author Vasiliy A. Ponomarjov March 15, 2015
 * @modified Vasiliy A. Ponomarjov March 24, 2015
 * @email vas@vingrad.ru
*/

#ifndef MMRS_FUNCTIONS_H
#define MMRS_FUNCTIONS_H

//<editor-fold defaultstate="" desc="TApplication">
struct TApplication {
public:
  TApplication() {};
  static void reset();
  static void starting();
  static void waiting();
  static void programming();
};

/**
 * Функция сброса игровых значений
 */
void TApplication::reset() {
  //тушим ленту светодиодную
  analogWrite(LED_STRIP_PIN, LOW);
  //подготавливаем переменные первоначальные
  lastNumber = 0;                    //начинаем с 0 т.к. еще ничего не открывали  
  isOpenFirstCell = false;           //открыта первая ячейка пары
  prevOpenCell = 0;                  //последняя открыта ячейка
  countOpenCells = 0;                //количество открытых ячеек
}

/**
 * Функция запуска игры
 */
void TApplication::starting() {
  //если не играем, то запускаем игру
  if (state != PLAYING) {
    TApplication::reset();
    //гасим вывод и начинаем обрабатывать нажатия
    GameField.changeStateAll(LOW);  
    state = PLAYING;  
  }
}

/**
 * Функция сердцебиения, говорит о том что  мы находимся  в режиме ождания
 */
void TApplication::waiting() {
  static uint8_t hbval = 128;
  static int8_t hbdelta = 8;
  if (hbval > 192) hbdelta = -hbdelta;
  if (hbval < 8) hbdelta = -hbdelta;
  hbval += hbdelta;
  analogWrite(LED_STRIP_PIN, hbval);
  delay(20);
}

/**
 * Функция программирования 
 */
void TApplication::programming() {
  if (state == PROGRAMMING) {
    //stop programming
    if (canSave) {
      //записываем
      saveEEPROM();
      //тушим
      GameField.changeStateAll(LOW);
      //читаем маску
      GameField.loadMask();
      //переходим в режим ожидания
      state = WAITING;
    } else {
      GameField.blinking(COUNT_BLINK_PROG_ERROR);
      GameField.writeState();
    }
  } else {
    //сброс игровых параметров
    TApplication::reset();
    //первоначальные установки для программирования
    canSave = false;                  //пустую матрицу сохранить нельзя
    lastNumber = 1;                   //начинаем с 1 (0 - это флаг не используемой ячейки)
    GameField.countGameCells = 0;     //сбрасываем число игровых картинок
    //заполняем матрицу единицами, т.е. маскируем все ячейки
    GameField.clearMask();
    //зажигаем все картинки
    GameField.changeStateAll(HIGH);
    //переходим в режим программирования
    state = PROGRAMMING;
  }
}
//</editor-fold>

#endif