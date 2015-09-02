/*
 * functions.h - header unit of functions for game Chudesa
 * @Author Vasiliy A. Ponomarjov September 02, 2015
 * @modified Vasiliy A. Ponomarjov September 02, 2015
 * @email vas@vingrad.ru
*/

#ifndef CHDS_FUNCTIONS_H
#define CHDS_FUNCTIONS_H

/**
 * Начало игры
 */
void startGame() {
#ifdef DEBUG
  Serial.println("start game ");
#endif
  prevKey = 0; //предыдущая нажатая клавиша
  guessed_letters = 0; //число угаданных букв
  GameField.reset(); //сбрасываем состояния поля
}

/**
 * Функция воспроизведения трека
 * @param Track
 * @param Sync
 */
void playSound(uint8_t ATrack, bool AWait) { 
#ifdef DEBUG
  Serial.print("start playing "); Serial.print(ATrack);
#endif
  mp3_play(ATrack);
  delay(50);
  if (AWait) {
    while (!digitalRead(BUSY)) {
      //пустой цикл для ожидания 
    } 
#ifdef DEBUG
  Serial.println("stop playing");
#endif
  }  
}

/**
 * Функция очистки буфера клавиатуры
 */
void clearPSBuffer() {
  char _key;
  //читаем клавиши
  while (Keyboard.available()) {
    _key = Keyboard.read(); 
  }  
}

#endif