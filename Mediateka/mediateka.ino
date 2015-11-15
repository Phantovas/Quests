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
  Serial.println("started");
#endif
  //инициализация входных пинов
  pinMode(BUSY, INPUT);
  pinMode(INDOOR_PIN, INPUT);
  pinMode(PHONE_PIN, INPUT);
  //состояния входных пинов
  digitalWrite(BUSY, HIGH);
  digitalWrite(INDOOR_PIN, HIGH);
  digitalWrite(PHONE_PIN, HIGH);
  //активируем антидребезг
  phoneBtn.attach(PHONE_PIN);
  phoneBtn.interval(TIME_BOUNCE_PROGRAM);
  
  //инициализация выходных пинов
  pinMode(LIGHTING_PIN, OUTPUT);
  pinMode(TV_PIN, OUTPUT);
  pinMode(BOX_PIN, OUTPUT);
  pinMode(OUTDOOR_PIN, OUTPUT);
  //состояния выходных пинов
  digitalWrite(LIGHTING_PIN, LOW);
  digitalWrite(TV_PIN, LOW);
  digitalWrite(BOX_PIN, LOW);
  digitalWrite(OUTDOOR_PIN, LOW);
  //плеер
	mSerial.begin(9600);
	mp3_set_serial(mSerial); 
  mp3_stop(); //вдруг жестко перенрузили на произрывании музыки
  //запускаем режим ожидания
  state = ST_WAIT;
}  

/**
 * Основная функция 
 */
void loop() {
#ifdef DEBUG
//  Serial.print("state ");
//  Serial.println(state);
#endif
  switch (state) {
    case ST_WAIT: 
      //если в режиме ожидания открыли дверь, то переходим в состояние 
      //включение света
      if (digitalRead(INDOOR_PIN) == HIGH) {
        //включаем освещение 
        digitalWrite(LIGHTING_PIN, HIGH); 
        changeState(ST_LIGHTING);
      }
      break;
    case ST_LIGHTING: 
      //освещение горит в течение заданного времени (массив timing в vars.h)
      if (millis() - start_time > timing[state]*1000) {
        //по прошествию заданного времени включается ТВ
        digitalWrite(TV_PIN, HIGH);
        changeState(ST_TV_ON);
      }
      break;
    case ST_TV_ON: 
      //показываем видеофрагмент в течение заданного времени (массив timing в vars.h)
      if (millis() - start_time > timing[state]*1000) {
        changeState(ST_PHONE);
      }
      break; 
    case ST_PHONE:
      //если телефон не в состоянии ЗАНЯТО, то проверяем состояние трубки
      //иначе ничего не делаем, у нас и так по-умолчанию проверка стоит на 
      //снятую трубку
      if (phoneState != PN_BUSY) {
        //если в режиме звонка
        if (phoneState == PN_RING) {
          phoneMelody = MP3_RING; 
          if (phoneBtn.update()) {
            //если подняли трубку
            if (phoneBtn.read() == LOW) {
              phoneMelody = MP3_TEXT; 
              phoneState = PN_PLAY;
              mp3_stop();
            }  
          }
          play(phoneMelody);  
        } else {
          Serial.print("digitalRead(BUSY) ");
          Serial.println(digitalRead(BUSY));
          if (digitalRead(BUSY)) {
            mp3_stop();
            changeState(ST_PHOTOCELL);
          }
        }
      } else {
        //тут можно привлечь внимание к телефону
        //может лежит неправильно трубка или ее вообще сняли 
        //ну и воспроизводим занято :)
        phoneBusy(); 
      }
      break;
    case ST_PHOTOCELL: 
      break;
    case ST_GAME_OVER: 
      mp3_stop(); //вдруг жестко перенрузили на произрывании музыки
      state = ST_WAIT;
  }
  //**************************************************************************
  //ДЛЯ СИЛЬНО ДОТОШНЫХ, ЕСЛИ СХВАТЯТ ТЕЛЕФОН В ЛЮБОЙ МОМЕНТ 
  //БУДЕТ ИГРАТЬ МЕЛОДИЯ ЗАНЯТО :))
  //**************************************************************************
  if (state != ST_PHONE) {
    phoneBusy();
  } 
}

/**
 * Функция изменения состояния с фиксирование времени
 * @param _state
 */
void changeState(TState _state) {
#ifdef DEBUG
      Serial.print("change state");
      Serial.println(_state);
#endif
  start_time = millis(); //время включения
  state = _state; //меняем состояние  
}

/**
 * Функция воспроизведения звука
 * @param _melody номер мелодии
 */
void play(unsigned int _melody) {
  delay(DELAY_MP3_PLAY);
  if (digitalRead(BUSY)) {
    mp3_play(_melody);
  }  
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
