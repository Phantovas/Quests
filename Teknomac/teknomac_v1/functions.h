/**
 * functions.h - functions file for teknomac
 * Created by Vasiliy A. Ponomarjov, August 09, 2016
 * Email: vas@vingrad.ru
*/

#ifndef FFUNCTIONS_H
#define FFUNCTIONS_H

/**
 * read the buttons
 */
int readBtn() {
  adcKeyIn = analogRead(0);      // read the value from the sensor
  delay(150);                     //ставим задержку на нажатие клавиш
#if (DFROBOT)
  //значения кнопок: 0-вправо, 131-вверх, 308-вниз, 481-влево, 720-выбор
  if (adcKeyIn > 1000) return BTN_NONE; // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 us this threshold
  if (adcKeyIn < 50)   return BTN_RIGHT;
  if (adcKeyIn < 200)  return BTN_UP;
  if (adcKeyIn < 300)  return BTN_DOWN;
  if (adcKeyIn < 500)  return BTN_LEFT;
  if (adcKeyIn < 700)  return BTN_SELECT;
#else
  //значения кнопок: 0-вправо, 131-вверх, 308-вниз, 481-влево, 720-выбор, ??-сброс
  // we add approx 50 to those values and check to see if we are close
  if (adcKeyIn > 1000) return BTN_NONE; // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 us this threshold
  if (adcKeyIn < 50)   return BTN_RIGHT;
  if (adcKeyIn < 250)  return BTN_UP;
  if (adcKeyIn < 450)  return BTN_DOWN;
  if (adcKeyIn < 650)  return BTN_LEFT;
  if (adcKeyIn < 750)  return BTN_SELECT;
#endif
  return BTN_NONE;  // when all others fail, return this...
}


#endif FFUNCTIONS_H