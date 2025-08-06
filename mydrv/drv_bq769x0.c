//
// Created by 69566 on 25-8-5.
//

#include "drv_bq769x0.h"

#include <stdio.h>


#include "soft_iic.h"
static uint8_t CRC8(uint8_t *ptr, uint8_t len)
{
    uint8_t i, crc=0;

    while (len-- != 0)
    {
        for (i = 0x80; i != 0; i /= 2)
        {
            if ((crc & 0x80) != 0)
            {
                crc *= 2;
                crc ^= 0x07;
            }
            else
            {
                crc *= 2;
            }
            if ((*ptr & i) != 0)
            {
                crc ^= 0x07;
            }
        }
        ptr++;
    }
    return(crc);
}
static   uint8_t BQ769X0_WriteRegisterByteWithCRC(uint8_t Register, uint8_t data)
{
   //  uint8_t dataBuffer[4];
   //
   //  dataBuffer[0] = BQ769X0_I2C_ADDR << 1;
   //  dataBuffer[1] = Register;
   //  dataBuffer[2] = data;
   //  dataBuffer[3] = CRC8(dataBuffer, 3);
   //  HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1,BQ769X0_I2C_ADDR << 1, &dataBuffer[1],3, 1000);
   // return (uint8_t)status;

}



#define I2C_TIMEOUT_MS       100      // 超时时间（ms）

/**
 * @brief 通过I2C读取BQ769x0寄存器（带CRC校验）
 * @param hi2c     I2C句柄（如&hi2c1）
 * @param Register 寄存器地址
 * @param data     读取的数据存储指针
 * @retval bool    true=成功, false=失败
 */
uint8_t BQ769X0_ReadRegisterByteWithCRC(uint8_t Register, uint8_t *data) {
    // uint8_t rxBuffer[2];  // 接收缓冲区：[数据 | CRC]
    // uint8_t crcInput[2];  // CRC计算输入
    // uint8_t crcValue;     // 计算得到的CRC
    //
    // // 步骤1：发送寄存器地址（写模式）
    // if (HAL_I2C_Master_Transmit(
    //         &hi2c1,
    //         BQ769X0_I2C_ADDR << 1,  // HAL库要求7位地址左移1位
    //         &Register,
    //         1,                      // 发送1字节（寄存器地址）
    //         I2C_TIMEOUT_MS) != HAL_OK) {
    //
    //     return 0;
    //         }
    //
    // // 步骤2：读取数据+CRC（读模式）
    // if (HAL_I2C_Master_Receive(
    //         &hi2c1,
    //         BQ769X0_I2C_ADDR << 1,
    //         rxBuffer,
    //         2,                      // 读取2字节（数据+CRC）
    //         I2C_TIMEOUT_MS) != HAL_OK) {
    //
    //     return 0;
    //         }
    //
    // // 步骤3：验证CRC
    // crcInput[0] = (BQ769X0_I2C_ADDR << 1) | 0x01;  // 设备地址（读方向）
    // crcInput[1] = rxBuffer[0];                     // 接收到的数据字节
    // crcValue = CRC8(crcInput, 2);         // 计算CRC
    //
    // if (crcValue != rxBuffer[1]) {
    //
    //     return 0;
    // }
    //
    // // 返回有效数据
    // *data = rxBuffer[0];
    // return 1;
}
/*进入睡眠模式*/
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
unsigned char BQ769_INITAdd[11]={SYS_STAT,CELLBAL1,CELLBAL2,SYS_CTRL1,SYS_CTRL2,PROTECT1,PROTECT2,PROTECT3,OV_TRIP,UV_TRIP,CC_CFG};
unsigned char BQ769_INITdata[11]={0xFF,     0x00,    0x00,    0x18,    0X43,      0x00,     0x00,    0x00,    0x00,   0x00,  0x19};
void BQ769X0_EntryShip(void)
{
    char i;
    for(i=0;i<11;i++)
    {
        HAL_Delay(50);
        IIC1_write_one_byte_CRC(BQ769_INITAdd[i],BQ769_INITdata[i]);
    }
    // IIC1_write_one_byte_CRC(SYS_CTRL1, 0x00);
    // IIC1_write_one_byte_CRC(SYS_CTRL1, 0x01);
    // IIC1_write_one_byte_CRC(SYS_CTRL1, 0x02);
    // BQ769X0_WriteRegisterByteWithCRC(SYS_CTRL1, 0x00);
    // BQ769X0_WriteRegisterByteWithCRC(SYS_CTRL1, 0x01);
    // BQ769X0_WriteRegisterByteWithCRC(SYS_CTRL1, 0x02);
}
/*唤醒afe*/
void BQ769X0_Wakeup(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
    HAL_Delay(500);
}
    //BQ769X0_TS1_set_out_mode();
    //gpio_ctrl(BQ769X0_TS1_Port, BQ769X0_TS1_Pin, SET);

    //BQ769X0_DELAY(1000);
    //gpio_ctrl(BQ769X0_TS1_Port, BQ769X0_TS1_Pin, RESET);
    //BQ769X0_TS1_set_in_mode();  // 设为输入模式，避免干扰温度采样
    //BQ769X0_DELAY(1000);
uint8_t a,b,c;
void BQ769X0_GetADCGainOffset(void)
{
    unsigned char gain[2] ={0};

    gain[0]=IIC1_read_one_byte(ADCGAIN1);//ADC_GAIN1
    gain[1]=IIC1_read_one_byte(ADCGAIN2);//ADC_GAIN2
    // BQ769X0_ReadRegisterByteWithCRC(ADCGAIN1, &(a));
    // BQ769X0_ReadRegisterByteWithCRC(ADCGAIN2, &(b));
    // BQ769X0_ReadRegisterByteWithCRC(ADCOFFSET, &(c));
    printf("gain[0]=%d,gain[1]=%d\n",gain[0],gain[1]);
}
 void BQ769X0_Initialize(void)
{
    BQ769X0_Wakeup();
    BQ769X0_EntryShip();
    BQ769X0_GetADCGainOffset();

}