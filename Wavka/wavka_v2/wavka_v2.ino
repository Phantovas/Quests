//Include libraries
#include <Bounce2.h>

// Пины для управленяи модулем
#define BUSY  5    // HIGH во время воспроизведения
#define DOUT  4    // Data input от Arduino
#define DCLK  3    // Data clock от Arduino
#define RESET 2    // Сброс, подача LOW на 6 мс

// Коды управления модулем
#define OPCODE_PLAY_PAUSE  0xFFFE   // Play a track, or pause the current
                                    //  playback. Note that this requires that
                                    //  you first send a number from 0-511 to
                                    //  the module to indicate which track you
                                    //  wish to play back- simply sending the
                                    //  PLAY_PAUSE opcode has no effect other
                                    //  than pausing playback.
#define OPCODE_STOP        0xFFFF   // Stops the current playback. Subsequent
                                    //  toggling of the PLAY pin will start
                                    //  playback as though no previous track
                                    //  were playing.
#define OPCODE_VOL         0xFFF0   // Set the volume. 0xFFF0 is muted, 0xFFF7
                                    //  is max. Defaults to max on startup.
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
    sendCommand(OPCODE_STOP);
  } else {
    if (selectFile != playingFile) {
      sendCommand((unsigned int) selectFile);
    }
    if (digitalRead(BUSY) == LOW) {
      sendCommand((unsigned int) selectFile);
      sendCommand(OPCODE_PLAY_PAUSE);
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
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, LOW);
  pinMode(DOUT, OUTPUT);
  digitalWrite(DOUT, HIGH);
  pinMode(DCLK, OUTPUT);
  digitalWrite(DOUT, HIGH);
  //input pin
  pinMode(BUSY, INPUT);
  //initial reset
  resetModule();
}

/**
 * Функция сброса модуля
 */
void resetModule() {
  digitalWrite(RESET, LOW);
  delay(100);
  digitalWrite(RESET, HIGH);
  delay(500);
}

/**
 * Основная фуеция отправки команды на исполнение 
 * @param unsigned int номер команды или номер трека
 */
void sendCommand(unsigned int command) {
  digitalWrite(DCLK, LOW);
  delayMicroseconds(1900);
  for (byte i = 0; i < 16; i++)
  {
    delayMicroseconds(100);
    digitalWrite(DCLK, LOW);
    digitalWrite(DOUT, LOW);
    if ((command & 0x8000) != 0)
    {
      digitalWrite(DOUT, HIGH);
    }
    delayMicroseconds(100);
    digitalWrite(DCLK, HIGH);
    command = command<<1;
  }
}
//</editor-fold>