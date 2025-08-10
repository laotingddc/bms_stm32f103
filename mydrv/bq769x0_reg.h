//
// Created by 69566 on 25-8-5.
//

#ifndef BQ769X0_REG_H
#define BQ769X0_REG_H

#define BQ769X0_I2C_ADDR 0x08  /*bq769x0 iic地址*/

#define ADCGAIN1 				0x50
#define ADCOFFSET 				0x51
#define ADCGAIN2 				0x59
#define SYS_CTRL1 				0x04
#define SYS_STAT 0x00
#define CELLBAL1 0x01
#define CELLBAL2 0x02
#define SYS_CTRL1 0x04
#define SYS_CTRL2 0x05
#define PROTECT1 0x06
#define PROTECT2 0x07
#define PROTECT3 0x08
#define OV_TRIP 0x09
#define UV_TRIP 0x0A
#define CC_CFG  0x0B
#endif //BQ769X0_REG_H
