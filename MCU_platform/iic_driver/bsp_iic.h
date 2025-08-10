//
// Created by 69566 on 25-8-7.
//

#ifndef BSP_IIC_H
#define BSP_IIC_H

#include "iic_port.h"

#include <stdint.h>
#include <stdio.h>
typedef enum
{
    IIC_OK    = 0, /* Operation completed successfully.  */
    IIC_ERROR = 1, /* Run-time error without case matched*/
} iic_status_t;

typedef struct
{
    void (*pf_delay_us)(const uint32_t);
} delay_interface_t;

typedef struct
{
    void (*pf_i2c_init)(void);
    //  void (*pf_i2c_deinit)               (void*);
    void    (*pf_i2c_sda_in)(void);
    void    (*pf_i2c_sda_out)(void);
    void    (*pf_set_scl)(uint8_t);
    void    (*pf_set_sda)(uint8_t);
    uint8_t (*pf_get_sda)(void);
    // iic_status_t (*pf_i2c_critical_enter)     (void*);/*EnterTheCriticalZone*/
    // iic_status_t (*pf_i2c_critical_exit)      (void*);/*ExitTheCriticalZone*/
} i2c_driver_interface_t;

typedef struct
{
    i2c_driver_interface_t *pf_i2c_interface_inst;
    delay_interface_t      *pf_delay_interface_inst;

} i2c_driver_t;

iic_status_t iic_driver_inst(i2c_driver_t           *i2c_driver_inst,
                             i2c_driver_interface_t *pf_i2c_interface_inst,
                             delay_interface_t      *pf_delay_interface_inst);

void iic_write_one_byte_CRC1(i2c_driver_t *i2c_driver ,uint16_t WriteAddr,uint16_t DataToWrite);
uint8_t iic_read_one_byte(i2c_driver_t *i2c_driver,uint8_t dev_addr,uint16_t ReadAddr);
uint8_t iic_read_bytes(i2c_driver_t *i2c_driver, uint8_t dev_addr, uint16_t ReadAddr, uint8_t *buf, uint8_t len);
#endif // BSP_IIC_H
