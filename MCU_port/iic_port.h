//
// Created by 69566 on 25-8-7.
//

#ifndef IIC_PORT_H
#define IIC_PORT_H
#include "main.h"
#include "stdint.h"
void    IIC_Init(void);
void    sda_in(void);
void    sda_out(void);
void    IIC_SCL(uint8_t n);
void    IIC_SDA(uint8_t n);
uint8_t READ_SDA(void);
void delay(uint32_t i);
#endif // IIC_PORT_H
