/**
  vars.h - header unit for quest Chudesa
 * @Author Vasiliy A. Ponomarjov August 31, 2015
 * @modified Vasiliy A. Ponomarjov September 01, 2015
 * @email vas@vingrad.ru
*/

/***************ЗАГАДАННОЕ СЛОВО, ДО 16 БУКВ***********************************/
char SECRET_WORD[] = "pfuflrf"; //слово "загадка"
/******************************************************************************/

//Контакты
//сдвиговый регистр
#define LATCH_PIN 8 //Пин подключен к ST_CP входу 74HC595
#define CLOCK_PIN 12 //Пин подключен к SH_CP входу 74HC595
#define DATA_PIN 11 //Пин подключен к DS входу 74HC595
//клавиатура                    
#define KBRD_IRQ_PIN 3 //пин прерывания клавиатуры
#define KBRD_DATA_PIN 2 //пин данных клавиатуры
//звук
#define BUSY 13 //LOW во время воспроизведения
#define S_RX 9 //RX for management player
#define S_TX  10 // TX for management player

int8_t guessed_letters = 0; //угадано букв, по-умолчанию ноль   

//клавиатура
PS2Keyboard Keyboard;
char key;    //нажатая клавиша

//игровое поле
TGameField GameField(SECRET_WORD, LATCH_PIN, CLOCK_PIN, DATA_PIN);

//число попыток
uint8_t count_try = 3;

//плеер
SoftwareSerial mSerial(S_RX, S_TX);      //Инициализация софтверного сериала для управления звуком

