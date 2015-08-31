/*
 * functions.h - header unit for functions
 * @Author Vasiliy A. Ponomarjov March 15, 2015
 * @modified Vasiliy A. Ponomarjov March 20, 2015
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
    //подготавливаем переменные
    TApplication::reset()
    //показываем все картинки
    GameField.changeStateAll(HIGH);
    delay(TIME_SHOW);
    //гасим вывод и начинаем обрабатывать нажатия
    GameField.changeStateAll(LOW);  
    state = PLAYING;  
  }
}

/**
 * Функция моргания в режиме ожидания игры 
 */
void TApplication::waiting() {
  static int8_t i = 0;
  //если зажигаем
  if (i<GameField.lengthMatrix()) {
    //зажигаем в случайном порядке и тушим, итак 64 раза
    int8_t r = random(0, GameField.lengthMatrix() - 1);
    GameField.changeStatePin(r, HIGH);
    delay(250);
    i++;
    GameField.changeStatePin(r, LOW);
  } else {
    //зажгем все
    GameField.changeStateAll(HIGH);
    //поморгаем
    GameField.blinking(COUNT_BLINK_WAIT);
    //потушим все
    GameField.changeStateAll(LOW);
    //сбросим счетчик
    i = 0;  
  }
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
    //подготавливаем переменные
    TApplication::reset()
    //первоначальные установки для программирования
    canSave = false;
    lastNumber = 1;                   //начинаем с 1 (0 - это флаг не используемой ячейки)
    GameField.countGameCells = 0;     //сбрасываем число игровых картинок
    //заполняем матрицу нулями
    GameField.fill(1);
    //зажигаем все картинки
    GameField.changeStateAll(HIGH);
    //переходим в режим программирования
    state = PROGRAMMING;
  }
}
//</editor-fold>

#endif