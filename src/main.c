#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_temp.h"
#include "app_error.h"
#include "bsp.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

int main(void)
{
    /* Configure board. */
    bsp_board_init(BSP_INIT_LEDS);

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    /* Toggle LEDs. */
    while (true)
    {
        NRF_LOG_INFO("Actual temperature: %d", 0);
        NRF_LOG_ERROR("O kurwens, dupka");
        nrf_delay_ms(1500);
        NRF_LOG_FLUSH();
    }
}
