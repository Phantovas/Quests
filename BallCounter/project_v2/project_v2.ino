/*******************************************************************************

  Counter Balls
  project.ino - program for Arduino (328P)
  Created by Vasiliy A. Ponomarjov, February 28, 2015.
  Email: vas@vingrad.ru

 * Алгоритм работы:
 * 1. При поступлении питания +5В на вход INPUT_PORT фиксируется поступление
 * шара, с этого момента отсчитывается время INTERVAL_BOUNCE если в течение
 * этого времени состояние входа датчика изменится на "ложь" то считается что
 * произошло ложное срабатывание и шар не в трубе.
 * 2. Если в течение предыдущего интервала состояние датчика не менялось, то 
 * считается что шар в трубе и с момента получения импульса отсчитывается время
 * INTERVAL_TRANSIT. В этот момент состояние датчика не учитывается, считается 
 * что шар движется по трубе.
 * 3. По проходу 3-х шаров включается время задержки INTERVAL_DELAY
 * 4. По истечении времени задержки переходим в рабочий режим на время 
 * INTERVAL_WORK
 * 5. По окончанию выпонения работы. Переходим в состояние счета шаров. 
 
 * В данном алгоритме для прохода шара выделяется время (INTERVAL_TRANSIT - 
 * INTERVAL_BOUNCE). В указанное время реации на изменение уровня на выходе 
 * датчика не будет и в случае если шар застрял и на выходе датчика высокий 
 * уровень будет считаться что шар еще не прошел трубу и далее счет 
 * прекратится до тех пор пока на выходе датчика не пропадет сигнал 5В. 
 
 ******************************************************************************/

//если при срабатывании низкий уровень, то раскомментировать строку, иначе закомментировать
//#define INVERSE                                 //флаг состояния датчика

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
#define INTERVAL_BOUNCE  100                    //время дребезга (мс)
#define INTERVAL_TRANSIT 500                    //время прохождения шара (мс)
#define INTERVAL_DELAY   3000                   //время ожидания после прохождения 3 шара (мс)   
#define INTERVAL_WORK    300000                 //время работы исполнительного мехнаизма (мс) (5 минут = 5*60*1000)

//рабочие переменные
enum {COUNTING, DELAYING, WORKING} state;       //перечисление работаем или считаем

unsigned long time_full;                        //время получения всех шаров
unsigned long time_start_work;                  //время начала работы
unsigned long time_start_bounce;                //время начала движения шара

uint8_t balls = 0;                              //число посчитанных шаров
bool ball_in_tube = false;                      //флаг наличия шара в трубе 
bool current_state = false;                     //текущее состояние датчика

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
#ifdef DEBUG
  Serial.begin(9600);
#endif
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
}

/**
 * Рабочий цикл
 */
void loop() {
  //проверяем текущее состояние процессора
  switch (state) {
    case COUNTING: {
      //опрашиваем датчик
      current_state = digitalRead(INPUT_PORT);
      //если шарик уже в трубе, то смотрим как он себя ведет 
      //текущее состояние датчика в этом случае нам не важно
      if (ball_in_tube) {
        //проверяем время проверки прохода шара, если прошел, то сбрасываем значения
        if ((millis() - time_start_bounce >= INTERVAL_TRANSIT)) {
#ifdef INVERSE
          ball_in_tube = !current_state;
#else          
          ball_in_tube = current_state;
#endif
          time_start_bounce = 0;
          //если 3 шара, то переходим в состояние ожидания запуска исполнительного механизма
          if (balls == 3) {
            state = DELAYING;
            time_full = millis();
          }
        }
      } else {
        //проверяем на диапазон дребезга, если его уже прошли, то сообщаем оператору
        if ((time_start_bounce != 0) && (millis() - time_start_bounce >= INTERVAL_BOUNCE)) {
          //шарик в трубе
          ball_in_tube = true;
          //увеличиваем счетчик шаров
          balls++;
          //ссобщаем оператору
          if (balls == 1) {
            digitalWrite(OPER_OUT_1, STATE_ON);
          } else if (balls == 2) {
            digitalWrite(OPER_OUT_2, STATE_ON);
          } else if (balls == 3) {
            digitalWrite(OPER_OUT_3, STATE_ON);
          }
        }
        //если датчик активен и время запуска шара не установлено, то устанавливаем
#ifdef INVERSE
        //если уровень 0
        if (!current_state) {
#else
        //если уровень 1  
        if (current_state) {
#endif
          if (time_start_bounce == 0) {
            time_start_bounce = millis();                    
          }
        } else {
          //если в время дребезга изменилось состояние, то это был не шар
          if (millis() - time_start_bounce < INTERVAL_BOUNCE)  
            time_start_bounce = 0;  
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
      //рабочий режим заданное время, по истечении сброс всех флагов
      if (millis() - time_start_work >= INTERVAL_WORK)
        stop();
      break;
    }
  }
}

