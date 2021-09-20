/**
   vars.h 
 * @Author Vasiliy A. Ponomarjov September 20, 2021
 * @modified Vasiliy A. Ponomarjov September 21, 2021
 * @email vas@vingrad.ru
*/

//************** Контакты **************
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

//Реле коллекторного мотора
#define DCM_IN1_PIN 11  //первое реле мотора
#define DCM_IN2_PIN 12  //второе реле мотора

//************** Переменные **************
//1-й шаговый мотор 
uint16_t Stm1Steps = 200;    //число шагов 1-го мотора
uint16_t Stm1Speed = 1000;   //скорость 1-го мотора

//2-й шаговый мотор 
uint16_t Stm2Steps = 200;    //число шагов 2-го мотора
uint16_t Stm2Speed = 1000;   //скорость 2-го мотора

//3-й шаговый мотор 
uint16_t Stm3Speed = 1000;   //скорость 3-го мотора

//Коллекторный мотор
TRelay DcmIn1(DCM_IN1_PIN, nullptr);    //1-ое реле управления
TRelay DcmIn2(DCM_IN2_PIN, nullptr);    //2-ое реле управления

bool DcmCurrentDirection = true;         //текущее направление вращения (true - вперед, FALSE - реверс)
uint16_t DcmTimeForward = 2000;          //время вращения вперед
uint16_t DcmTimeRevers  = 2000;          //время вращения реверсом
