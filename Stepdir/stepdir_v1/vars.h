/**
   vars.h 
 * @Author Vasiliy A. Ponomarjov September 20, 2021
 * @modified Vasiliy A. Ponomarjov September 21, 2021
 * @email vas@vingrad.ru
*/

#ifndef STEPDIR_VARS_H
#define STEPDIR_VARS_H

// ************** Контакты **************
// 1-й шаговый мотор
#define STM1_STEP 2
#define STM1_DIR  3
#define STM1_EN   4

// 2-й шаговый мотор
#define STM2_STEP 5
#define STM2_DIR  6
#define STM2_EN   7

// 3-й шаговый мотор
#define STM3_STEP 8
#define STM3_DIR  9
#define STM3_EN   10

// Реле коллекторного мотора
#define DCM_IN1_PIN 11  // первое реле мотора
#define DCM_IN2_PIN 12  // второе реле мотора

// ************** Макросы *****************
// меняем задержку на 10 мкс, иначе большой скетч не будет успевать тактировать 
#define DRIVER_STEP_TIME 10  

// Если нужен алгоритм ускорения "Модифицированный алгоритм из библиотеки AccelStepper" для библиотеки см. https://alexgyver.ru/gyverstepper/
// то необходимо раскоментировать строку
// #define SMOOTH_ALGORITHM
// максимальная скорость мотора, по возмолжностям  библиотеки 30000, но это 
// в случае, когда нет кода практически никакого. Поэтому отсановимся на 10000 
#define MAX_STEP_SPEED 10000   

// ************** Клавиши ******************
// 1-й мотор
#define STM1_KEY_CW     0x01  //b00000001
#define STM1_KEY_CCW    0x02  //b00000010

// 2-й мотор
#define STM2_KEY_CW     0x04  //b00000100
#define STM2_KEY_CCW    0x08  //b00001000

// 3-й мотор
#define STM3_KEY_CW     0x10  //b00010000
#define STM3_KEY_CCW    0x20  //b00100000


// ************** Переменные **************
// скорости и шани определяем переменными для возможности их изменения в процессе работы скетча
// 1-й шаговый мотор 
long Stm1Steps = 1000;                  // число шагов 1-го мотора
float Stm1Speed = 500;                  // скорость 1-го мотора
bool Stm1Reverse = false;               // направление вращения (true - CW; false- CCW)
GS_runMode Stm1Mode = KEEP_SPEED;       // режим работы движка

GStepper<STEPPER4WIRE> Stm1(2048, 21, 18, 19, 20);
//GStepper<STEPPER4WIRE> Stm1(2048, 17, 15, 16, 14);

// 2-й шаговый мотор 
long Stm2Steps = 1000;                  // число шагов 2-го мотора
float Stm2Speed = 500;                  // скорость 2-го мотора
bool Stm2Reverse = false;               // направление вращения (true - CW; false- CCW)

GStepper<STEPPER4WIRE> Stm2(2048, 17, 15, 16, 14);
// GStepper<STEPPER4WIRE> Stm2(2048, 25, 22, 23, 24);

// 3-й шаговый мотор 
uint8_t Stm3Steps = 1;                   // число шагов 3-го мотора за одно нажатие клавиши
float Stm3Speed = 500;                  // скорость 3-го мотора

// GStepper<STEPPER4WIRE> Stm3(2048, 17, 15, 16, 14);
GStepper<STEPPER4WIRE> Stm3(2048, 28, 25, 26, 27);

// Коллекторный мотор
TRelay DcmIn1(DCM_IN1_PIN, nullptr);    // 1-ое реле управления
TRelay DcmIn2(DCM_IN2_PIN, nullptr);    // 2-ое реле управления

bool DcmCurrentDirection = true;         // текущее направление вращения (true - вперед, FALSE - реверс)
uint16_t DcmTimeForward = 2000;          // время вращения вперед
uint16_t DcmTimeRevers  = 2000;          // время вращения реверсом

#endif