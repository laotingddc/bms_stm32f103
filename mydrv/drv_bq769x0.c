//
// Created by 69566 on 25-8-5.
//

#include "drv_bq769x0.h"
#include <stdio.h>
#include "iic_port.h"
#include "crc.h"
#include <string.h>  // 提供memcpy函数
static iic_device_t* bq_device; //iic设备实例
static RegisterGroup Registers = {0};// bq769x0寄存器组

/* ADC增益 */
static float Gain = 0;		
static int16_t iGain = 0;
static int8_t Adcoffset;
void iic_write_one_byte_CRC1(uint16_t WriteAddr,uint8_t DataToWrite)
{
    unsigned char DataBuffer[4];
    DataBuffer[0] = BQ769X0_I2C_ADDR << 1;
    DataBuffer[1] = WriteAddr;
    DataBuffer[2] = DataToWrite;
    DataBuffer[3] = CRC8(DataBuffer, 3);
    bq_device->write(bq_device->driver,BQ769X0_I2C_ADDR, DataBuffer[1],&DataBuffer[2],2);
}
/**
 * @brief 通过I2C写入多字节数据并附加CRC校验
 * @param DevAddr  I2C设备地址(7位格式)
 * @param WriteAddr  要写入的目标地址
 * @param pData  要写入的数据指针
 * @param DataLen  要写入的数据长度(字节数)
 * @return 成功返回0，失败返回错误码
 */
int iic_write_multi_bytes_CRC( uint16_t WriteAddr, uint8_t *pData, uint16_t DataLen)
{
	// 计算总缓冲区大小: DevAddr(1) + WriteAddr(1或2) + Data + CRC(1)
	uint16_t total_len = 1 + (WriteAddr > 0xFF ? 2 : 1) + DataLen + 1;
	uint8_t *DataBuffer = (uint8_t *)malloc(total_len);
	if(DataBuffer == NULL) {
		return -1; // 内存分配失败
	}

	uint8_t *p = DataBuffer;

	// 填充设备地址(左移1位，写操作位为0)
	*p++ = (BQ769X0_I2C_ADDR << 1);

	// 填充写入地址(支持16位地址)
	if(WriteAddr > 0xFF) {
		*p++ = (uint8_t)(WriteAddr >> 8);   // 高地址字节
	}
	*p++ = (uint8_t)(WriteAddr);           // 低地址字节

	// 填充要写入的数据
	memcpy(p, pData, DataLen);
	p += DataLen;

	// 计算CRC(包括设备地址、写入地址和写入数据)
	uint8_t crc = CRC8(DataBuffer, p - DataBuffer);
	*p = crc;

	// 实际I2C写入(包含CRC)
	int ret = bq_device->write(bq_device->driver, BQ769X0_I2C_ADDR,
							  WriteAddr, DataBuffer + 1, total_len - 1);

	free(DataBuffer);
	return ret;
}
uint8_t iic_read_multi_byte(uint16_t ReadAddr,uint8_t *pBuffer,uint8_t NumToRead)
{
    if(NULL == bq_device)
    {
        printf("bq_device is null\n");
    }
   return bq_device->read(bq_device->driver,BQ769X0_I2C_ADDR, ReadAddr,pBuffer,NumToRead);

}
/*************************************************************/
void BQ769X0_EntryShip(void)
{
     iic_write_one_byte_CRC1( SYS_CTRL1,0x00);
     iic_write_one_byte_CRC1( SYS_CTRL1,0x01);
     iic_write_one_byte_CRC1( SYS_CTRL1,0x02);
     HAL_Delay(100);
}
/*唤醒afe*/
void BQ769X0_Wakeup(void)
{
    //TODO 函数解耦
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
    HAL_Delay(1500);
}

void BQ769X0_GetADCGainOffset(void)
{

    iic_read_multi_byte( ADCGAIN1, &(Registers.ADCGain1.ADCGain1Byte), 1);
	printf("Registers.ADCGain1.ADCGain1Byte =%d\n",Registers.ADCGain1.ADCGain1Byte);
    iic_read_multi_byte( ADCGAIN2, &(Registers.ADCGain2.ADCGain2Byte), 1);
    iic_read_multi_byte( ADCOFFSET,&(Registers.ADCOffset),1);
    /*GAIN is uV/LSB,OFFSET is mV*/
	Gain = (ADCGAIN_BASE + ((Registers.ADCGain1.ADCGain1Byte & 0x0C) << 1) + ((Registers.ADCGain2.ADCGain2Byte & 0xE0)>> 5)) / 1000.0;
	iGain = ADCGAIN_BASE + ((Registers.ADCGain1.ADCGain1Byte & 0x0C) << 1) + ((Registers.ADCGain2.ADCGain2Byte & 0xE0)>> 5);
	
    if (Registers.ADCOffset & 0x80)
	{
		Adcoffset = 256 - (int16_t)Registers.ADCOffset * -1;
	}
	else
	{
		Adcoffset = Registers.ADCOffset;
	}

}


 void BQ769X0_Initialize(void)
{
    bq_device = iic_device_find("bq769x0");

    BQ769X0_EntryShip();
    HAL_Delay(500); //TODO 延时函数解耦
    BQ769X0_Wakeup();
    BQ769X0_GetADCGainOffset();
    Registers.Protect1.Protect1Bit.SCD_THRESH = SCD_THRESH_89mV_44mV;// 2
	Registers.Protect1.Protect1Bit.SCD_DELAY  = INIT_SCD_DELAY;// 1
	Registers.Protect2.Protect2Bit.OCD_THRESH = OCD_THRESH_22mV_11mV;	//1
	Registers.Protect2.Protect2Bit.OCD_DELAY  = INIT_OCD_DELAY;	//5
	Registers.Protect3.Protect3Bit.UV_DELAY   = INIT_UV_DELAY;	//1
	Registers.Protect3.Protect3Bit.OV_DELAY   = INIT_OV_DELAY;//1
    Registers.OVTrip = (uint8_t)((((uint16_t)((INIT_OV_PROTECT * 1000 - Adcoffset)/Gain/* + 0.5*/) - OV_THRESH_BASE) >> 4) & 0xFF);
    Registers.UVTrip = (uint8_t)((((uint16_t)(( INIT_UV_PROTECT * 1000 - Adcoffset)/Gain/* + 0.5*/) - UV_THRESH_BASE) >> 4) & 0xFF);
	unsigned char ReadBuffer[8];

	// 开ADC,选择外部NTC
	Registers.SysCtrl1.SysCtrl1Byte = 0x18;

	// 使能电流连续采样，关闭充放电MOS
	Registers.SysCtrl2.SysCtrl2Byte = 0x40;

	// 配置CC_CFG,说明书要求在初始化时应配置为0X19以获得更好的性能
	Registers.CCCfg = 0x19;

	// 写入配置到寄存器
	 iic_write_multi_bytes_CRC(SYS_CTRL1, &(Registers.Protect1.Protect1Byte), 5);
	//iic_write_one_byte_CRC1(SYS_CTRL1,Registers.Protect1.Protect1Byte);
	iic_read_multi_byte(SYS_CTRL1, &ReadBuffer, 5);
	printf("hello world\n");
	for (int i =0; i < 8; i++)
	{
		printf("ReadBuffer%d\n", ReadBuffer[i]);
	}
	if( (ReadBuffer[0]&0X7F) != Registers.SysCtrl1.SysCtrl1Byte
|| ReadBuffer[1] != Registers.SysCtrl2.SysCtrl2Byte
|| ReadBuffer[2] != Registers.Protect1.Protect1Byte
|| ReadBuffer[3] != Registers.Protect2.Protect2Byte
|| ReadBuffer[4] != Registers.Protect3.Protect3Byte
|| ReadBuffer[5] != Registers.OVTrip
|| ReadBuffer[6] != Registers.UVTrip
|| ReadBuffer[7] != Registers.CCCfg)
	{
		printf("ReadBuffer[0]) =%d,Registers.SysCtrl1.SysCtrl1Byte=%d\n",(ReadBuffer[0]),Registers.SysCtrl1.SysCtrl1Byte);
		printf("ReadBuffer[1] =%d ,Registers.SysCtrl2.SysCtrl2Byte=%d\n",ReadBuffer[1],Registers.SysCtrl2.SysCtrl2Byte);
		printf("ReadBuffer[2] =%d ,Registers.Protect1.Protect1Byte=%d\n",ReadBuffer[2],Registers.Protect1.Protect1Byte);
		printf("ReadBuffer[3] =%d ,Registers.Protect2.Protect2Byte=%d\n",ReadBuffer[3],Registers.Protect2.Protect2Byte);
		printf("ReadBuffer[4] =%d ,Registers.Protect3.Protect3Byte=%d\n",ReadBuffer[4],Registers.Protect3.Protect3Byte);
		printf("ReadBuffer[5] =%d ,Registers.OVTrip=%d\n",ReadBuffer[5],Registers.OVTrip);
		printf("ReadBuffer[6] =%d ,Registers.UVTrip=%d\n",ReadBuffer[6],Registers.UVTrip);
		printf("ReadBuffer[7] =%d ,Registers.CCCfg=%d\n",ReadBuffer[7],Registers.CCCfg);

		while(1);
	}
}
