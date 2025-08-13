
#include "bsp_iic.h"
/**
 * @brief  IIC start signal
 * @param  i2c_driver_inst IIC driver instance
 * @return - none
 * @note   - none
 */
void IIC_Start(i2c_driver_t *i2c_driver)
{
    i2c_driver->pf_i2c_interface_inst->pf_i2c_sda_out();
    i2c_driver->pf_i2c_interface_inst->pf_set_sda(1);
    i2c_driver->pf_i2c_interface_inst->pf_set_scl(1);
    i2c_driver->pf_delay_interface_inst->pf_delay_us(100);
    i2c_driver->pf_i2c_interface_inst->pf_set_sda(0);
    i2c_driver->pf_delay_interface_inst->pf_delay_us(100);
    i2c_driver->pf_i2c_interface_inst->pf_set_scl(0);
}
/**
 * @brief  IIC stop signal
 * @param  i2c_driver_inst IIC driver instance
 * @return - none
 * @note   - none
 */
void IIC_Stop(i2c_driver_t *i2c_driver)
{
    i2c_driver->pf_i2c_interface_inst->pf_i2c_sda_out();
    i2c_driver->pf_i2c_interface_inst->pf_set_sda(0);
    i2c_driver->pf_i2c_interface_inst->pf_set_scl(1);
    i2c_driver->pf_delay_interface_inst->pf_delay_us(100);
    i2c_driver->pf_i2c_interface_inst->pf_set_sda(1);
    i2c_driver->pf_delay_interface_inst->pf_delay_us(100);
    i2c_driver->pf_i2c_interface_inst->pf_set_scl(0);
}

/**
 * @brief  IIC wait for ack signal
 * @param  i2c_driver_inst IIC driver instance
 * @return - none
 * @note   - none
 */
uint8_t IIC_Wait_Ack(i2c_driver_t *i2c_driver)
{

    uint16_t ucErrTime = 0;
    i2c_driver->pf_i2c_interface_inst->pf_i2c_sda_in();
    i2c_driver->pf_i2c_interface_inst->pf_set_sda(1);
    i2c_driver->pf_delay_interface_inst->pf_delay_us(100);
    i2c_driver->pf_i2c_interface_inst->pf_set_scl(1);
    i2c_driver->pf_delay_interface_inst->pf_delay_us(100);

    while(i2c_driver->pf_i2c_interface_inst->pf_get_sda())
    {
        ucErrTime++;

        if(ucErrTime > 2000)
        {
            IIC_Stop(i2c_driver);
            return 1;
        }
    }
    i2c_driver->pf_i2c_interface_inst->pf_set_scl(0);
    return 0;
}
/**
 * @brief  IIC ack signal
 * @param  i2c_driver_inst IIC driver instance
 * @return - none
 * @note   - none
 */
void IIC_Ack(i2c_driver_t *i2c_driver)
{
    i2c_driver->pf_i2c_interface_inst->pf_set_scl(0);
    i2c_driver->pf_i2c_interface_inst->pf_i2c_sda_out();
    i2c_driver->pf_i2c_interface_inst->pf_set_sda(0);
    i2c_driver->pf_delay_interface_inst->pf_delay_us(100);
    i2c_driver->pf_i2c_interface_inst->pf_set_scl(1);
    i2c_driver->pf_delay_interface_inst->pf_delay_us(100);
    i2c_driver->pf_i2c_interface_inst->pf_set_scl(0);
}
/**
 * @brief  IIC nack signal
 * @param  i2c_driver_inst IIC driver instance
 * @return - none
 * @note   - none
 */
void IIC_NAck(i2c_driver_t *i2c_driver)
{

    i2c_driver->pf_i2c_interface_inst->pf_set_scl(0);
    i2c_driver->pf_i2c_interface_inst->pf_i2c_sda_out();
    i2c_driver->pf_i2c_interface_inst->pf_set_sda(1);
    i2c_driver->pf_delay_interface_inst->pf_delay_us(100);
    i2c_driver->pf_i2c_interface_inst->pf_set_scl(1);
    i2c_driver->pf_delay_interface_inst->pf_delay_us(100);
    i2c_driver->pf_i2c_interface_inst->pf_set_scl(0);
}
/**
 * @brief  IIC send byte
 * @param  i2c_driver_inst IIC driver instance  txd - data to be sent
 * @return - none
 * @note   - none
 */
void IIC_Send_Byte(i2c_driver_t *i2c_driver, uint8_t txd)
{

    uint8_t t;
    i2c_driver->pf_i2c_interface_inst->pf_i2c_sda_out();
    i2c_driver->pf_i2c_interface_inst->pf_set_scl(0);
    for(t = 0; t < 8; t++)
    {
        i2c_driver->pf_i2c_interface_inst->pf_set_sda((txd & 0x80) >> 7);
        txd <<= 1;
        i2c_driver->pf_i2c_interface_inst->pf_set_scl(1);
        i2c_driver->pf_delay_interface_inst->pf_delay_us(100);
        i2c_driver->pf_i2c_interface_inst->pf_set_scl(0);
        i2c_driver->pf_delay_interface_inst->pf_delay_us(100);
    }
}
/**
 * @brief  IIC read byte
 * @param  i2c_driver_inst IIC driver instance  ack - ack signal
 * @return - none
 * @note   - none
 */
uint8_t IIC_Read_Byte(i2c_driver_t *i2c_driver, unsigned char ack)
{

    unsigned char i, receive = 0;
    i2c_driver->pf_i2c_interface_inst->pf_i2c_sda_in();
    for(i = 0; i < 8; i++)
    {
        i2c_driver->pf_i2c_interface_inst->pf_set_scl(0);
        i2c_driver->pf_delay_interface_inst->pf_delay_us(100);
        i2c_driver->pf_i2c_interface_inst->pf_set_scl(1);
        receive <<= 1;
        if(i2c_driver->pf_i2c_interface_inst->pf_get_sda())
            receive++;
        i2c_driver->pf_delay_interface_inst->pf_delay_us(100);
    }

    if(!ack)
        IIC_NAck(i2c_driver);
    else
        IIC_Ack(i2c_driver);
    return receive;
}
/**
 * @brief  IIC write data
 * @param  i2c_driver_inst  -IIC driver instance
 *         addr             - device address
 *         cmd              - command
 *         data             - data to be sent
 *         len              - data length
 * @return -
 * @note   - no
 */
uint8_t IIC_Write_Data(i2c_driver_t *i2c_driver,
                       uint8_t       addr,
                       uint8_t       cmd,
                       uint8_t      *data,
                       uint8_t       len)
{

    IIC_Start(i2c_driver);
    IIC_Send_Byte(i2c_driver, (addr << 1) | 0);

    if(IIC_Wait_Ack(i2c_driver))
    {
        IIC_Stop(i2c_driver);
        return 1;
    }

    IIC_Send_Byte(i2c_driver, cmd);
    IIC_Wait_Ack(i2c_driver);
    for(uint8_t i = 0; i < len; i++)
    {
        IIC_Send_Byte(i2c_driver, data[i]);
        IIC_Wait_Ack(i2c_driver);
    }

    IIC_Stop(i2c_driver);
    return 0;
}

uint8_t IIC_Read_Data(i2c_driver_t *i2c_driver,
                      uint8_t       addr,
                      uint8_t      *data,
                      uint8_t       len)
{

    IIC_Start(i2c_driver);
    IIC_Send_Byte(i2c_driver, (addr << 1) | 0x01);
    if(IIC_Wait_Ack(i2c_driver))
    {
        IIC_Stop(i2c_driver);
        return 1;
    }
    for(uint8_t i = 0; i < len; i++)
    {
        if(i == (len - 1))
            data[i] = IIC_Read_Byte(i2c_driver, 0);

        else
            data[i] = IIC_Read_Byte(i2c_driver, 1);
    }

    IIC_Stop(i2c_driver);
    return 0;
}
uint8_t iic_read_one_byte(i2c_driver_t *i2c_driver,uint8_t dev_addr,uint16_t ReadAddr)
{
    uint8_t temp=0;
    IIC_Start(i2c_driver);
    IIC_Send_Byte(i2c_driver, dev_addr << 1);
    IIC_Wait_Ack(i2c_driver);
    IIC_Send_Byte(i2c_driver,ReadAddr);   //发送低地址
    IIC_Wait_Ack(i2c_driver);
    IIC_Start(i2c_driver);
    IIC_Send_Byte(i2c_driver, (dev_addr << 1) | 0x01);
    IIC_Wait_Ack(i2c_driver);;
    temp=IIC_Read_Byte(i2c_driver,1);
    IIC_Stop(i2c_driver);
    return temp;
}
iic_status_t iic_driver_init(i2c_driver_t *i2c_driver_inst)
{
    i2c_driver_inst->pf_i2c_interface_inst->pf_i2c_init();
    return IIC_OK;
}

uint8_t iic_read_bytes(i2c_driver_t *i2c_driver, uint8_t dev_addr, uint16_t ReadAddr, uint8_t *buf, uint8_t len)
{
    if (len == 0) return 1; // 长度为 0 无意义

    IIC_Start(i2c_driver);
    IIC_Send_Byte(i2c_driver, (dev_addr << 1) | 0x00); // 写模式
    if (IIC_Wait_Ack(i2c_driver)) { IIC_Stop(i2c_driver); return 1; }

    IIC_Send_Byte(i2c_driver, ReadAddr); // 寄存器地址
    if (IIC_Wait_Ack(i2c_driver)) { IIC_Stop(i2c_driver); return 1; }

    IIC_Start(i2c_driver);
    IIC_Send_Byte(i2c_driver, (dev_addr << 1) | 0x01); // 读模式
    if (IIC_Wait_Ack(i2c_driver)) { IIC_Stop(i2c_driver); return 1; }

    for (uint8_t i = 0; i < len; i++)
    {
        if (i != (len - 1))
            buf[i] = IIC_Read_Byte(i2c_driver, 1); // ACK 表示还要继续读
        else
            buf[i] = IIC_Read_Byte(i2c_driver, 0); // NACK 表示最后一个字节
    }

    IIC_Stop(i2c_driver);
    return 0; // 成功
}

/**
 * @brief  向I2C设备写入数据。
 * @param  i2c_driver: I2C驱动实例。
 * @param  dev_addr: I2C设备地址，高7位。
 * @param  WriteAddr: 写入的寄存器地址，高8位或16位。
 * @param  buf: 待写入数据的缓冲区指针。
 * @param  len: 待写入数据的长度。
 * @retval 0: 成功, 1: 失败 (例如, 未收到ACK)。
 */
uint8_t iic_write_bytes(i2c_driver_t *i2c_driver, uint8_t dev_addr, uint8_t WriteAddr, uint8_t *buf, uint8_t len)
{
    // 检查参数有效性
    if (!i2c_driver || !buf || len == 0)
    {
        return 1; // 失败
    }

    IIC_Start(i2c_driver); // 发送起始信号

    // 发送设备地址，并判断是否收到ACK
    // I2C设备地址为7位，需要左移1位，最后一位为0表示写入
    IIC_Send_Byte(i2c_driver, dev_addr<<1);// 0XA0 的写法不严谨，通常使用7位地址左移
    IIC_Wait_Ack(i2c_driver);
    // 发送寄存器地址
    // 这里的实现假设寄存器地址是16位的，因此需要发送两次
    IIC_Send_Byte(i2c_driver, WriteAddr); // 发送高8位地址
    IIC_Wait_Ack(i2c_driver);
    // 循环发送数据缓冲区中的所有字节
    for (uint8_t i = 0; i < len; i++)
    {
        IIC_Send_Byte(i2c_driver, buf[i]);
        if (IIC_Wait_Ack(i2c_driver) == 1)
        {
            IIC_Stop(i2c_driver);
            return 1; // 写入数据失败
        }
    }

    IIC_Stop(i2c_driver); // 发送停止信号
    
    // 写入操作后，一些设备可能需要一段时间来处理数据。
    // 这里加入一个延时是很好的实践。
    i2c_driver->pf_delay_interface_inst->pf_delay_us(1000);

    return 0; // 成功
}

iic_status_t iic_driver_inst(i2c_driver_t           *i2c_driver_inst,
                             i2c_driver_interface_t *pf_i2c_interface_inst,
                             delay_interface_t      *pf_delay_interface_inst)
{
    if(NULL == i2c_driver_inst || NULL == pf_i2c_interface_inst ||
       NULL == pf_delay_interface_inst)
    {
        return IIC_ERROR;
    }
    i2c_driver_inst->pf_i2c_interface_inst   = pf_i2c_interface_inst;
    i2c_driver_inst->pf_delay_interface_inst = pf_delay_interface_inst;
    if(IIC_OK != iic_driver_init(i2c_driver_inst))
    {
        return IIC_ERROR;
    }
    i2c_driver_inst->pf_i2c_interface_inst->pf_set_sda(1);
    i2c_driver_inst->pf_i2c_interface_inst->pf_set_scl(1);
    return IIC_OK;
}