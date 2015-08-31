/*******************************************************************************

  Counter Balls
  project.ino - program for Arduino (328P)
  Created by Vasiliy A. Ponomarjov, February 28, 2015.
  Email: vas@vingrad.ru

 * Алгоритм работы:
 * 1. При поступлении питания +5В на вход INPUT_PORT фиксируется поступление
 * шара, с этого момента отсчитывается время INTERVAL_TRANSIT при этом не важно
 * каким является состояние датчика, шар уже зафиксирован.
 * 2. По проходу 3-х шаров включается время задержки INTERVAL_DELAY
 * 3. По истечении времени задержки переходим в рабочий режим на время 
 * INTERVAL_WORK
 * 4. По окончанию выпонения работы. Переходим в состояние счета шаров.
 
 * В данном алгоритме считается что шар гарантировано проходит трубу за время
 * INTERVAL_TRANSIT. В случае застревания шара, по истечении указанного
 * времени будет считаться что пришел следующий шар и так по кругу.
 
 ******************************************************************************/

//если при срабатывании низкий уровень, то раскомментировать строку, иначе закомментировать
#define INVERSE                                 //флаг состояния датчика

#define STATE_OFF   LOW                         //выключено (//выключено (низкий уровень)  низкий уровень)  
#define STATE_ON    HIGH                        //включено (высокий уровень)

//входы
#define INPUT_PORT  3                           //вход счетчика
//выходы
#define OPER_OUT_1  8                           //первый выход для оператора
#define OPER_OUT_2  9                           //второй выход для оператора
#define OPER_OUT_3  10                          //третий выход для оператора
#define WORK_PORT   11                          //выход исполнительного механизма
//константы задержек
#define INTERVAL_TRANSIT 500                    //время прохождения шара (мс)
#define INTERVAL_DELAY  3000                    //время ожидания после прохождения 3 шара (мс)   
#define INTERVAL_WORK   300000                  //время работы исполнительного мехнаизма (мс) (5 минут = 5*60*1000)

//рабочие переменные
enum {COUNTING, DELAYING, WORKING} state;       //перечисление работаем или считаем

unsigned long time_full;                        //время получения всех шаров
unsigned long time_start_work;                  //время начала работы
unsigned long time_start_bounce;                //время начала движения шара

uint8_t balls = 0;                              //число посчитанных шаров
bool _state_ball = false;                       //флаг наличия шара в трубе 

/**
 * Функция включения механизма
 */
void start() {
  digitalWrite(WORK_PORT, STATE_ON);            //включаем выход
  time_start_work = millis();                   //запоминаем время старта
  state = WORKING;                              //переходим в рабочий режим
}

/**
 * Функция выключени и перехода в состояние счета
 */
void stop() {
  balls = 0;                                    //обнуляем счетчик шаров
  digitalWrite(OPER_OUT_1, STATE_OFF);          //отключаем выходы
  digitalWrite(OPER_OUT_2, STATE_OFF);
  digitalWrite(OPER_OUT_3, STATE_OFF);
  digitalWrite(WORK_PORT, STATE_OFF);
  time_start_work = 0;                          //обнуляем время старта
  state = COUNTING;                             //переходим в состояние счета    
}

/**
 * Инициализация
 */
void setup() {
  //инициализируем выходы
  pinMode(INPUT_PORT, INPUT);
  pinMode(OPER_OUT_1, OUTPUT);
  pinMode(OPER_OUT_2, OUTPUT);
  pinMode(OPER_OUT_3, OUTPUT);
  pinMode(WORK_PORT, OUTPUT); 
  //первоначальные состояния
#ifdef INVERSE
  digitalWrite(INPUT_PORT, HIGH);
#else  
  digitalWrite(INPUT_PORT, LOW);
#endif
  digitalWrite(OPER_OUT_1, STATE_OFF);
  digitalWrite(OPER_OUT_2, STATE_OFF);
  digitalWrite(OPER_OUT_3, STATE_OFF);
  digitalWrite(WORK_PORT, STATE_OFF);
  //инициализируем переменные
  time_full = time_start_work = time_start_bounce = 0;
  state = COUNTING;
  //небольшая задержка для инициализации времени
  delay(100);
}

/**
 * Рабочий цикл
 */
void loop() {
  //проверяем текущее состояние процессора
  switch (state) {
    case COUNTING: {
      //чичтаем состояние датчика
      _state_ball = digitalRead(INPUT_PORT);
      //если время время начала прохождения шара = 0 
      if (time_start_bounce == 0) {  
        //если шар пришел
#ifdef INVERSE
        //если уровень 0
        if (!_state_ball) { 
#else
        //если уровень 1  
        if (_state_ball)  { 
#endif
          balls++;                               //увеличиваем счетчик шаров     
          time_start_bounce = millis();          //запоминаем время начала движения
        }
      } else {
        if ((millis() - time_start_bounce >= INTERVAL_TRANSIT)) {
          time_start_bounce = 0;                 //обнуляем время начала движения шара 
          if (balls == 1) {
            digitalWrite(OPER_OUT_1, STATE_ON);
          } else if (balls == 2) {
            digitalWrite(OPER_OUT_2, STATE_ON);
          } else if (balls == 3) {
            digitalWrite(OPER_OUT_3, STATE_ON);
            state = DELAYING;
            time_full = millis();
          }
        }
      } 
      break;
    }
    case DELAYING: {
      //если задержка больше заданной, то запуск механизма, диоды не тушим
      if (millis() - time_full >= INTERVAL_DELAY)
        start();
      break;
    }
    case WORKING: {
      if (millis() - time_start_work >= INTERVAL_WORK)
        stop();
      break;
    }
  }
}

