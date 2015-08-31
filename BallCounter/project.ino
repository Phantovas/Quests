/*******************************************************************************

  Counter Balls
  project.ino - program for Arduino (328P)
  Created by Vasiliy A. Ponomarjov, February 28, 2015.
  Email: vas@vingrad.ru
 
 ******************************************************************************/

#define DEBUG

#define STATE_OFF   LOW                         //выключено (//выключено (низкий уровень)  низкий уровень)  
#define STATE_ON    HIGH                        //включено (высокий уровень)

//входы
#define INPUT_PORT  3                           //вход счетчика
//выходы
#define OPER_OUT_1  8                           //первый выход для оператора
#define OPER_OUT_2  9                           //второй выход для оператора
#ifdef DEBUG
#define OPER_OUT_3  13                          //третий выход для оператора
#else
#define OPER_OUT_3  10                          //третий выход для оператора
#endif
#define WORK_PORT   11                          //выход исполнительного механизма
//константы задержек
#define INTERVAL_BOUNCE  1000                    //время дребезга (мс)
#define INTERVAL_TRANSIT 2000                    //время прохождения шара (мс)
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
  digitalWrite(INPUT_PORT, LOW);  
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
#ifdef DEBUG
      Serial.println(" ");
      Serial.print("COUNTING ");
      Serial.println(current_state);
      Serial.println(time_start_bounce);
#endif
      //если шарик уже в трубе, то смотрим как он себя ведет 
      //текущее состояние датчика в этом случае нам не важно
      if (ball_in_tube) {
        //проверяем время проверки прохода шара, если прошел, то сбрасываем значения
        if ((millis() - time_start_bounce >= INTERVAL_TRANSIT)) {
          ball_in_tube = current_state;
          time_start_bounce = 0;
          //если 3 шара, то переходим в состояние ожидания запуска исполнительного механизма
          if (balls == 3) {
            state = DELAYING;
            time_full = millis();
          }
        }
#ifdef DEBUG
        Serial.print("ball in tube ");
        Serial.println(millis() - time_start_bounce);
#endif
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
#ifdef DEBUG
        Serial.print("not ball in tube ");
        Serial.println(millis() - time_start_bounce);
#endif
        //если датчик активен и время запуска шара не установлено, то устанавливаем
        if (current_state) {
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
#ifdef DEBUG
      Serial.println(" ");
      Serial.print("DELAYING ");
#endif
      //если задержка больше заданной, то запуск механизма, диоды не тушим
      if (millis() - time_full >= INTERVAL_DELAY)
        start();
      break;
    }
    case WORKING: {
#ifdef DEBUG
      Serial.println(" ");
      Serial.print("WORKING ");
#endif
      if (millis() - time_start_work >= INTERVAL_WORK)
        stop();
      break;
    }
  }
}

