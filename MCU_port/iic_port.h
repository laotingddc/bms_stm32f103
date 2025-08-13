//
// Created by 69566 on 25-8-7.
//

#ifndef IIC_PORT_H
#define IIC_PORT_H
#include "bsp_iic.h"
#include "main.h"
#include "stdint.h"

typedef enum
{
    BQ769X0_IIC_DEVICE,
    AT24CXX_IIC_DEVICE,
    IIC_DEVICE_NUM
}iic_device_num_e;
typedef struct iic_device_t
{
    char *name;
    i2c_driver_t *driver;
    uint8_t (*read)(i2c_driver_t *dev, uint8_t dev_addr, uint16_t ReadAddr, uint8_t *buf, uint8_t len);
    uint8_t (*write)(i2c_driver_t *dev, uint8_t dev_addr,uint16_t ReadAddr,uint8_t *buf, uint16_t len);
}iic_device_t;
iic_device_t* iic_device_find(const char* name);
#endif // IIC_PORT_H
