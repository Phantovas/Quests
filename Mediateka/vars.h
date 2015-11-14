/**
  vars.h - header unit for quest Mediateka
 * @Author Vasiliy A. Ponomarjov November 13, 2015
 * @modified Vasiliy A. Ponomarjov November 13, 2015
 * @email vas@vingrad.ru
*/

/***************НОМЕРА МЕЛОДИЙ*************************************************/
#define MP3_RING 1 //мелодия звонка телефона
#define MP3_TEXT 2 //мелодия текста
#define MP3_BUSY 3 //мелодия занятия телефона
#define MP3_BOX  4 //мелодия шкатулки
/******************************************************************************/

/******************** КОНТАКТЫ ************************************************/
//звук
#define BUSY 13 //LOW во время воспроизведения
#define S_RX 9 //RX for management player
#define S_TX  10 // TX for management player
//пин входных состояний
#define INDOOR_PIN 2 //пин для открытия двери
#define PHOTOCELL_PIN 14 //пин для подключения фотоэлемента
//пины кправления механизмами
#define LIGHTING_PIN 3 //пин освещения
#define TV_PIN 4 //пин для включения ТВ
#define PHONE_PIN 5 //пин поднятия трубки
#define BOX_PIN 6 //пин открытия шкатулки
#define OUTDOOR_PIN 7 //пин открытия первой двери

//плеер
SoftwareSerial mSerial(S_RX, S_TX);      //Инициализация софтверного сериала для управления звуком

//состояния игры (запуск, освещение, включение ТВ, звонок, подсветка фотоэлемента, конец игры)
typedef enum {ST_WAIT, ST_LIGHTING, ST_TV_ON, ST_PHONE, ST_PHOTOCELL, ST_GAME_OVER} TState; 

TState state;

//времена работы состояний
unsigned long timing[6] = {0, 120, 105, 0, 0, 0};

//время запуска события
unsigned long start_time = 0;

