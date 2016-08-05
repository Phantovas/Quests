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

//counter
unsigned long previousMillis;        //время предыдущего опроса

//general vars
bool _mainScreen = true;      //показывать главный экран
int8_t _numScreen = 0;        //номер текущего экрана
bool _repaintScreen = true;   //флаг перерисовки главного экрана, сразу истина, чтобы нарисоваь первый экран

//setup vars
//состояния -  работаем, ожжидаем или программируемся
enum { 
  TESTING,
  WORKING, 
  STOPED 
} state;  

//char vars
char *strClear = "Clear EEPROM?";
char *strState[6] = {"OFF ", "ON ", "CLOSE ", "OPEN ", "NO ", "YES "};

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