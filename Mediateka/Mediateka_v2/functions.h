/*
 * functions.h - header unit of functions for game Chudesa
 * @Author Vasiliy A. Ponomarjov November 15, 2015
 * @modified Vasiliy A. Ponomarjov November 15, 2015
 * @email vas@vingrad.ru
*/

#ifndef MDTK_FUNCTIONS_H
#define MDTK_FUNCTIONS_H


/**
 * Функция изменения состояния с фиксирование времени
 * @param _state
 */
void changeState(TState _state) {
#ifdef DEBUG
      Serial.print("change state ");
      Serial.println(_state);
#endif
  start_time = millis(); //время включения
  state = _state; //меняем состояние  
}

/**
 * Функция включения/выключения освещения
 * @param _state
 */
void changeLight(bool _state) {
  if (stateLight != _state) {
    stateLight = _state;
    light_time = millis();
    digitalWrite(LIGHTING_PIN, _state); 
  };
}

/**
 * Функция включения/выключения свечи
 * @param _state
 */
void changeCandle(bool _state) {
  if (stateCandle != _state) {
    stateCandle = _state;
    candle_time = millis();
    digitalWrite(CANDLE_PIN, _state); 
  };
}

/**
 * Функция воспроизведения звука
 * @param _melody номер мелодии
 */
void play(unsigned int _melody) {
  if (digitalRead(BUSY)) {
    mp3_play(_melody);
    delay(DELAY_MP3_PLAY);
  }  
}

/**
 * Останавливаме воспроизведение
 */
void stop() {
  mp3_stop();
  delay(DELAY_MP3_PLAY);
}

/**
 * Функция проверки состояния телефона
 */
void phoneBusy() {
    phoneBtn.update();
    //состояние телефона
    if (phoneBtn.read() == HIGH) {
      phoneState = PN_RING;
      mp3_stop();
    } else {
      phoneState = PN_BUSY;
      //играем в телефон занято
      play(MP3_BUSY);
    }     
}

#endif