#include "ccs811.h"
#include "twi_helpers.h"

#include "nrf_delay.h"
#include "nrf_drv_twi.h"
#include "nrf_log.h"

extern nrf_drv_twi_t m_twi;

void ccs811_init()
{
  uint8_t command[6] = {CCS811_REG_SW_RESET, 0x11, 0xE5, 0x72, 0x8A};
  nrf_drv_twi_tx(&m_twi, CCS811_ADDR, command, 5, false);

  nrf_delay_ms(1);

  uint8_t start[1] = {CCS811_REG_BOOT_APP_START};
  nrf_drv_twi_tx(&m_twi, CCS811_ADDR, start, 1, false);

  writeRegister(CCS811_ADDR, CCS811_REG_APP_MEAS_MODE, 0b00010000);
}

void ccs811_send_env_data(float temp, float humidity)
{
  uint16_t hum_conv = humidity * 512.0f + 0.5f;
  uint16_t temp_conv = (temp + 25.0f) * 512.0f + 0.5f;

  uint8_t buf[] = {
      CCS811_REG_APP_ENV_DATA,
      (uint8_t)((hum_conv >> 8) & 0xFF), (uint8_t)(hum_conv & 0xFF),
      (uint8_t)((temp_conv >> 8) & 0xFF), (uint8_t)(temp_conv & 0xFF)};

  nrf_drv_twi_tx(&m_twi, CCS811_ADDR, buf, 5, false);
}

// TODO: read status of measurement
CCS811_output ccs811_read_data()
{
  uint8_t data[6] = {0};

  readData(CCS811_ADDR, CCS811_REG_APP_ALG_RESULT_DATA, data, 6);

  CCS811_output output = {0};
  output.eCO = (uint16_t)(data[0] << 8) | data[1];
  output.eTVOC = (uint16_t)(data[2] << 8) | data[3];

  return output;
}