#include <stdint.h>

int writeRegister(uint8_t addr, uint8_t register, uint8_t data);

uint8_t read8(uint8_t addr, uint8_t register_addr);
uint16_t read16(uint8_t addr, uint8_t register_addr);
uint16_t read16_LE(uint8_t addr, uint8_t register_addr);
uint32_t read24(uint8_t addr, uint8_t register_addr);
uint32_t read32(uint8_t addr, uint8_t register_addr);

void readData(uint8_t addr, uint8_t register_addr, uint8_t *data, uint8_t data_len);