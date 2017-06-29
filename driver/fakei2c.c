#include <fakei2c.h>
#include <system.h>

/* 发送/接收模式 */
#define I2C_DIRECTION_TX    ((uint8)0x00)
#define I2C_DIRECTION_RX    ((uint8)0x01)

/*
 * _I2C_Delay - I2C延时函数
 */
static void _I2C_Delay(uint16 us) {
    sys_delay_us(us);
}
/*
* I2C_Start - 发送起始信号
*/
static void _I2C_Start(i2c_dev *dev) {
    dev->sda_h();
    dev->scl_h();

    _I2C_Delay(dev->delayus);
    dev->sda_l();
    _I2C_Delay(dev->delayus);
    dev->scl_l();
    _I2C_Delay(dev->delayus);
}
/*
* I2C_Stop - 发送终止信号
*/
static void _I2C_Stop(i2c_dev *dev) {
    dev->scl_l();
    _I2C_Delay(dev->delayus);
    dev->sda_l();
    _I2C_Delay(dev->delayus);
    dev->scl_h();
    _I2C_Delay(dev->delayus);
    dev->sda_h();
    _I2C_Delay(dev->delayus);
}
/*
* I2C_SendACK - 发送应答
*
* @ack: 0-ACK, 1-NACK
*/
static void _I2C_SendACK(i2c_dev *dev, uint8 ack) {
    dev->scl_l();
    _I2C_Delay(dev->delayus);
    (0 == ack) ? dev->sda_l() : dev->sda_h();
    _I2C_Delay(dev->delayus);
    dev->scl_h();
    _I2C_Delay(dev->delayus);
    dev->scl_l();
    _I2C_Delay(dev->delayus);
}

#define _ACK_     0
#define _NACK_    1

/*
* I2C_WaitAck - 等待应答
*
* return: 0-ACK,1-NACK
*/
static uint8 _I2C_WaitAck(i2c_dev *dev) {
    uint8 re;
    dev->scl_l();
    _I2C_Delay(dev->delayus);

    dev->set_sda_in();

    dev->scl_h();
    _I2C_Delay(dev->delayus);
    re = (dev->sda()) ? 1 : 0;
    dev->scl_l();

    dev->set_sda_out();
    _I2C_Delay(dev->delayus);
    return re;
}

/*
* I2C_SendByte - 发送一个字节
*
* @data: 待发送数据
*/
static void _I2C_WriteByte(i2c_dev *dev, uint8 data) {
    uint8 i = 8;
    while (i--) {
        dev->scl_l();

        _I2C_Delay(dev->delayus);
        (data & 0x80) ? dev->sda_h() : dev->sda_l();
        data <<= 1;
        _I2C_Delay(dev->delayus);
        dev->scl_h();
        _I2C_Delay(dev->delayus);
    }
    dev->scl_l();
    _I2C_WaitAck(dev);
}

/*
* I2C_ReadByte - 接收一个字节
*
* return: 接收的字节
*/
static uint8 _I2C_ReadByte(i2c_dev *dev) {
    uint8 i = 8;
    uint8 re = 0;

    dev->sda_h();
    dev->set_sda_in();
    while (i--) {
        re <<= 1;
        dev->scl_l();
        _I2C_Delay(dev->delayus);
        dev->scl_h();
        _I2C_Delay(dev->delayus);
        if (dev->sda())
            re |= 0x01;
    }
    dev->scl_l();
    dev->set_sda_out();

    return re;
}

/*
* InitI2C - 初始化I2C
*/
void Init_I2C(i2c_dev *dev) {
    dev->init();
}


/*
* I2C_SendByte - 发送字节@data到设备@dev的寄存器@reg中
*
* @data: 数据
* @dev: 设备地址
* @reg: 寄存器
*/
void I2C_SendByte(i2c_dev *dev, uint8 data, uint8 add, uint8 reg) {
    _I2C_Start(dev);
    _I2C_WriteByte(dev, add | I2C_DIRECTION_TX);
    _I2C_WriteByte(dev, reg);
    _I2C_WriteByte(dev, data);
    _I2C_Stop(dev);
}
/*
* I2C_SendDatas - 发送数据*buf到设备@dev的寄存器@reg中
*
* @buf: 数据缓存
* @dev: 设备地址
* @reg: 寄存器
*/
void I2C_SendDatas(i2c_dev *dev, const uint8 *buf, int len, uint8 add, uint8 reg) {
    _I2C_Start(dev);
    _I2C_WriteByte(dev, add | I2C_DIRECTION_TX);
    _I2C_WriteByte(dev, reg);
    while (len--) {
        _I2C_WriteByte(dev, buf[0]);
        buf++;
    }
    _I2C_Stop(dev);
}
/*
* I2C_ReceiveDatas - 从设备@dev的寄存器@reg中接收长度为@len的数据*buf
*
* @buf: 数据缓存
* @len: 数据长度
* @dev: 设备地址
* @reg: 寄存器
*/
void I2C_ReceiveDatas(i2c_dev *dev, uint8 *buf, int len, uint8 add, uint8 reg) {
    _I2C_Start(dev);
    _I2C_WriteByte(dev, add | I2C_DIRECTION_TX);
    _I2C_WriteByte(dev, reg);

    _I2C_Start(dev);
    _I2C_WriteByte(dev, add | I2C_DIRECTION_RX);
    while (len > 1) {
        buf[0] = _I2C_ReadByte(dev);
        buf++;
        len--;
        _I2C_SendACK(dev, _ACK_);
    }

    buf[0] = _I2C_ReadByte(dev);
    _I2C_SendACK(dev, _NACK_);
    _I2C_Stop(dev);
}




