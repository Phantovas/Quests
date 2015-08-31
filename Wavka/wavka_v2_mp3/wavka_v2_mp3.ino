//Include libraries
#include <Bounce2.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

// Пины для управленяи модулем
#define BUSY  5                          // LOW во время воспроизведения
#define S_RX  10                         // RX for management player
#define S_TX  11                         // TX for management player

//Прочие устаноки
#define COUNT_INPUT        6        //Число опрашиваемых входов >0 и <255
#define BOUNCE_TIME        50       //Время дребезга 50 мс
#define DELAY_PLAY_TIME    50       //время для запуска воспроизведения файла 
                                    //плата долго думает, ну видимо ищет файл, потом его читает
                                    //а только потом начинает воспроизводить.
                                    //Если задержку не поставить, то микроконтроллер успевает запустить 
                                    //файл на воспроизведение, а на следующем шаге его уже поставить на паузу
                                    //т.к. код команды и воспроизведения один OPCODE_PLAY_PAUSE. 
                                    //Надо ждать время равное DELAY_PLAY_TIME

//Переменные
byte selectFile = 0;                //Номер файла для воспроизведения
byte playingFile = 0;                //Номер воспроизводимого файла    
byte startPin = 14;                 //Номер первого опрашиваемого пина

Bounce inputPin[COUNT_INPUT];       //Массив входных пинов с антидребезгом
byte stateInputPin[COUNT_INPUT];    //массив состояний входов

SoftwareSerial mSerial(S_RX, S_TX);      //Инициализация софтверного сериала для управления звуком

/**
 * Функция инициализации
 */
void setup() {
  //Serial.begin(9600);
  //инициализируем входы
  initInput();
  //инициализируем модуль
  initModule();
}

/**
 * Основной цикл
 */
void loop() {
  uint8_t i;
  //состояние всех пинов путем операции логического И
  byte allState = 1; 
  //проверяем состояния пинов,
  for (i=0;i<COUNT_INPUT;i++) {
    //если пин замунт на массу, то запоминаем номе файла он будет равен порядковому номеру пина
    if (!stateInputPin[i]) 
      selectFile = i;
    //общее состояние пинов вычисляем через логическое И (т.к. не замкнутый пин в режиме HIGH)
    allState &= stateInputPin[i];
  }
  //долго плата запускает звук, еще не занялся пин BUSY, а мы уже пробегаем по циклу несколько раз
  //и успеваем пгставить воспроизведение на паузу или вообще как повезет
  delay(DELAY_PLAY_TIME);
  //выключаем если общее состояние HIGH(1)
  //иначе воспроизводим файл с выбранным номером
  if (allState) {
    mp3_stop();
  } else {
    if (selectFile != playingFile) {
      mp3_play((unsigned int) selectFile);
    }
    if (digitalRead(BUSY) == HIGH) {
      mp3_play((unsigned int) selectFile);
    }
    //если мы попали сюда, то либо мы еще играем тот же файл, либо новый
    //но он уже стал текущим
    playingFile = selectFile;
  }
  //пока ищраем надо проверить не переключили случаем какой-нибудь из пинов
  for (i=0;i<COUNT_INPUT;i++) { 
    if (inputPin[i].update()) 
      stateInputPin[i] = inputPin[i].read();
  }
  
}

/**
 * Функция инициализации кнопок
 * Сначала читаем состояния, а потсм прикручиваем антидребезг
 */
void initInput() {
  //перебираем все заданные пины
  for (uint8_t i=0;i<COUNT_INPUT;i++) {
    //задаем состояния входам и опрашиваем текущие
    pinMode(startPin + i, INPUT);
    //подтягиваем внутренний резистор, замыкать будем на массу
    //иначе у нас в неподключенном состоянии на пиах будет плавать
    //сигнал с частотой ~ 50 Гц (наводки от сети 220В)
    digitalWrite(startPin + i, HIGH);
    //и тут же читаем состояние пина
    stateInputPin[i] = digitalRead(startPin + i);
    //прикручиваем антидребезг программный
    inputPin[i].attach(startPin + i);
    inputPin[i].interval(BOUNCE_TIME);
  }
}


//<editor-fold defaultstate="collapsed" desc="MOdule functions">
/**
 * Функция инициализации модуля
 * установки занчений пинов и его сброс
 */
void initModule() {
  //output pin
	mSerial.begin(9600);
	mp3_set_serial(mSerial); //назначаем порт управления для модуля	
  //input pin
  pinMode(BUSY, INPUT);
  digitalWrite(BUSY, HIGH);
}
//</editor-fold>