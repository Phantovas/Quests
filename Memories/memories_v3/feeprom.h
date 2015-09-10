/*
 * Library for EEPROM
 * @Author Vasiliy A. Ponomarjov 11 September 2013
 * @modified Vasiliy A. Ponomarjov 15 March 2015
 * @email vas@vingrad.ru
 */

#ifndef FEEPROM_H
#define FEEPROM_H

//byte
void EEPROMWrite(int p_address, uint8_t p_value) {
	EEPROM.write(p_address, p_value);
}

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
uint8_t EEPROMRead(int p_address) {
	return EEPROM.read(p_address);
}

//integer
void EEPROMWriteInt(int p_address, int p_value) {
	byte lowByte = ((p_value >> 0) & 0xFF);
	byte highByte = ((p_value >> 8) & 0xFF);

	EEPROM.write(p_address, lowByte);
	EEPROM.write(p_address + 1, highByte);
}

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
int EEPROMReadInt(int p_address) {
	byte lowByte = EEPROM.read(p_address);
	byte highByte = EEPROM.read(p_address + 1);

	return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}

// ************************************************
// Write floating point values to EEPROM
// ************************************************
void EEPROMWriteDouble(int p_address, double value) {
  byte* p = (byte*)(void*)&value;
  for (int i = 0; i < sizeof(value); i++) {
    EEPROM.write(p_address++, *p++);
  }
}
 
// ************************************************
// Read floating point values from EEPROM
// ************************************************
double EEPROMReadDouble(int p_address) {
  double value = 0.0;
  byte* p = (byte*)(void*)&value;
  for (int i = 0; i < sizeof(value); i++) {
    *p++ = EEPROM.read(p_address++);
  }
  return value;
}

/**
 * Очитска EEPROM
 */
void clearEEPROM() {
  for (int i=0;i<512;i++) {
    EEPROM.write(i, '0xFF');
  }
}

/**
 * Читаем сохраненные значения
 */
void readEEPROM() {
  //массив матрицы
  for (uint8_t i=0; i<64; i++) {
    GameField.Matrix[i] = EEPROMRead(EA_MATRIX + i);
  }
}

/**
 * Сохраняем заводские установки
 */
void saveEEPROM() {
  //флаг заливки епрома
  EEPROMWrite(EA_CLEAR, 1);  
  //массив матрицы
  for (uint8_t i=0; i<64; i++) {
    EEPROMWrite(EA_MATRIX + i, GameField.Matrix[i]);
  }
}

#endif