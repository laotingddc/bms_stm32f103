//
// Created by 69566 on 25-8-7.
//

#include "iic_port.h"
#include <string.h>
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
void IIC_Init_bq769x0(void) {}
void sda_in_bq769x0(void) {}
void sda_out_bq769x0(void) {}
void IIC_SCL_bq769x0(uint8_t n) { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, n); }
void IIC_SDA_bq769x0(uint8_t n) { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, n); }
uint8_t READ_SDA_bq769x0(void) { return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9); }
void delay(uint32_t i)
{
    while(i)
    {
        i--;
    }
    while(i)
    {
        i--;
    }
}
/*************************************************************/
i2c_driver_interface_t bq769x0_i2c_ops = {
    .pf_i2c_init    = IIC_Init_bq769x0,
    .pf_i2c_sda_in  = sda_in_bq769x0,
    .pf_i2c_sda_out = sda_out_bq769x0,
    .pf_set_scl     = IIC_SCL_bq769x0,
    .pf_set_sda     = IIC_SDA_bq769x0,
    .pf_get_sda     = READ_SDA_bq769x0,
};
delay_interface_t delay_interface = {
    .pf_delay_us = delay,
};
static i2c_driver_t bq769x0_driver ={
    .pf_i2c_interface_inst= &bq769x0_i2c_ops,
    .pf_delay_interface_inst = &delay_interface,
};

iic_device_t iic_device[] = {
    [BQ769X0_IIC_DEVICE] = {
        .name = "bq769x0",
        .driver = &bq769x0_driver,
        .read =iic_read_bytes,
        .write = iic_write_bytes,
    }
};
const uint32_t iic_device_count = sizeof(iic_device) / sizeof(iic_device[0]);

iic_device_t* iic_device_find(const char* name)
{
    if (name == NULL) {
        return NULL;
    }
    for (uint32_t i = 0; i < ARRAY_SIZE(iic_device); i++) {
        // 使用strcmp进行字符串比较
        if (strcmp(iic_device[i].name, name) == 0) {
            return &iic_device[i];
        }
    }
    
    return NULL; // 未找到设备
}