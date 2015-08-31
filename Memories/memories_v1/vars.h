/*
  vars.h - header unit for game Memories
 * @Author Vasiliy A. Ponomarjov March 15, 2015
 * @modified Vasiliy A. Ponomarjov March 20, 2015
 * @email vas@vingrad.ru
*/

//#define DEBUG

//Контакты
//сдвиговый регистр
#define LATCH_PIN               8             //Пин подключен к ST_CP входу 74HC595
#define CLOCK_PIN               12            //Пин подключен к SH_CP входу 74HC595
#define DATA_PIN                11            //Пин подключен к DS входу 74HC595
//кнопки                    
#define START_BTN               14            //Пин для кнопки START (замыкаем на массу) А0
#define PROGRAM_BTN             15            //Пин для кнопки программирования (замыкаем на массу) A1
//клавиатура                    
#define KBRD_IRQ_PIN             3             //пин прерывания клавиатуры
#define KBRD_DATA_PIN            2             //пин данных клавиатуры
//сигнал успешного прохождения         
#define SUCCESS                  7             //успешное выполнение задания
         
//Тайминги         
#define TIME_SHOW                5000          //время показа всех картинок
#define TIME_BOUNCE_START        50            //время антидребезга кнопки старт
#define TIME_BOUNCE_PROGRAM      500           //время реакции кнопки на режим программирования
#define TIME_LIGHT_MISTAKE       1500          //время подсветки картинки при ошибке
#define TIME_OPEN_DOOR           20000         //время открытой двери
         
//Моргания и сообщения         
#define COUNT_BLINK_WAIT         1             //число морганий всеми картинками в режиме ожидания
#define COUNT_BLINK_PROG_ERROR   5             //число морганий при ошибочном ПРОГРАММИРОВАНИИ
#define COUNT_BLINK_SUCCESS      10            //число морганий при успешном завершении игры 


//состояния -  работаем, ожжидаем или программируемся
enum { 
  WAITING,
  PLAYING, 
  PROGRAMMING 
} state;       

//кнопки
Bounce startBtn;      //старт
Bounce programBtn;    //программирование

//игровое поле
TGameField GameField(LATCH_PIN, CLOCK_PIN, DATA_PIN);

//игра 
uint8_t lastNumber;           //последний открытый номер счета картинок
bool isOpenFirstCell;         //первая ячейка пары
int8_t openCell;             //номер открытой ячейки
uint8_t prevOpenCell;         //последняя открытая ячейка
uint8_t countOpenCells;       //количество открытых ячеек

//программирование
bool canSave = false;     //возможно сохранить матрицу

//EEPROM address
enum { 
  EA_CLEAR = 0,
  EA_MATRIX = 10
};

//клавиатура
PS2Keyboard Keyboard;
uint8_t key;  //код нажатой клавиши

//массив соотношения скан кодов клавиш и номеров картинок 
//картинки считаются  с нуля, код -1 - клавиша не используется
int8_t cells[131] = {
  -1,  0, -1,  1,  2,  3,  4,  5,
  -1,  6,  7,  8,  9, 10, 11, -1,
  -1, 12, 13, -1, 14, 15, 16, -1,
  -1,	-1, 17, 18, 19, 20, 21, -1,
  -1, 22, 23, 24, 25, 26, 27, -1,
  -1, 28, 29, 30, 31, 32, 33, -1,
  -1,	30, 31, 32, 33, 34, 35, -1,
  -1,	-1,	36, 37, 38, 39, 40, -1,
  -1, 41, 42, 43, 44, 45, 46, -1,
  -1,	47, 48, 49, 50, 51, 52, -1,
  -1, -1, 53, -1, 54, 55, -1, -1,
  56, 57, 58, 59, -1, 60, -1, -1,
  -1, -1, -1, -1, -1, -1, 61, -1,
  -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, 62, -1, 
  63, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1
};