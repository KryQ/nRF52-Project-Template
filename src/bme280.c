#include <stdint.h>
#include "bme280.h"

#include "nrf_delay.h"
#include "nrf_drv_twi.h"
#include "nrf_log.h"

#include "twi_helpers.h"

extern nrf_drv_twi_t m_twi;

uint8_t sensor_initialized = 0;
BME280_calibration calibrations = {0};
int32_t t_fine;

int16_t r_16(uint8_t *addr)
{
  return (int16_t)(((uint32_t)addr[1] << 8) | (uint32_t)addr[0]);
}

uint16_t r_u16(uint8_t *addr)
{
  uint16_t result = 0;
  result |= addr[0];
  result |= (addr[1] << 8) & 0xff00;
  return result;
}

uint32_t r_u32(uint8_t *addr)
{
  uint32_t out = 0;
  out |= (uint32_t)(addr[0]) << 16 | (uint32_t)(addr[1]) << 8 |
         (uint32_t)(addr[2]);
  return out;
}

void read_coeffs()
{
  uint8_t data[33] = {0};
  readData(BME280_ADDR, BME280_REG_DIG_T1, data, 26);
  readData(BME280_ADDR, BME280_REG_DIG_H2, &data[26], 7);

  calibrations.dig_T1 = r_u16(&data[0]);
  calibrations.dig_T2 = r_16(&data[2]);
  calibrations.dig_T3 = r_16(&data[4]);

  calibrations.dig_P1 = r_u16(&data[6]);
  calibrations.dig_P2 = r_16(&data[8]);
  calibrations.dig_P3 = r_16(&data[10]);
  calibrations.dig_P4 = r_16(&data[12]);
  calibrations.dig_P5 = r_16(&data[14]);
  calibrations.dig_P6 = r_16(&data[16]);
  calibrations.dig_P7 = r_16(&data[18]);
  calibrations.dig_P8 = r_16(&data[20]);
  calibrations.dig_P9 = r_16(&data[22]);

  calibrations.dig_H1 = data[25];
  calibrations.dig_H2 = r_16(&data[26]);
  calibrations.dig_H3 = data[28];
  uint8_t tmp = data[30];
  calibrations.dig_H4 = ((uint16_t)(data[29] << 4) & 0xff0) | (tmp & 0xf);
  calibrations.dig_H5 = ((uint16_t)(data[31] << 4) & 0xff0) | ((tmp >> 4) & 0xf);
  calibrations.dig_H6 = data[32];
}

uint8_t check_if_sensor_ready()
{
  uint8_t status = read8(BME280_ADDR, BME280_REG_STATUS);
  return (status & (1 << 0)) != 0;
}

void bme280_init()
{
  // Reset chip
  writeRegister(BME280_ADDR, BME280_REG_RESET, 0xB6);

  nrf_delay_ms(2);

  while (check_if_sensor_ready())
  {
    NRF_LOG_INFO("Waiting for sensor");
    nrf_delay_us(500);
  }
  read_coeffs();

  // Enable humidity (write to CTRL_MEAS after!!!)
  writeRegister(BME280_ADDR, BME280_REG_CTRL_HUM, 1);

  // Enable temp and preassure
  writeRegister(BME280_ADDR, BME280_REG_CTRL_MEAS, (1 << 5) | (1 << 2) | 2);

  sensor_initialized = 1;
}

void bme280_start_measurements()
{
  if (!sensor_initialized)
  {
    NRF_LOG_INFO("Sensor not initialized before starting measurement, initializing");
    bme280_init();
  }

  writeRegister(BME280_ADDR, BME280_REG_CTRL_MEAS, (1 << 5) | (1 << 2) | 2);
}

float readTemperature(uint8_t *rawTemp)
{
  int32_t var1, var2;

  int32_t adc_T = r_u32(rawTemp);
  adc_T >>= 4;

  var1 = (int32_t)((adc_T / 8) - ((int32_t)calibrations.dig_T1 * 2));
  var1 = (var1 * ((int32_t)calibrations.dig_T2)) / 2048;
  var2 = (int32_t)((adc_T / 16) - ((int32_t)calibrations.dig_T1));
  var2 = (((var2 * var2) / 4096) * ((int32_t)calibrations.dig_T3)) / 16384;

  t_fine = var1 + var2;

  int32_t T = (t_fine * 5 + 128) / 256;

  return (float)T / 100;
}

float readPressure(uint8_t *rawPress)
{
  int64_t var1, var2, var3, var4;

  int32_t adc_P = r_u32(rawPress);
  adc_P >>= 4;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)calibrations.dig_P6;
  var2 = var2 + ((var1 * (int64_t)calibrations.dig_P5) * 131072);
  var2 = var2 + (((int64_t)calibrations.dig_P4) * 34359738368);
  var1 = ((var1 * var1 * (int64_t)calibrations.dig_P3) / 256) +
         ((var1 * ((int64_t)calibrations.dig_P2) * 4096));
  var3 = ((int64_t)1) * 140737488355328;
  var1 = (var3 + var1) * ((int64_t)calibrations.dig_P1) / 8589934592;

  if (var1 == 0)
  {
    return 0; // avoid exception caused by division by zero
  }

  var4 = 1048576 - adc_P;
  var4 = (((var4 * 2147483648) - var2) * 3125) / var1;
  var1 = (((int64_t)calibrations.dig_P9) * (var4 / 8192) * (var4 / 8192)) /
         33554432;
  var2 = (((int64_t)calibrations.dig_P8) * var4) / 524288;
  var4 = ((var4 + var1 + var2) / 256) + (((int64_t)calibrations.dig_P7) * 16);

  float P = var4 / 256.0;

  return P;
}

float readHumidity(uint8_t *rawHumi)
{
  int32_t var1, var2, var3, var4, var5;

  int32_t adc_H = r_16(rawHumi);

  var1 = t_fine - ((int32_t)76800);
  var2 = (int32_t)(adc_H * 16384);
  var3 = (int32_t)(((int32_t)calibrations.dig_H4) * 1048576);
  var4 = ((int32_t)calibrations.dig_H5) * var1;
  var5 = (((var2 - var3) - var4) + (int32_t)16384) / 32768;
  var2 = (var1 * ((int32_t)calibrations.dig_H6)) / 1024;
  var3 = (var1 * ((int32_t)calibrations.dig_H3)) / 2048;
  var4 = ((var2 * (var3 + (int32_t)32768)) / 1024) + (int32_t)2097152;
  var2 = ((var4 * ((int32_t)calibrations.dig_H2)) + 8192) / 16384;
  var3 = var5 * var2;
  var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
  var5 = var3 - ((var4 * ((int32_t)calibrations.dig_H1)) / 16);
  var5 = (var5 < 0 ? 0 : var5);
  var5 = (var5 > 419430400 ? 419430400 : var5);
  uint32_t H = (uint32_t)(var5 / 4096);

  return (float)H / 1024.0;
}

BME280_output bme280_read_measurements()
{
  // TODO: Timeout
  while (read8(BME280_ADDR, BME280_REG_STATUS) & 0x08)
  {
  }

  BME280_output out;

  uint8_t measurementsRaw[8] = {0};
  readData(BME280_ADDR, BME280_REG_PRESSURE, measurementsRaw, 8);

  out.temperature = readTemperature(&measurementsRaw[3]);
  out.pressure = readPressure(&measurementsRaw[0]);
  out.humidity = readHumidity(&measurementsRaw[5]);

  return out;
}