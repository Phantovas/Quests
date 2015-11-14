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
/******************************************************************************/

/******************** КОНТАКТЫ ************************************************/
//звук
#define BUSY 13 //LOW во время воспроизведения
#define S_RX 9 //RX for management player
#define S_TX  10 // TX for management player
//пин для двери
#define DOOR_PIN 4 //пин для открытия двери
#define TV_PIN 3 //пин для включения ТВ
#define PHONE_PIN 5 //пин поднятия трубки
#define PHOTOCELL_PIN 6 

//плеер
SoftwareSerial mSerial(S_RX, S_TX);      //Инициализация софтверного сериала для управления звуком

//состояния игры (запуск, освещение, включение ТВ, звонок, подсветка фотоэлемента, конец игры)
enum {ST_START, ST_LIGHTING, ST_TV_ON, ST_PHONE, ST_PHOTOCELL, ST_GAME_OVER} state; 


