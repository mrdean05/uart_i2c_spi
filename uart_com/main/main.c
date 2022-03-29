#include <stdio.h>
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h" 

#define PIN_TXD 5
#define PIN_RXD 6

#define BUF_SIZE_RX 1024

void app_main(void)
{
  uart_config_t uart_config = {
      .baud_rate = 115200,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };

uart_param_config(UART_NUM_1, &uart_config);
uart_set_pin(UART_NUM_1, PIN_TXD, PIN_RXD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
uart_driver_install (UART_NUM_1, BUF_SIZE_RX, 0, 0, NULL, 0);

char outgoing_message[] = "Hello ESP 32";
printf("sending: %s\n", outgoing_message);
uart_write_bytes(UART_NUM_1, outgoing_message, sizeof(outgoing_message));

char incoming_message[BUF_SIZE_RX];
memset(incoming_message, 0, sizeof(incoming_message));
uart_read_bytes(UART_NUM_1, (uint8_t *)incoming_message, BUF_SIZE_RX, pdMS_TO_TICKS(500));
printf("received: %s\n", incoming_message);

}
