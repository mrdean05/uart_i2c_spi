#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_types.h"
#define SDA_GPIO 26
#define SCL_GPIO 25
#define LM75A_ADDRESS 0X48


void app_main(void)
{   
    i2c_config_t i2c_config ={
        .mode= I2C_MODE_MASTER,
        .sda_io_num = SDA_GPIO,
        .scl_io_num = SCL_GPIO, 
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000};

i2c_param_config(I2C_NUM_0, &i2c_config);
i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);

uint8_t raw_value[2];
i2c_cmd_handle_t cmd_handle = i2c_cmd_link_create();
i2c_master_start (cmd_handle);
i2c_master_write_byte(cmd_handle, (LM75A_ADDRESS << 1) | I2C_MASTER_READ, true);
i2c_master_read(cmd_handle, (uint8_t *)&raw_value, 2, I2C_MASTER_ACK);
i2c_master_stop(cmd_handle);
i2c_master_cmd_begin (I2C_NUM_0, cmd_handle, 1000/ portTICK_PERIOD_MS);
i2c_cmd_link_delete (cmd_handle);

bool isNeg = false;
if (raw_value[0] & 0x80)
    {
        isNeg = true;
        raw_value[0] = raw_value[0] & 0x7f;
    }

int16_t data = (raw_value[0] << 8 | raw_value[1] >> 5);
float temperature = (data * 0.125) * (isNeg ? -1: 1);
printf("temperature %f\n", temperature);

}
