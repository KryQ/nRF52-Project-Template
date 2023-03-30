#include <stdint.h>

#define CCS811_REG_APP_STATUS 0x00
#define CCS811_REG_APP_MEAS_MODE 0x01
#define CCS811_REG_APP_ALG_RESULT_DATA 0x02
#define CCS811_REG_APP_RAW_DATA 0x03
#define CCS811_REG_APP_ENV_DATA 0x05
#define CCS811_REG_APP_THRESHOLDS 0x10
#define CCS811_REG_APP_BASELINE 0x11
#define CCS811_REG_APP_HW_ID 0x20
#define CCS811_REG_APP_HW_VERSION 0x21
#define CCS811_REG_APP_FW_BOOT_VERSION 0x23
#define CCS811_REG_APP_FW_APP_VERSION 0x24
#define CCS811_REG_APP_INTERNAL_STATE 0xA0
#define CCS811_REG_APP_ERROR_ID 0xE0

#define CCS811_REG_BOOT_STATUS 0x00
#define CCS811_REG_BOOT_HW_ID 0x20
#define CCS811_REG_BOOT_HW_VERSION 0x21
#define CCS811_REG_BOOT_FW_BOOT_VERSION 0x23
#define CCS811_REG_BOOT_FW_APP_VERSION 0x24
#define CCS811_REG_BOOT_APP_ERASE 0xF1
#define CCS811_REG_BOOT_APP_DATA 0xF2
#define CCS811_REG_BOOT_APP_VERIFY 0xF3
#define CCS811_REG_BOOT_APP_START 0xF4

#define CCS811_REG_SW_RESET 0xFF

#define CCS811_ADDR 0x5A

typedef struct
{
  uint16_t eCO;
  uint16_t eTVOC;
} CCS811_output;

void ccs811_init();
CCS811_output ccs811_read_data();
void ccs811_send_env_data(float temp, float humidity);