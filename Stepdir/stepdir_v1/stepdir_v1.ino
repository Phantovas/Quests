/**
   stepdir.ino
 * @Author Vasiliy A. Ponomarjov September 20, 2021
 * @modified Vasiliy A. Ponomarjov September 21, 2021
 * @email vas@vingrad.ru
*/

//#define DEBUG

/**
* Системные модули
*/
#include <Arduino.h>

/**
 * Библиотеки
 */
#include <Bounce2.h>

/**
 * Модули
 */
#include "TRelay.h"
#include "vars.h"
// #include "functions.h"

// переменные для подключения 4 контактов с приемника HY-DJM-5V //
#define RADIO_PIN1 2
#define RADIO_PIN2 3
#define RADIO_PIN3 4
#define RADIO_PIN4 5 

void setup() {
  pinMode(RADIO_PIN1, INPUT);
  pinMode(RADIO_PIN2, INPUT);
  pinMode(RADIO_PIN3, INPUT);
  pinMode(RADIO_PIN4, INPUT);
  
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);

  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
}

void loop() {
  if (digitalRead(RADIO_PIN1) == HIGH) {
    digitalWrite(A0, HIGH);
  }
  if (digitalRead(RADIO_PIN2) == HIGH) {
    digitalWrite(A0, LOW);
  }
  if (digitalRead(RADIO_PIN3) == HIGH) {
    digitalWrite(A1, HIGH);
  }
  if (digitalRead(RADIO_PIN4) == HIGH) {
    digitalWrite(A1, LOW);
  }
}

