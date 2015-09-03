/*
 * functions.h - header unit of functions for game Chudesa
 * @Author Vasiliy A. Ponomarjov September 02, 2015
 * @modified Vasiliy A. Ponomarjov September 02, 2015
 * @email vas@vingrad.ru
*/

#ifndef CHDS_FUNCTIONS_H
#define CHDS_FUNCTIONS_H

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
  delay(100);
  if (AWait) {
    while (!digitalRead(BUSY)) {
      //пустой цикл для ожидания 
    } 
#ifdef DEBUG
  Serial.println(" stop playing");
#endif
  }  
}


/**
 * Начало игры
 */
void startGame() {
#ifdef DEBUG
  Serial.print("start game ");
#endif
  prevKey = 0; //предыдущая нажатая клавиша
  guessed_letters = 0; //число угаданных букв
  GameField.reset(); //сбрасываем состояния поля
  mp3_stop(); //останавливаем звук, если софтовый ресет, то он не останавливается сам
  count_try = 0; //число попыток 0
  digitalWrite(DOOR_PIN, LOW); //закрываем дверь
  //говорим приглашение к новой игре
  playSound(MP3_INTRO, false);
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

/**
 * Функция поиска адреса считанного ключа в массиве валидных карт
 * Если один и тот же адрес ключа будет в двух строках, то вернется БОЛЬШЕЕ ЗНАЧЕНИЕ
 */
uint8_t getTryCount(byte* address) {
  uint8_t result = 0;
  //ищем карту
  for (uint8_t i=0;i<CARD_COUNT;i++ ) {
    byte lres = 1;
    for (uint8_t j=0;j<8;j++) {
      lres &= address[j] == validCard[i][j];
    }
#ifdef DEBUG
  Serial.print("find card in array ");  
  Serial.println(lres);
#endif
    if (lres) {
      result = i;
    }
  }
  return result;
}
#endif