#include "twi_helpers.h"

#include "nrf_drv_twi.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"

extern nrf_drv_twi_t m_twi;

int writeRegister(uint8_t addr, uint8_t reg, uint8_t data)
{
  uint8_t command[2] = {reg, data};
  nrf_drv_twi_tx(&m_twi, addr, command, 2, false);

  return 0;
}

uint8_t read8(uint8_t addr, uint8_t register_addr)
{
  nrf_drv_twi_tx(&m_twi, addr, &register_addr, 1, false);

  uint8_t result = 0;
  nrf_drv_twi_rx(&m_twi, addr, &result, 1);

  return result;
}

uint16_t read16(uint8_t addr, uint8_t register_addr)
{
  nrf_drv_twi_tx(&m_twi, addr, &register_addr, 1, false);

  uint8_t result[2] = {0};
  nrf_drv_twi_rx(&m_twi, addr, &result, 2);

  return (uint16_t)(result[0]) << 8 | (uint16_t)(result[1]);
}

uint16_t read16_LE(uint8_t addr, uint8_t register_addr)
{
  uint16_t temp = read16(addr, register_addr);
  return (temp >> 8) | (temp << 8);
}

uint32_t read24(uint8_t addr, uint8_t register_addr)
{
  nrf_drv_twi_tx(&m_twi, addr, &register_addr, 1, false);

  uint8_t result[3] = {0};
  nrf_drv_twi_rx(&m_twi, addr, &result, 3);

  uint32_t out = 0;
  out |= (uint32_t)(result[0]) << 16 | (uint32_t)(result[1]) << 8 |
         (uint32_t)(result[2]);
  return out;
}

uint32_t read32(uint8_t addr, uint8_t register_addr)
{
  nrf_drv_twi_tx(&m_twi, addr, &register_addr, 1, false);

  uint8_t result[4] = {0};
  nrf_drv_twi_rx(&m_twi, addr, &result, 4);

  return result[0] << 24 | result[1] << 16 | result[2] << 8 | result[3];
}

void readData(uint8_t addr, uint8_t register_addr, uint8_t *data, uint8_t data_len)
{
  nrf_drv_twi_tx(&m_twi, addr, &register_addr, 1, true);
  nrf_drv_twi_rx(&m_twi, addr, data, data_len);
}