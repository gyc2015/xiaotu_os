#include <fakei2c.h>

/* ����/����ģʽ */
#define I2C_DIRECTION_TX    ((uint8)0x00)
#define I2C_DIRECTION_RX    ((uint8)0x01)

/*
 * _I2C_Delay - I2C��ʱ����
 */
static void _I2C_Delay(void) {
    int i;
    for (i = 0; i < 200; i++);
}
/*
* I2C_Start - ������ʼ�ź�
*/
static void _I2C_Start(i2c_dev *dev) {
    dev->sda_h();
    dev->scl_h();

    _I2C_Delay();
    dev->sda_l();
    _I2C_Delay();
    dev->scl_l();
    _I2C_Delay();
}
/*
* I2C_Stop - ������ֹ�ź�
*/
static void _I2C_Stop(i2c_dev *dev) {
    dev->scl_l();
    _I2C_Delay();
    dev->sda_l();
    _I2C_Delay();
    dev->scl_h();
    _I2C_Delay();
    dev->sda_h();
    _I2C_Delay();
}
/*
* I2C_SendACK - ����Ӧ��
*
* @ack: 0-ACK, 1-NACK
*/
static void _I2C_SendACK(i2c_dev *dev, uint8 ack) {
    dev->scl_l();
    _I2C_Delay();
    (0 == ack) ? dev->sda_l() : dev->sda_h();
    _I2C_Delay();
    dev->scl_h();
    _I2C_Delay();
    dev->scl_l();
    _I2C_Delay();
}

#define _ACK_     0
#define _NACK_    1

/*
* I2C_WaitAck - �ȴ�Ӧ��
*
* return: 0-ACK,1-NACK
*/
static uint8 _I2C_WaitAck(i2c_dev *dev) {
    uint8 re;
    dev->scl_l();
    _I2C_Delay();

    dev->set_sda_in();

    dev->scl_h();
    _I2C_Delay();
    re = (dev->sda()) ? 1 : 0;
    dev->scl_l();

    dev->set_sda_out();
    _I2C_Delay();
    return re;
}

/*
* I2C_SendByte - ����һ���ֽ�
*
* @data: ����������
*/
static void _I2C_WriteByte(i2c_dev *dev, uint8 data) {
    uint8 i = 8;
    while (i--) {
        dev->scl_l();

        _I2C_Delay();
        (data & 0x80) ? dev->sda_h() : dev->sda_l();
        data <<= 1;
        _I2C_Delay();
        dev->scl_h();
        _I2C_Delay();
    }
    dev->scl_l();
    _I2C_WaitAck(dev);
}

/*
* I2C_ReadByte - ����һ���ֽ�
*
* return: ���յ��ֽ�
*/
static uint8 _I2C_ReadByte(i2c_dev *dev) {
    uint8 i = 8;
    uint8 re = 0;

    dev->sda_h();
    dev->set_sda_in();
    while (i--) {
        re <<= 1;
        dev->scl_l();
        _I2C_Delay();
        dev->scl_h();
        _I2C_Delay();
        if (dev->sda())
            re |= 0x01;
    }
    dev->scl_l();
    dev->set_sda_out();

    return re;
}

/*
* InitI2C - ��ʼ��I2C
*/
void Init_I2C(i2c_dev *dev) {
    dev->init();
}


/*
* I2C_SendByte - �����ֽ�@data���豸@dev�ļĴ���@reg��
*
* @data: ����
* @dev: �豸��ַ
* @reg: �Ĵ���
*/
void I2C_SendByte(i2c_dev *dev, uint8 data, uint8 add, uint8 reg) {
    _I2C_Start(dev);
    _I2C_WriteByte(dev, add | I2C_DIRECTION_TX);
    _I2C_WriteByte(dev, reg);
    _I2C_WriteByte(dev, data);
    _I2C_Stop(dev);
}
/*
* I2C_SendDatas - ��������*buf���豸@dev�ļĴ���@reg��
*
* @buf: ���ݻ���
* @dev: �豸��ַ
* @reg: �Ĵ���
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
* I2C_ReceiveDatas - ���豸@dev�ļĴ���@reg�н��ճ���Ϊ@len������*buf
*
* @buf: ���ݻ���
* @len: ���ݳ���
* @dev: �豸��ַ
* @reg: �Ĵ���
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




