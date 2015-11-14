/**
   Chudesa.ino - main program file for the game Mediateka
 * @Author Vasiliy A. Ponomarjov November 13, 2015
 * @modified Vasiliy A. Ponomarjov November 13, 2015
 * @email vas@vingrad.ru
*/

#define DEBUG

/**
* Системные модули
*/

/**
 * Библиотеки
 */
#include <Bounce2.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <OneWire.h>

/**
 * Модули
 */
#include "vars.h"

/**
 * Функция установки значений
 */
void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif
  //состояния входных пинов
  pinMode(INDOOR_PIN, INPUT);
  digitalWrite(INDOOR_PIN, HIGH);
  
  //состояния выходных пинов
  pinMode(LIGHTING_PIN, OUTPUT);
  digitalWrite(LIGHTING_PIN, LOW);
  pinMode(TV_PIN, OUTPUT);
  digitalWrite(TV_PIN, LOW);
  pinMode(PHONE_PIN, OUTPUT);
  digitalWrite(PHONE_PIN, LOW);
  pinMode(BOX_PIN, OUTPUT);
  digitalWrite(BOX_PIN, LOW);
  pinMode(OUTDOOR_PIN, OUTPUT);
  digitalWrite(OUTDOOR_PIN, LOW);
  //запускаем режим ожидания
  state = ST_WAIT;
}  

/**
 * Основная функция 
 */
void loop() {
#ifdef DEBUG
  Serial.print("state ");
  Serial.println(state);
#endif
  switch (state) {
    case ST_WAIT: {
      //если в режиме ожидания открыли дверь, то переходим в состояние 
      //включение света
      if (digitalRead(INDOOR_PIN) == HIGH) {
        digitalWrite(LIGHTING_PIN, HIGH); //включаем освещение
        changeState(ST_LIGHTING);
      }
      break;
    };
    case ST_LIGHTING: {
#ifdef DEBUG
  Serial.print("time ");
  Serial.println(millis() - start_time);
  Serial.println(timing[state]*1000);
  Serial.println(millis() - start_time >  timing[state]*1000);
#endif
      if (millis() - start_time > timing[state]*1000) {
        digitalWrite(TV_PIN, HIGH);
        changeState(ST_TV_ON);
      }
      break;
    };
    case ST_TV_ON: {
      break; 
    };
    case ST_PHONE: {
      break;
    };
    case ST_PHOTOCELL: {
      break;
    };
    case ST_GAME_OVER: {
      state = ST_WAIT;
    }
  }
}

/**
 * Функция изменения состояния с фиксирование времени
 * @param _state
 */
void changeState(TState _state) {
  start_time = millis(); //время включения
  state = _state; //меняем состояние  
}