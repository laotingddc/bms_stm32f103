//
// Created by 69566 on 25-8-5.
//

#ifndef BQ769X0_REG_H
#define BQ769X0_REG_H

#define BQ769X0_I2C_ADDR 0x08  /*bq769x0 iic地址*/

#define OV_THRESH_BASE			0x2008
#define UV_THRESH_BASE			0x1000
#define OV_STEP					0x10
#define UV_STEP					0x10


#define ADCGAIN_BASE			365
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
#define SCD_THRESH_44mV_22mV	0x00
#define SCD_THRESH_67mV_33mV	0x01
#define SCD_THRESH_89mV_44mV	0x02
#define SCD_THRESH_111mV_56mV	0x03
#define SCD_THRESH_133mV_67mV	0x04
#define SCD_TRHESH_155mV_68mV	0x05
#define SCD_THRESH_178mV_89mV	0x06
#define SCD_THRESH_200mV_100mV	0x07

#define OCD_THRESH_17mV_8mV		0x00
#define OCD_THRESH_22mV_11mV	0x01
#define OCD_THRESH_28mV_14mV	0x02
#define OCD_THRESH_33mV_17mV	0x03
#define OCD_THRESH_39mV_19mV	0x04
#define OCD_THRESH_44mV_22mV	0x05
#define OCD_THRESH_50mV_25mV	0x06
#define OCD_THRESH_56mV_28MV	0x07
#define OCD_THRESH_61mV_31mV	0x08
#define OCD_THRESH_67mV_33mV	0x09
#define OCD_THRESH_72mV_36mV	0x0A
#define OCD_THRESH_78mV_39mV	0x0B
#define OCD_THRESH_83mV_42mV	0x0C
#define OCD_THRESH_89mV_44mV	0x0D
#define OCD_THRESH_94mV_47mV	0x0E
#define OCD_THRESH_100mV_50mV	0x0F

typedef struct _Register_Group
{
	union
	{
		struct
		{
			uint8_t OCD				:1;
			uint8_t SCD				:1;
			uint8_t OV				:1;
			uint8_t UV				:1;
			uint8_t OVRD_ALERT		:1;
			uint8_t DEVICE_XREADY	:1;
			uint8_t WAKE			:1;
			uint8_t CC_READY		:1;
		}StatusBit;
		uint8_t StatusByte;
	}SysStatus;

	union
	{
		struct
		{
			uint8_t RSVD		:3;
			uint8_t CB5			:1;
			uint8_t CB4			:1;
			uint8_t CB3			:1;
			uint8_t CB2			:1;
			uint8_t CB1			:1;
		}CellBal1Bit;
		uint8_t CellBal1Byte;
	}CellBal1;

	union
	{
		struct
		{
			uint8_t RSVD		:3;
			uint8_t CB10		:1;
			uint8_t CB9			:1;
			uint8_t CB8			:1;
			uint8_t CB7			:1;
			uint8_t CB6			:1;
		}CellBal2Bit;
		uint8_t CellBal2Byte;
	}CellBal2;

	union
	{
		struct
		{
			uint8_t RSVD			:3;
			uint8_t CB15			:1;
			uint8_t CB14			:1;
			uint8_t CB13			:1;
			uint8_t CB12			:1;
			uint8_t CB11			:1;
		}CellBal3Bit;
		uint8_t CellBal3Byte;
	}CellBal3;

	union
	{
		struct
		{
			uint8_t SHUT_B			:1;
			uint8_t SHUT_A			:1;
			uint8_t RSVD1			:1;
			uint8_t TEMP_SEL		:1;
			uint8_t ADC_EN			:1;
			uint8_t RSVD2			:2;
			uint8_t LOAD_PRESENT	:1;
		}SysCtrl1Bit;
		uint8_t SysCtrl1Byte;
	}SysCtrl1;

	union
	{
		struct
		{
			uint8_t CHG_ON			:1;
			uint8_t DSG_ON			:1;
			uint8_t WAKE_T			:2;
			uint8_t WAKE_EN			:1;
			uint8_t CC_ONESHOT		:1;
			uint8_t CC_EN			:1;
			uint8_t DELAY_DIS		:1;
		}SysCtrl2Bit;
		uint8_t SysCtrl2Byte;
	}SysCtrl2;

	union
	{
		struct
		{
			uint8_t SCD_THRESH		:3;
			uint8_t SCD_DELAY		:2;
			uint8_t RSVD			:2;
			uint8_t RSNS			:1;
		}Protect1Bit;
		uint8_t Protect1Byte;
	}Protect1;

	union
	{
		struct
		{
			uint8_t OCD_THRESH		:4;
			uint8_t OCD_DELAY		:3;
			uint8_t RSVD			:1;
		}Protect2Bit;
		uint8_t Protect2Byte;
	}Protect2;

	union
	{
		struct
		{
			uint8_t RSVD			:4;
			uint8_t OV_DELAY		:2;
			uint8_t UV_DELAY		:2;
		}Protect3Bit;
		uint8_t Protect3Byte;
	}Protect3;

	uint8_t OVTrip;
	uint8_t UVTrip;
	uint8_t CCCfg;			//must be 0x19

	union
	{
		struct
		{
			uint8_t VC1_HI;
			uint8_t VC1_LO;
		}VCell1Byte;
		uint16_t VCell1Word;
	}VCell1;

	union
	{
		struct
		{
			uint8_t VC2_HI;
			uint8_t VC2_LO;
		}VCell2Byte;
		uint16_t VCell2Word;
	}VCell2;

	union
	{
		struct
		{
			uint8_t VC3_HI;
			uint8_t VC3_LO;
		}VCell3Byte;
		uint16_t VCell3Word;
	}VCell3;

	union
	{
		struct
		{
			uint8_t VC4_HI;
			uint8_t VC4_LO;
		}VCell4Byte;
		uint16_t VCell4Word;
	}VCell4;

	union
	{
		struct
		{
			uint8_t VC5_HI;
			uint8_t VC5_LO;
		}VCell5Byte;
		uint16_t VCell5Word;
	}VCell5;

	union
	{
		struct
		{
			uint8_t VC6_HI;
			uint8_t VC6_LO;
		}VCell6Byte;
		uint16_t VCell6Word;
	}VCell6;

	union
	{
		struct
		{
			uint8_t VC7_HI;
			uint8_t VC7_LO;
		}VCell7Byte;
		uint16_t VCell7Word;
	}VCell7;

	union
	{
		struct
		{
			uint8_t VC8_HI;
			uint8_t VC8_LO;
		}VCell8Byte;
		uint16_t VCell8Word;
	}VCell8;

	union
	{
		struct
		{
			uint8_t VC9_HI;
			uint8_t VC9_LO;
		}VCell9Byte;
		uint16_t VCell9Word;
	}VCell9;

	union
	{
		struct
		{
			uint8_t VC10_HI;
			uint8_t VC10_LO;
		}VCell10Byte;
		uint16_t VCell10Word;
	}VCell10;

	union
	{
		struct
		{
			uint8_t VC11_HI;
			uint8_t VC11_LO;
		}VCell11Byte;
		uint16_t VCell11Word;
	}VCell11;

	union
	{
		struct
		{
			uint8_t VC12_HI;
			uint8_t VC12_LO;
		}VCell12Byte;
		uint16_t VCell12Word;
	}VCell12;

	union
	{
		struct
		{
			uint8_t VC13_HI;
			uint8_t VC13_LO;
		}VCell13Byte;
		uint16_t VCell13Word;
	}VCell13;

	union
	{
		struct
		{
			uint8_t VC14_HI;
			uint8_t VC14_LO;
		}VCell14Byte;
		uint16_t VCell14Word;
	}VCell14;

	union
	{
		struct
		{
			uint8_t VC15_HI;
			uint8_t VC15_LO;
		}VCell15Byte;
		uint16_t VCell15Word;
	}VCell15;

	union
	{
		struct
		{
			uint8_t BAT_HI;
			uint8_t BAT_LO;
		}VBatByte;
		uint16_t VBatWord;
	}VBat;

	union
	{
		struct
		{
			uint8_t TS1_HI;
			uint8_t TS1_LO;
		}TS1Byte;
		uint16_t TS1Word;
	}TS1;

	union
	{
		struct
		{
			uint8_t TS2_HI;
			uint8_t TS2_LO;
		}TS2Byte;
		uint16_t TS2Word;
	}TS2;

	union
	{
		struct
		{
			uint8_t TS3_HI;
			uint8_t TS3_LO;
		}TS3Byte;
		uint16_t TS3Word;
	}TS3;

	union
	{
		struct
		{
			uint8_t CC_HI;
			uint8_t CC_LO;
		}CCByte;
		uint16_t CCWord;
	}CC;

	union
	{
		struct
		{
			uint8_t RSVD1			:2;
			uint8_t ADCGAIN_4_3		:2;
			uint8_t RSVD2			:4;
		}ADCGain1Bit;
		uint8_t ADCGain1Byte;
	}ADCGain1;

	uint8_t ADCOffset;

	union
	{
		struct
		{
			uint8_t RSVD			:5;
			uint8_t ADCGAIN_2_0		:3;
		}ADCGain2Bit;
		uint8_t ADCGain2Byte;
	}ADCGain2;

}RegisterGroup;
