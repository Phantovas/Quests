/**
  vars.h - header unit for quest Mediateka
 * @Author Vasiliy A. Ponomarjov November 13, 2015
 * @modified Vasiliy A. Ponomarjov November 13, 2015
 * @email vas@vingrad.ru
*/

/***************НОМЕРА МЕЛОДИЙ*************************************************/
//число попыток должны соответствовать названиям файлов 
#define MP3_INTRO 4 //мелодия приветсвия
#define MP3_CALL_LETTER 5 //мелодия просьбы назвать букву
#define MP3_FOUND_LETTER 6 //открыта буква
#define MP3_NOTFOUND_LETTER 7 //нет такой буквы
/******************************************************************************/

/******************** КОНТАКТЫ ************************************************/
//звук
#define BUSY 13 //LOW во время воспроизведения
#define S_RX 9 //RX for management player
#define S_TX  10 // TX for management player
//пин для двери
#define DOOR_PIN 4 //пин для открытия двери

//плеер
SoftwareSerial mSerial(S_RX, S_TX);      //Инициализация софтверного сериала для управления звуком