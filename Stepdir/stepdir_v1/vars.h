/**
   vars.h 
 * @Author Vasiliy A. Ponomarjov September 20, 2021
 * @modified Vasiliy A. Ponomarjov September 23, 2021
 * @email vas@vingrad.ru
*/

#ifndef STEPDIR_VARS_H
#define STEPDIR_VARS_H

// ************** Макросы *****************
// меняем задержку на 10 мкс, иначе большой скетч не будет успевать тактировать 
// #define DRIVER_STEP_TIME 10  

// Если нужен алгоритм ускорения "Модифицированный алгоритм из библиотеки AccelStepper" для библиотеки см. https://alexgyver.ru/gyverstepper/
// то необходимо раскоментировать строку
// #define SMOOTH_ALGORITHM

// максимальная скорость мотора, по возмолжностям  библиотеки 30000, но это 
// в случае, когда нет кода практически никакого. Поэтому отсановимся на 10000 
#define MAX_STEP_SPEED 10000   

//ускорение для всех моторов
#define STEP_ACCELEREATION 500

// диапазон времн вращения коллекторного двигателя (мс)
#define DCM_MIN_PERIOD      500       
#define DCM_MAX_PERIOD      20000

// ************** Контакты **************

//! ПИН 2 не занимать на него вешается RCSwitch !

// 1-й шаговый мотор
#define STM1_STEP_PIN 4
#define STM1_DIR_PIN  5

// 2-й шаговый мотор
#define STM2_STEP_PIN 6
#define STM2_DIR_PIN  7

// 3-й шаговый мотор
#define STM3_STEP_PIN 8
#define STM3_DIR_PIN  9

// Реле коллекторного мотора
#define DCM_IN1_PIN 11                  // первое реле мотора
#define DCM_IN2_PIN 12                  // второе реле мотора

// ************** Клавиши ******************
// 1-й мотор
#define STM1_KEY_MODE   0x01            //b00000001
#define STM1_KEY_CCW    0x02            //b00000010
#define STM1_KEY_CW     0x04            //b00000100

// 2-й мотор          
#define STM2_KEY_CW     0x08            //b00001000
#define STM2_KEY_CCW    0x10            //b00010000

// 3-й мотор          
#define STM3_KEY_CW     0x20            //b00100000
#define STM3_KEY_CCW    0x40            //b01000000

// коллекторный мотор
#define DCM_KEY_CW      0x80            //b10000000
#define DCM_KEY_CCW     0x100           //b000100000000

// ************** Переменные **************
// скорости и шаги определяем переменными для возможности их изменения в процессе работы скетча

// 1-й шаговый мотор 
long Stm1StepsAuto = 1000;              // число шагов 1-го мотора в автоматическом режиме
long Stm1StepsManual = 1;               // число шагов 1-го мотора в ручном режиме
float Stm1Speed = 500;                  // скорость 1-го мотора
bool Stm1Reverse = false;               // направление вращения (true - CW; false- CCW)
GS_runMode Stm1Mode = FOLLOW_POS;       // режим работы движка

// GStepper< STEPPER2WIRE> Stm1([STEPS], STM1_STEP_PIN, STM1_DIR_PIN); // STEPS - количество шагов на один оборот вала для расчётов с градусами
// GStepper<STEPPER4WIRE> Stm1(2048, 21, 18, 19, 20);
GStepper<STEPPER4WIRE> Stm1(2048, 17, 15, 16, 14);

// 2-й шаговый мотор 
long Stm2Steps = 1000;                  // число шагов 2-го мотора
float Stm2Speed = 500;                  // скорость 2-го мотора
bool Stm2Reverse = false;               // направление вращения (true - CW; false- CCW)

// GStepper< STEPPER2WIRE> Stm2([STEPS], STM2_STEP_PIN, STM2_DIR_PIN); // STEPS - количество шагов на один оборот вала для расчётов с градусами
// GStepper<STEPPER4WIRE> Stm2(2048, 17, 15, 16, 14);
GStepper<STEPPER4WIRE> Stm2(2048, 25, 22, 23, 24);

// 3-й шаговый мотор 
uint8_t Stm3Steps = 1;                   // число шагов 3-го мотора за одно нажатие клавиши
float Stm3Speed = 500;                   // скорость 3-го мотора

// GStepper< STEPPER2WIRE> Stm3([STEPS], STM3_STEP_PIN, STM3_DIR_PIN); // STEPS - количество шагов на один оборот вала для расчётов с градусами
// GStepper<STEPPER4WIRE> Stm3(2048, 17, 15, 16, 14);
GStepper<STEPPER4WIRE> Stm3(2048, 29, 26, 27, 28);

// Коллекторный мотор
bool DcmCurrentDirection;                 // текущее направление вращения (true - вперед, FALSE - реверс)
uint16_t DcmTimeForward = 5000;           // время вращения вперед
uint16_t DcmTimeBackward  = 10000;        // время вращения реверсом

TRelay DcmIn1(DCM_IN1_PIN, nullptr);      // 1-ое реле управления
TRelay DcmIn2(DCM_IN2_PIN, nullptr);      // 2-ое реле управления

// пульт управления !ИСПОЛЬЗУЕТ ПРЕРЫВАНИЕ НА ПИНЕ 2!
RCSwitch mainSwitch = RCSwitch(); 

#endif