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
#include "functions.h"

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
Serial.println(analogRead(PHOTOCELL_PIN));  
#endif
  switch (state) {
    case ST_WAIT: 
      //если в режиме ожидания открыли дверь, то переходим в состояние 
      //включение света
      if (digitalRead(INDOOR_PIN) == HIGH) {
        //включаем освещение 
        lighting(HIGH); 
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
      /*************************************************************************
       * Внимание!!! Если начато воспроизведение текста в трубку, то положив ее 
       * он не остановится, а воспроизведется до конца.
       */
      //если телефон  в состоянии  НЕ ЗАНЯТО, то проверяем состояние трубки
      //иначе ничего не делаем, у нас и так по-умолчанию проверка стоит на 
      //снятую трубку
      if (phoneState != PN_BUSY) {
        //если в режиме звонка
        if (phoneState == PN_RING) {
          //выбираем мелодию звонка
          phoneMelody = MP3_RING; 
          //проверяем кнопку
          if (phoneBtn.update()) {
            //если подняли трубку
            if (phoneBtn.read() == LOW) {
              stop();
              phoneMelody = MP3_TEXT; 
              phoneState = PN_PLAY;
            }  
          }
        //воспроизводим
        play(phoneMelody);  
        } else {
          if (digitalRead(BUSY)) {
            stop();
            changeState(ST_LIGHT_BLINK);
          }
        }
      } else {
        //тут можно привлечь внимание к телефону
        //может лежит неправильно трубка или ее вообще сняли 
        //ну и воспроизводим занято :)
        phoneBusy(); 
      }
      break;
    case ST_LIGHT_BLINK:
      //моргаем освещением
      blinking(DELAY_BLINK);
      if (millis() - start_time > timing[state]*1000) {
        //включаем свет, а то мало-ли как времена у нас установлены
        lighting(HIGH);
        changeState(ST_PHOTOCELL);
      }
      break;
    case ST_PHOTOCELL: 
      //если посветили на фотоэлемент
      if (analogRead(PHOTOCELL_PIN) > SENSITIVITY) {
        changeState(ST_GAME_OVER);
      } else {
        if (millis() - start_time > timing[state]*1000) {
          blinking(DELAY_BLINK);
        }
      }
      break;
    case ST_GAME_OVER: 
      digitalWrite(BOX_PIN, HIGH);
      digitalWrite(OUTDOOR_PIN, HIGH);
      lighting(LOW);
  }
  //**************************************************************************
  //ДЛЯ СИЛЬНО ДОТОШНЫХ, ЕСЛИ СХВАТЯТ ТЕЛЕФОН В ЛЮБОЙ МОМЕНТ 
  //БУДЕТ ИГРАТЬ МЕЛОДИЯ ЗАНЯТО :))
  //**************************************************************************
  if (state != ST_PHONE) {
    phoneBusy();
  } 
}
