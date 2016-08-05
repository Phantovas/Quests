/*
 * Variables
 */

//DONE: -o Vas Разобраться с режимом растопки. доступен ли он будте в режиме работы?
//TODO: -o Vas Вынести код в cpp файлы, а описание оставить в header фалах.
//DONE: -o Vas Сделать флаг перерисовки главного экрана, если true - перерисовать и переключить на false

#ifndef VARS_H
#define VARS_H

#define ST_WORK "Working "
#define ST_STOP "Stopped "
#define ST_DISABLE "Disable "

//константы
#define BTN_RIGHT  0
#define BTN_UP     1
#define BTN_DOWN   2
#define BTN_LEFT   3
#define BTN_SELECT 4
#define BTN_NONE   5

//screens
#define SCR_COUNT 4

//pinout

//interrupts
#define ioCLOSE_BANK  0         //прерывание датчика буфера на 2 цифровом порту


//counter
unsigned long previousMillis;        //время предыдущего опроса

//general vars
bool _mainScreen = true;      //показывать главный экран
int8_t _numScreen = 0;        //номер текущего экрана
bool _repaintScreen = true;   //флаг перерисовки главного экрана, сразу истина, чтобы нарисоваь первый экран
bool _working = false;        //флаг работы, по-умолчанию false
bool _critical = false;       //флаг достижения критической температуры
//actuators flag
bool _wAuger = false;        //работа шнека
bool _wSpiral = false;       //работа списрали шнека
bool _wFan = false;          //работа вентилятора
bool _wPump = false;         //работа насоса
volatile bool _wBank = false; //открыт бункер

//setup vars
//типы топлива и его описание
char *strRastopka = "Rastopka";
char *strStopped = "Can stop?";
char *strClear = "Clear EEPROM?";
uint8_t typeFuel = 0;
char *strTypefuel = "Vid topliva";
char *strFuel[3] = { "Soloma", "Opilki", "Othody"};
char *strActuator[5] = {"AUGER ", "FAN ", "PUMP ", "AUGER SPIRAL ", "BUNKER "};
char *state[6] = {"OFF ", "ON ", "CLOSE ", "OPEN ", "NO ", "YES "};
//температуры
int tmpAuger = 80;                               //шнек (5..254)
char *strTmpAuger = "Temp. shneka";
const uint8_t c_tmpAugerMin = 10;
const uint8_t c_tmpAugerMax = 95;

int tmpFeed = 50;                                               //подача (0..90)
char *strTmpFeed = "Temp. podachi";
const uint8_t c_tmpFeedMin = 0;
const uint8_t c_tmpFeedMax = 90;

int tmpFeedCritical = 80;                                       //критическая подача (0..98)
char *strTmpFeedCritical = "Temp. critical";
const uint8_t c_tmpFeedCriticalMin = 0;
const uint8_t c_tmpFeedCriticalMax = 98;

int tmpReturn =  30;                                              //обратка (10..60)
char *strTmpReturn = "Temp. obratki";
const uint8_t c_tmpReturnMin = 10;
const uint8_t c_tmpReturnMax = 60;

//настройки
uint8_t speedFanStart = 65;                              //вентилятор минимальная скорость (0..254)
char *strFanSpeed = "Speed fan start";
const uint8_t c_speedFanStartMin = 0;
const uint8_t c_speedFanStartMax = 100;

uint8_t fanKinding = 75;                                  //вентилятор при растопке (0..254)
char *strFankinding = "Speed fan rastopki";
const uint8_t c_fanKindingMin = 0;
const uint8_t c_fanKindingMax = 100;

int hysteresis = 5;                              //гистерезис (0..45)
char *strHysteresis = "Hysteresis";
const uint8_t c_hysteresisMin = 0;
const uint8_t c_hysteresisMax = 45;

int tmWorkAuger[3] = {180, 180, 180};            //время вращения шнека по типу топлива (0..2^32)
int tmUnworkAuger[3] = {60, 60, 60};              //время простоя шнека
const int c_tmWorkAugerMin = 0;
const int c_tmWorkAugerMax = 24*60*60;               //в секундах сутки

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