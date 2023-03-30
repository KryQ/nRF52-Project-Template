#define BME280_REG_DIG_T1 0x88
#define BME280_REG_DIG_T2 0x8A
#define BME280_REG_DIG_T3 0x8C

#define BME280_REG_DIG_P1 0x8E
#define BME280_REG_DIG_P2 0x90
#define BME280_REG_DIG_P3 0x92
#define BME280_REG_DIG_P4 0x94
#define BME280_REG_DIG_P5 0x96
#define BME280_REG_DIG_P6 0x98
#define BME280_REG_DIG_P7 0x9A
#define BME280_REG_DIG_P8 0x9C
#define BME280_REG_DIG_P9 0x9E

#define BME280_REG_DIG_H1 0xA1
#define BME280_REG_DIG_H2 0xE1
#define BME280_REG_DIG_H3 0xE3
#define BME280_REG_DIG_H4 0xE4
#define BME280_REG_DIG_H5 0xE5
#define BME280_REG_DIG_H6 0xE7

#define BME280_REG_ID 0xD0
#define BME280_REG_RESET 0xE0
#define BME280_REG_CTRL_HUM 0xF2
#define BME280_REG_STATUS 0xF3
#define BME280_REG_CTRL_MEAS 0xF4
#define BME280_REG_CONFIG 0xF5
#define BME280_REG_PRESSURE 0xF7
#define BME280_REG_TEMPERATURE 0xFA
#define BME280_REG_HUMIDITY 0xFD
#define BME280_ADDR 0x76

typedef struct
{
  uint16_t dig_T1; ///< temperature compensation value
  int16_t dig_T2;  ///< temperature compensation value
  int16_t dig_T3;  ///< temperature compensation value

  uint16_t dig_P1; ///< pressure compensation value
  int16_t dig_P2;  ///< pressure compensation value
  int16_t dig_P3;  ///< pressure compensation value
  int16_t dig_P4;  ///< pressure compensation value
  int16_t dig_P5;  ///< pressure compensation value
  int16_t dig_P6;  ///< pressure compensation value
  int16_t dig_P7;  ///< pressure compensation value
  int16_t dig_P8;  ///< pressure compensation value
  int16_t dig_P9;  ///< pressure compensation value

  uint8_t dig_H1; ///< humidity compensation value
  int16_t dig_H2; ///< humidity compensation value
  uint8_t dig_H3; ///< humidity compensation value
  int16_t dig_H4; ///< humidity compensation value
  int16_t dig_H5; ///< humidity compensation value
  int8_t dig_H6;  ///< humidity compensation value
} BME280_calibration;

typedef struct
{
  float temperature;
  float pressure;
  float humidity;
} BME280_output;

void bme280_init();
void bme280_start_measurements();
BME280_output bme280_read_measurements();