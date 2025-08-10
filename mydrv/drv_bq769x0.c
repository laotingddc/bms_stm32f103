//
// Created by 69566 on 25-8-5.
//

#include "drv_bq769x0.h"
#include <stdio.h>
/*************************************************************/
i2c_driver_interface_t bq769x0_i2c_ops = {
    .pf_i2c_init    = IIC_Init,
    .pf_i2c_sda_in  = sda_in,
    .pf_i2c_sda_out = sda_out,
    .pf_set_scl     = IIC_SCL,
    .pf_set_sda     = IIC_SDA,
    .pf_get_sda     = READ_SDA,
};
delay_interface_t delay_interface = {
    .pf_delay_us = delay,
};
static i2c_driver_t bq769x0_driver;
/*************************************************************/
void BQ769X0_EntryShip(void)
{
    iic_write_one_byte_CRC1(&bq769x0_driver , SYS_CTRL1,0x00);
    iic_write_one_byte_CRC1(&bq769x0_driver , SYS_CTRL1,0x01);
    iic_write_one_byte_CRC1(&bq769x0_driver , SYS_CTRL1,0x02);
}
/*唤醒afe*/
void BQ769X0_Wakeup(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
    HAL_Delay(500);
}
int ADC_offset,GAIN;
float ADC_GAIN = 0;
void BQ769X0_GetADCGainOffset(void)
{
    unsigned char gain[3] ={0};
    iic_read_bytes(&bq769x0_driver,BQ769X0_I2C_ADDR, ADCGAIN1, &gain[0], 1);
    iic_read_bytes(&bq769x0_driver,BQ769X0_I2C_ADDR, ADCGAIN2, &gain[1], 1);
    ADC_GAIN = ((gain[0]&0x0c)<<1)+((gain[1]&0xe0)>>5);//12uV
    ADC_offset=iic_read_one_byte(&bq769x0_driver,BQ769X0_I2C_ADDR,ADCOFFSET);
    GAIN = 365+ADC_GAIN;//GAIN=377uV
}

 void Get_Battery1(void)
{
    unsigned int readbattbuf[2];
    short batteryval1;
    readbattbuf[1] = iic_read_one_byte(&bq769x0_driver,BQ769X0_I2C_ADDR,0x0c);//ADC_GAIN1
    readbattbuf[0] = iic_read_one_byte(&bq769x0_driver,BQ769X0_I2C_ADDR,0x0d);//ADC_GAIN1

    batteryval1= (readbattbuf[1] << 8) |readbattbuf[0];
    batteryval1=((batteryval1*GAIN)/1000)+ADC_offset;
	printf("batteryval1 =%d\n",batteryval1);
}
 void BQ769X0_Initialize(void)
{
    iic_driver_inst(&bq769x0_driver, &bq769x0_i2c_ops, &delay_interface);
    BQ769X0_Wakeup();
    BQ769X0_EntryShip();
    BQ769X0_GetADCGainOffset();
    Get_Battery1();
}
