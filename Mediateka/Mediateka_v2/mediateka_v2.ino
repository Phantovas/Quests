/**
   Chudesa.ino - main program file for the game Mediateka
 * @Author Vasiliy A. Ponomarjov November 13, 2015
 * @modified Vasiliy A. Ponomarjov November 16, 2015
 * @email vas@vingrad.ru
*/

//режим отладки
//#define DEBUG

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
  pinMode(START_PIN, INPUT);
  pinMode(PHONE_PIN, INPUT);
  //состояния входных пинов
  digitalWrite(BUSY, HIGH);
  digitalWrite(START_PIN, HIGH);
  digitalWrite(PHONE_PIN, HIGH);
  //активируем антидребезг
  phoneBtn.attach(PHONE_PIN);
  phoneBtn.interval(TIME_BOUNCE_BTN);
  
  //инициализация выходных пинов
  pinMode(LIGHTING_PIN, OUTPUT);
  pinMode(CANDLE_PIN, OUTPUT);
  pinMode(MP_PIN, OUTPUT);
  pinMode(TV_PIN, OUTPUT);
  pinMode(TV_BTN_PIN, OUTPUT);
  //состояния выходных пинов
  digitalWrite(LIGHTING_PIN, LOW);
  digitalWrite(CANDLE_PIN, LOW);
  digitalWrite(MP_PIN, LOW);
  digitalWrite(TV_PIN, LOW);
  digitalWrite(TV_BTN_PIN, HIGH);
  //плеер
	mSerial.begin(9600);
	mp3_set_serial(mSerial); 
  mp3_stop(); //вдруг жестко перенрузили на проигрывании музыки
  //запускаем режим ожидания
  changeState(ST_START);
}  

/**
 * Основная функция 
 */
void loop() {
  if (state == ST_START) {
    //если в режиме ожидания открыли дверь, то включаем свет
    if (digitalRead(START_PIN) == LOW) {
      //включаем освещение 
      changeLight(HIGH); 
      changeCandle(HIGH); 
      changeState(ST_LIGHTING);
    }
  } else {
    switch (state) {
      case ST_LIGHTING: 
        //освещение горит в течение заданного времени (массив timing в vars.h)
        if (millis() - start_time > timing[state] * 1000) {
          //включаем медиаплеер
          digitalWrite(MP_PIN, HIGH);
          changeState(ST_MP_ON);
        }
        break;
      case ST_MP_ON: 
        if (millis() - start_time > timing[state] * 1000) {
          //включаем ТВ
          digitalWrite(TV_PIN, HIGH);
          changeState(ST_TV_ON);
        }
        break;
      case ST_TV_ON: 
        //показываем видеофрагмент в течение заданного времени (массив timing в vars.h)
        if (millis() - start_time > timing[state] * 1000) {
          digitalWrite(TV_BTN_PIN, LOW);
          delay(PRESS_TV_BTN_TIME);
          digitalWrite(TV_BTN_PIN, HIGH);
          changeState(ST_PLAY_VIDEO);
        } 
        break; 
      case ST_PLAY_VIDEO: 
        //показываем видеофрагмент в течение заданного времени (массив timing в vars.h)
        if (millis() - start_time > timing[state] * 1000) {
          //выключаем плеер с телеком
          digitalWrite(MP_PIN, LOW);
          digitalWrite(TV_PIN, LOW);
          changeState(ST_PHONE);
        } 
        break; 
      case ST_PHONE:
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
                phoneMelody = currentMelody; 
                phoneState = PN_PLAY;
              }  
            }
          //воспроизводим
          play(phoneMelody);  
          } else {
            phoneBtn.update();
            if (digitalRead(BUSY) || (phoneBtn.read() == HIGH))  {
              stop();
              currentMelody++; 
              changeState(ST_WAIT);
            }
          }
        } else {
          //тут можно привлечь внимание к телефону
          //может лежит неправильно трубка или ее вообще сняли 
          //ну и воспроизводим занято :)
          phoneBusy(); 
        }
        break;
      case (ST_WAIT): 
        //ожидаем прохождение игры и периодически врубаем телефон
        if (millis() - start_time > timing[ST_PHONE] * 1000) {
          //только если еще не все мелодии пропели
          if (currentMelody < MP3_START_MELODY + MP3_COUNT_MELODY) {  
            changeState(ST_PHONE);
          }
        }
        break;
    }
    //а тут моргаем периодически светом 
    if (stateLight) { //если включен
      if (millis() - light_time > wait_blink_time * 1000 ) 
        changeLight(LOW);
    } else { //если выключен
      if (millis() - light_time > TIME_BLINK * 1000 ) {
        changeLight(HIGH);
        wait_blink_time = random(MIN_BLINK_PERIOD, MAX_BLINK_PERIOD);
      }
    }
    
  }      
  //**************************************************************************
  //ДЛЯ СИЛЬНО ДОТОШНЫХ, ЕСЛИ СХВАТЯТ ТЕЛЕФОН В ЛЮБОЙ МОМЕНТ 
  //БУДЕТ ИГРАТЬ МЕЛОДИЯ ЗАНЯТО :))
  //**************************************************************************
  if (state != ST_PHONE) {
    phoneBusy();
  } 
}
