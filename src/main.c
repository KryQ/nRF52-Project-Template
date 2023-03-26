#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "app_uart.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "bsp.h"
#if defined(UART_PRESENT)
#include "nrf_uart.h"
#endif
#if defined(UARTE_PRESENT)
#include "nrf_uarte.h"
#endif

void uart_error_handle(app_uart_evt_t *p_event)
{

    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}

int main(void)
{
    uint32_t err_code = 0;
    // Simply to prevent unused var error
    err_code++;
    /* Configure board. */
    bsp_board_init(BSP_INIT_LEDS);

    /**
     * P(0,17) as RX
     * P(0,20) as TX
     * no flow control pin
     * no flow control pin
     * flow control disabled
     * parity disabled
     * baud
     */
    const app_uart_comm_params_t uart_params = {
        17, 20, -1, -1,
        APP_UART_FLOW_CONTROL_DISABLED,
        false,
        NRF_UART_BAUDRATE_115200};

    APP_UART_FIFO_INIT(&uart_params, 256, 256, uart_error_handle, APP_IRQ_PRIORITY_LOW, err_code);

    printf("\r\nUART example started.\r\n");

    /* Toggle LEDs. */
    while (true)
    {
        printf("Hello World\r\n");
        nrf_delay_ms(1500);
    }
}
