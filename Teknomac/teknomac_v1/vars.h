/*
 * Variables
 */

//DONE: -o Vas Разобраться с режимом растопки. доступен ли он будте в режиме работы?
//TODO: -o Vas Вынести код в cpp файлы, а описание оставить в header фалах.
//DONE: -o Vas Сделать флаг перерисовки главного экрана, если true - перерисовать и переключить на false

#ifndef VARS_H
#define VARS_H

//константы
#define BTN_RIGHT  0
#define BTN_UP     1
#define BTN_DOWN   2
#define BTN_LEFT   3
#define BTN_SELECT 4
#define BTN_NONE   5

//screens
#define SCR_COUNT 4

//constants
#define TIME_BOUNCE_BTN     100         //время антидребезга кнопок в мс

//pinouts input
#define STOPBTN_PIN         0           //пин конпки Стоп (незамедлительная остановка)
#define STARTBTN_PIN        1           //пин главной кнопки Старт
#define FEED_PIN            2           //датчик подачи
#define EMISSION_PIN        3           //датчик выброса
#define SHAFT_DOWN_PIN      15          //датчик вала подачи, нижнее положение
#define SHAFT_UP_PIN        16          //датчик вала подачи, верхнее положение
#define IRON_DOWN_PIN       17          //датчик утюга, нижнее положение
#define IRON_UP_PIN         18          //датчик утюга, верхнее положение
//pinouts output    
#define MOTOR_PIN           10          //вывод двигателей вращения
#define SHAFTPNEVMO_PIN     11          //вывод пневмоцилиндров вала подачи
#define EMISSIONPNEVMO_PIN  12          //вывод пневмоцилиндров выброса

//timers
unsigned long previousMillis;           //время предыдущего опроса

//general vars
bool _mainScreen = true;      //показывать главный экран
int8_t _numScreen = 0;        //номер текущего экрана
bool _repaintScreen = true;   //флаг перерисовки главного экрана, сразу истина, чтобы нарисоваь первый экран
bool _working = false;        //флаг выполнения длительной операции (тестирование или работа))

//setup vars
//состояния -  работаем, ожидаем или программируемся
enum { 
  TESTING, //в режиме теста
  WAITING, //работаем, но в режиме ожидания багета
  WORKING, //работаем
  STOPED   //остановлен принудительно (возможно что переход в режим работы только через тест) 
} state;  

//антидребезг кнопок
Bounce startBtn;              //кнопка запуска рабочего цикла
Bounce stopBtn;               //кнопка остановки рабочего цикла

//define some values used by the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
enum {LCD_COL = 16, LCD_ROW = 2};

//define some values used by the menu
LiquidMenu menu(LCD_COL, LCD_ROW);

//keyboard
int clickKey  = 0;
int adcKeyIn  = 0;

//array address for eeprom
enum { EA_CLEAR = 0,
       EA_STATE = EA_CLEAR + 1,
       EA_TYPEFUEL = 10
    };


#endif