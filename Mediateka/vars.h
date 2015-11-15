/**
  vars.h - header unit for quest Mediateka
 * @Author Vasiliy A. Ponomarjov November 13, 2015
 * @modified Vasiliy A. Ponomarjov November 15, 2015
 * @email vas@vingrad.ru
*/

/*************** ЗНАЧЕНИЯ ПЕРЕМЕННЫХ ******************************************/
#define TIME_BOUNCE_PROGRAM      500           //время реакции кнопки на режим программирования
#define DELAY_MP3_PLAY           50            //задержка на воспроизведение звука
#define DELAY_BLINK              1000          //длительность свечения света при моргании в мс
#define SENSITIVITY              1000           //чувтствительность фотоэлемента
/******************************************************************************/

/***************НОМЕРА МЕЛОДИЙ*************************************************/
#define MP3_RING 1 //мелодия звонка телефона
#define MP3_TEXT 6 //мелодия текста
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
#define PHONE_PIN 3 //пин поднятия трубки
#define PHOTOCELL_PIN 14 //пин для подключения фотоэлемента
//пины кправления механизмами
#define LIGHTING_PIN 4 //пин освещения
#define TV_PIN 5 //пин для включения ТВ
#define BOX_PIN 6 //пин открытия шкатулки
#define OUTDOOR_PIN 7 //пин открытия первой двери

//плеер
SoftwareSerial mSerial(S_RX, S_TX);      //Инициализация софтверного сериала для управления звуком

//состояния игры (запуск, освещение, включение ТВ, звонок, моргание света, подсветка фотоэлемента, конец игры)
typedef enum {ST_WAIT, ST_LIGHTING, ST_TV_ON, ST_PHONE, ST_LIGHT_BLINK, ST_PHOTOCELL, ST_GAME_OVER} TState; 
TState state;

//времена работы состояний {0, 120, 105, 0, 20, 0, 0} 0 - не используется
unsigned long timing[7] = {0, 120, 105, 0, 20, 10, 0};

//время запуска события
unsigned long start_time = 0;

//состояние света
bool lightState = false;
//время включения/отключения освещения
unsigned long lightOnOffTime = 0;
bool _blinking = false;
unsigned long _blink_period;

//антидребезг кнопок
Bounce phoneBtn; //кнопка поднятия телефонной трубки

//состояния телефона
enum {PN_RING, PN_PLAY, PN_BUSY} phoneState;
//текущая мелодия
unsigned int phoneMelody = MP3_RING;
