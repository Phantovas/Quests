/*
 * lion.ino - unit for quest "Lion head"
 * @Author Vasiliy A. Ponomarjov Jule 16, 2015
 * @modified Vasiliy A. Ponomarjov Jule 17, 2015
 * @email vas@vingrad.ru
*/

//#define DEBUG

//Include libraries
#include <Bounce2.h>

/******************************************************************************
 * Файл рычания должен быть нулевым, если не надо рычать - то не пишем 
 * его на SD.
 * Остальные файлы должны соответствовать номерам зубов.
 * Полный текст, выводимы при наличии всех зубов, должен быть на единицу больше 
 * количества зубов (если зубов шесть, то файл полного текста 007)
******************************************************************************/

//******************************************************************************
// Пины для управленяи модулем
#define RESET 2            // Сброс, подача LOW на 6 мс
#define BUSY  5            // HIGH во время воспроизведения
#define DOUT  4            // Data input от Arduino
#define DCLK  3            // Data clock от Arduino

#define INIT_PIN 12        //Номер пина активации рычания
#define LED_PIN  13        //Номер пина для светящихся глаз 
byte startTooth = 14;      //Номер пина первого зуба, остальные по порядку

#define COUNT_TOOTH 4      //Число зубов
#define BOUNCE_TIME 50     //Время дребезга 50 мс

//****************************************************************************** 
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
//******************************************************************************

//Переменные
byte selectFile = 255;              //Номер файла для воспроизведения
byte lastPlayingFile = 255;             //Номер воспроизводимого файла   

byte state = false;                 //состояние - не играем по-умолчанию

Bounce teeth[COUNT_TOOTH];          //Массив входных пинов с антидребезгом
byte stateTeeth[COUNT_TOOTH];       //массив состояний входов

/**
 * Функция инициализации
 */
void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif
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
  //вставлены все зубы
  byte allTeeth = 1;  
  //вставлен хотябы один зуб
  byte oneOfTooth = 0;
  //проверяем состояния пинов,
  for (i=0;i<COUNT_TOOTH;i++) {
    //общее состояние пинов вычисляем через логическое И (т.к. не замкнутый пин в режиме HIGH)
    allTeeth &= stateTeeth[i];
    oneOfTooth |= stateTeeth[i];
  }
  //проверяем есть хоть один из зубов
  if (oneOfTooth) {
    state = true;    //уже играем, так что состояние активно, иначе остановим воспроизведение
    //если все зубы
    if (allTeeth) {
      selectFile = COUNT_TOOTH + 1;
#ifdef DEBUG
      Serial.println("***********************");
      Serial.print("allTeeth = ");
      Serial.println(allTeeth);
#endif
    }
  } else {
    state = !digitalRead(INIT_PIN);
    if (state) {
      digitalWrite(LED_PIN, HIGH);     //зажигаем диод
      selectFile = 0;                  //нулевая дорожка на воспроизведение   
    } else {
      digitalWrite(LED_PIN, LOW);      //тушим диод
    }
  }
#ifdef DEBUG  
  Serial.print("selectFile ");
  Serial.println(selectFile);
  Serial.print("lastPlayingFile ");
  Serial.println(lastPlayingFile);
#endif
  //проверяем пины
  for (i=0;i<COUNT_TOOTH;i++) { 
    if (teeth[i].update()) { 
      lastPlayingFile = 255;
      stateTeeth[i] = !teeth[i].read();
      if (stateTeeth[i]) {
        selectFile = i + 1;
      } else {
        selectFile = 255;
      } 
    }  
  }

  //воспроизводим звук
  playSound();

}

void playSound() {
  //долго плата запускает звук, еще не занялся пин BUSY, а мы уже пробегаем по циклу несколько раз
  delay(500);
  //выключаем если общее состояние false
  //иначе воспроизводим файл с выбранным номером
  if (!state) {
    sendCommand(OPCODE_STOP);
    selectFile = 255;
    lastPlayingFile = 255;
  } else {
    //нужно гарантировано воспроизвести файл
    if ((selectFile != lastPlayingFile) && !digitalRead(BUSY)) {
      sendCommand((unsigned int) selectFile);
      sendCommand(OPCODE_PLAY_PAUSE);
      //если мы попали сюда, то либо мы еще играем тот же файл, либо новый
      //но он уже стал текущим
      lastPlayingFile = selectFile;
    } 
  }  
}

/**
 * Функция инициализации кнопок
 * Сначала читаем состояния, а потсм прикручиваем антидребезг
 */
void initInput() {
  //Контакт инициализации
  pinMode(INIT_PIN, INPUT);
  digitalWrite(INIT_PIN, HIGH);
  
  //светодиоды глаз
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  //зубы и их состояния
  for (uint8_t i=0;i<COUNT_TOOTH;i++) {
    //задаем состояния входам и опрашиваем текущие
    pinMode(startTooth + i, INPUT);
    //подтягиваем внутренний резистор, замыкать будем на массу
    //иначе у нас в неподключенном состоянии на пиах будет плавать
    //сигнал с частотой ~ 50 Гц (наводки от сети 220В)
    digitalWrite(startTooth + i, HIGH);
    //и тут же читаем состояние пина
    stateTeeth[i] = !digitalRead(startTooth + i);
    //прикручиваем антидребезг программный
    teeth[i].attach(startTooth + i);
    teeth[i].interval(BOUNCE_TIME);
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