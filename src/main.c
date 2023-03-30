#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "bme280.h"
#include "twi_helpers.h"

uint8_t tx_data[] = {0xd0};
const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(0);

void twi_init(void)
{
    const nrf_drv_twi_config_t twi_lm75b_config = {
        .scl = 13,
        .sda = 15,
        .frequency = NRF_DRV_TWI_FREQ_100K}; // CHANGE IT TO 400k or more

    APP_ERROR_CHECK(nrf_drv_twi_init(&m_twi, &twi_lm75b_config, NULL, NULL));

    nrf_drv_twi_enable(&m_twi);
}

int main(void)
{
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    NRF_LOG_INFO("TWI sensor example started.");

    twi_init();
    bme280_init();

    while (true)
    {
        bme280_start_measurements();
        BME280_output bme_output = bme280_read_measurements();

        NRF_LOG_INFO("Temp: %d", bme_output.temperature * 100);
        NRF_LOG_INFO("Press: %d", bme_output.pressure * 100);
        NRF_LOG_INFO("Humi: %d", bme_output.humidity * 100);

        nrf_delay_ms(1000);
        NRF_LOG_FLUSH();
    }
}
