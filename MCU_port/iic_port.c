//
// Created by 69566 on 25-8-7.
//

#include "iic_port.h"
void IIC_Init(void) {}
void sda_in(void) {}
void sda_out(void) {}
void IIC_SCL(uint8_t n) { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, n); }
void IIC_SDA(uint8_t n) { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, n); }
uint8_t READ_SDA(void) { return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9); }
void delay(uint32_t i)
{
    while(i)
    {
        i--;
    }
}