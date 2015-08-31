/*
 * lift.ino - main unit for game "Lift"
 * @Author Vasiliy A. Ponomarjov 
 * @modified Vasiliy A. Ponomarjov August 9, 2015
 * @email vas@vingrad.ru
*/

/**
* Системные модули
*/
#include <Arduino.h>
#include <Bounce2.h>
/**
 * Классы
 */
#include "tlift.h"

#define PIN_LIFT 9                     //привод лифта
#define PIN_LIFT_STOP 6                //пин состояния двигателя

#define PIN_RELAY_ENTER 12             //замок входной двери
#define PIN_RELAY_EXIT 8               //замок выходной двери
#define PIN_MOVE_DOWN 4                //индикатор движения вниз
#define PIN_MOVE_UP   5                //индикатор движения вверх
#define PIN_BTN_DOWN  10               //кнопка вниз
#define PIN_BTN_UP    7                //кнопка вверх
#define PIN_GAME_OVER 3                //конец игры

#define LIFT_MOVE_TIME  30000           //время движения лифта
#define OPEN_DOOR_TIME  10000          //время открытых дверей 
#define BTN_BOUNCE_TIME 50             //антидребезг кнопок 

TLift Lift(PIN_LIFT, LIFT_MOVE_TIME, PIN_LIFT_STOP); //лифт 
//TLift Lift(PIN_LIFT, LIFT_MOVE_TIME); //лифт - такая запись используется в случае если не будет использоваться пин остановки 

Bounce btnDown;                        //кнопка вниз
Bounce btnUp;                          //кнопка вверх 

bool gameOver = false;                //конец игры 


void setup() {
  Serial.begin(9600);

  //лифт
  pinMode(PIN_LIFT, OUTPUT);
  digitalWrite(PIN_LIFT, LOW);
  Lift.ledDown = PIN_MOVE_DOWN;
  Lift.ledUp = PIN_MOVE_UP;
  pinMode(PIN_LIFT_STOP, INPUT);
  digitalWrite(PIN_LIFT_STOP, HIGH);  

  //двери лифта
  pinMode(PIN_RELAY_ENTER, OUTPUT);
  pinMode(PIN_RELAY_EXIT, OUTPUT);
  //обе двери открыты
  digitalWrite(PIN_RELAY_ENTER, HIGH);
  digitalWrite(PIN_RELAY_EXIT, HIGH);
  
  //индикаторы движения вверх/вниз
  pinMode(PIN_MOVE_DOWN, OUTPUT);
  pinMode(PIN_MOVE_UP, OUTPUT);
  //обе двери погашены
  digitalWrite(PIN_MOVE_DOWN, LOW);
  digitalWrite(PIN_MOVE_UP, LOW);
  
  //кнопки с антидребезгом
  pinMode(PIN_BTN_DOWN, INPUT);
  pinMode(PIN_BTN_UP, INPUT);
  digitalWrite(PIN_BTN_DOWN, HIGH);
  digitalWrite(PIN_BTN_UP, HIGH);
  //включаем антидребезг программный
  btnDown.attach(PIN_BTN_DOWN);
  btnUp.attach(PIN_BTN_UP);
  btnUp.interval(BTN_BOUNCE_TIME);  
  btnUp.interval(BTN_BOUNCE_TIME);  
  
  //пин конца игры
  pinMode(PIN_GAME_OVER, INPUT);
  digitalWrite(PIN_GAME_OVER, HIGH);

  //ЕСЛИ НУЖНА ПРОВЕРКА НА ВЕРХНЮЮ ТОЧКУ ТО РАСКОММЕНТИРУЙ БЛОК
  //если концевик двигателя не замкнут на массу, 
  //то возвращаем двигатель в первоначальное состояние
//  if (Lift.getStateStopPin()) {
//    Lift.goUp();
//  }
}

void loop() {
  //если лифт не движется, то работаем, иначе просто ждем
  switch (Lift.getLevel()) {
    case 0: 
      Serial.println("LEVEL 0");
      if (btnDown.update()) {
        if (!btnDown.read()) {
          Serial.println("PRESS BTN DOWN");
          //едем вниз
          Lift.goDown();
          //открываем выходную дверь
          digitalWrite(PIN_RELAY_EXIT, LOW);
          //ждем некоторое время и блокируем дверь
          delay(OPEN_DOOR_TIME);
          //блокируем выходную дверь
          digitalWrite(PIN_RELAY_EXIT, HIGH);
        }
      }
      break;
    case 1: 
      Serial.println("LEVEL 1");
      if (!digitalRead(PIN_GAME_OVER)) {
        gameOver = true;
        //открываеи выходную дверь
        digitalWrite(PIN_RELAY_EXIT, LOW);
        //ждем некоторое время и блокируем дверь
        delay(OPEN_DOOR_TIME);
        //блокируем выходную дверь
        digitalWrite(PIN_RELAY_EXIT, HIGH);
      }
      //если закончили игру
      if (gameOver) {      
        if (btnUp.update()) {
          if (!btnUp.read()) {
            Serial.println("PRESS BTN UP");
            //если едем вверх, то снимаем флаг окончания игры
            gameOver = false;
            //едем вниз
            Lift.goUp();
            //открываем входную дверь
            digitalWrite(PIN_RELAY_ENTER, LOW);
            //ждем некоторое время и блокируем дверь
            delay(OPEN_DOOR_TIME);
            //блокируем входную дверь
            digitalWrite(PIN_RELAY_ENTER, HIGH);
          }
        }
      }
      break;
  }
  
}