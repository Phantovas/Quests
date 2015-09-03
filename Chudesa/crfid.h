/*
 * gamefield.h - header unit for TGameField class
 * @Author Vasiliy A. Ponomarjov August 31, 2015
 * @modified Vasiliy A. Ponomarjov September 01, 2015
 * @email vas@vingrad.ru
*/

#ifndef CHDS_RFID_H
#define CHDS_RFID_H

/**
 * Структура считывателя карт
 */
struct TRfid {
private:
  OneWire* _ds;
  byte _addr[8];// буфер приема 
public:
  //функции
  TRfid(OneWire* AValue);
  bool avalaible();
  byte* read();
};

/**
 * Constructor
 */
TRfid::TRfid(OneWire* AValue) {
  this->_ds = AValue;
}

/**
 * Читаем метку
 * @return 
 */
bool TRfid::avalaible() {
  //если обнаружено устройттво
  if (this->_ds->reset()) {  
#ifdef DEBUG
    Serial.println("find rfid reading...");
#endif
    //отправляем команду "считать ROM" delay(50); // на всякий случай ждем 
    this->_ds->write(0x33); 
    delay(50);
    //считываем
    for (int i = 0; i < 8; i++)     {
      this->_addr[i] = this->_ds->read();  
    }
    return true;
  } else {
    return false;
  }
}

/**
 * Функция возвращает указатель на считанный буфер
 * @return 
 */
byte* TRfid::read() {
  return this->_addr;
}

#endif