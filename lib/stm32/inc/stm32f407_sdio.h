#ifndef STM32F407_SDIO_H
#define STM32F407_SDIO_H

#include <types.h>

#define SDIO_Power_Off 0x00
#define SDIO_Power_Up 0x02
#define SDIO_Power_On 0x03

/*
 * �������ƼĴ��� SDIO_POWER
 * ƫ�Ƶ�ַ: 0x00
 * ��λֵ: 0x0000 0000
 * ����: word����
 */
struct sdio_power_bits {
    uint32 PWRCTRL : 2;
    uint32 rsv : 30;
};
union sdio_power {
    struct sdio_power_bits bits;
    uint32 all;
};

#define SDIO_BusWid_1 (0x00)
#define SDIO_BusWid_4 (0x01)
#define SDIO_BusWid_8 (0x02)
/*
 * ʱ�ӿ��ƼĴ��� SDIO_CLKCR
 * ƫ�Ƶ�ַ: 0x04
 * ��λֵ: 0x0000 0000
 * ����: word����
 */
struct sdio_clkcr_bits {
    uint32 CLKDIV : 8;          /* ʱ�ӷ�Ƶϵ�� SDIO_CK = SDIOCLK / [CLKDIV + 2] */
    uint32 CLKEN : 1;           /* ʹ��ʱ��SDIO_CK */
    uint32 PWRSAV : 1;          /* ʡ��ģʽ���ڸ�ģʽ��ֻ�е�������Actived״̬�²�ʹ��sdio_ck */
    uint32 BYPASS : 1;          /* ��·ʱ�ӷ�Ƶ����ֱ����SDIOCLK����SDIO_CK */
    uint32 WIDBUS : 2;          /* ����λ��00: SDIO_D0, 01:SDIO_D[3:0], 10: SDIO_D[7:0] */
    uint32 NEGEDGE : 1;         /* ��SDIOCLK���½��ز���SDIO_CK */
    uint32 HWFC_EN : 1;         /* ʹ��Ӳ�������� */
    uint32 rsv : 17;
};
union sdio_clkcr {
    struct sdio_clkcr_bits bits;
    uint32 all;
};

#define SDIO_Response_No     (0x00)
#define SDIO_Response_Short  (0x01)
#define SDIO_Response_Long   (0x03)
/*
 * ָ��Ĵ��� SDIO_CMD
 * ƫ�Ƶ�ַ: 0x0C
 * ��λֵ: 0x0000 0000
 * ����: word����
 */
struct sdio_cmd_bits {
    uint32 CMDINDEX : 6;            /* ָ������ */
    uint32 WAITRESP : 2;            /* �ȴ���Ӧ���� */
    uint32 WAITINT : 1;             /* ����λΪ1��CPSM���ر�ָ�ʱ��⣬�ȴ�һ���ж����� */
    uint32 WAITPEND : 1;            /* ��Ϊ1��CPSM�ڷ�����һ��ָ��֮ǰ����ȴ�һ�����ݽ����ź� */
    uint32 CPSMEN : 1;              /* ʹ��Command Path State Machine */
    uint32 SDIOSuspend : 1;         /* ����ָ�� */
    uint32 ENCMDcompl : 1;          /* ��֪���� */
    uint32 nIEN : 1;                /* �ر�CE-ATA�豸���ж� */
    uint32 ATACMD : 1;              /* ��Ϊ1��CPSM����CMD61�����ǲ�֪���� */
    uint32 rsv : 17;
};

union sdio_cmd {
    struct sdio_cmd_bits bits;
    uint32 all;
};

struct sdio_respcmd_bits {
    uint32 RESPCMD : 6;
    uint32 rsv : 26;
};

union sdio_respcmd {
    struct sdio_respcmd_bits bits;
    uint32 all;
};

struct sdio_dlen_bits {
    uint32 datalen : 25;
    uint32 rsv : 7;
};

union sdio_dlen {
    struct sdio_dlen_bits bits;
    uint32 all;
};

struct sdio_dctrl_bits {
    uint32 DTEN : 1;
    uint32 DTDIR : 1;
    uint32 DTMODE : 1;
    uint32 DMAEN : 1;
    uint32 DBLOCKSIZE : 4;
    uint32 RWSTART : 1;
    uint32 RWSTOP : 1;
    uint32 RWMOD : 1;
    uint32 SDIOEN : 1;
    uint32 rsv : 20;
};

union sdio_dctrl {
    struct sdio_dctrl_bits bits;
    uint32 all;
};

struct sdio_dcount_bits {
    uint32 datacount : 25;
    uint32 rsv : 7;
};

union sdio_dcount {
    struct sdio_dcount_bits bits;
    uint32 all;
};

struct sdio_sta_bits {
    uint32 ccrcfail : 1;
    uint32 dcrcfail : 1;
    uint32 ctimeout : 1;
    uint32 dtimeout : 1;
    uint32 txunderr : 1;
    uint32 rxoverr : 1;
    uint32 cmdrend : 1;
    uint32 cmdsent : 1;
    uint32 dataend : 1;
    uint32 stbiterr : 1;
    uint32 dbckend : 1;
    uint32 cmdact : 1;
    uint32 txact : 1;
    uint32 rxact : 1;
    uint32 txfifohe : 1;
    uint32 rxfifohf : 1;
    uint32 txfifof : 1;
    uint32 rxfifof : 1;
    uint32 txfifoe : 1;
    uint32 rxfifoe : 1;
    uint32 txdavl : 1;
    uint32 rxdavl : 1;
    uint32 sdioit : 1;
    uint32 ceataend : 1;
    uint32 rsv : 8;
};

union sdio_sta {
    struct sdio_sta_bits bits;
    uint32 all;
};

struct sdio_icr_bits {
    uint32 ccrcfailc : 1;
    uint32 dcrcfailc : 1;
    uint32 ctimeoutc : 1;
    uint32 dtimeoutc : 1;
    uint32 txunderrc : 1;
    uint32 rxoverrc : 1;
    uint32 cmdrendc : 1;
    uint32 cmdsentc : 1;
    uint32 dataendc : 1;
    uint32 stbiterrc : 1;
    uint32 dbckendc : 1;
    uint32 rsv1 : 11;
    uint32 sdioitc : 1;
    uint32 ceataendc : 1;
    uint32 rsv2 : 8;
};

union sdio_icr {
    struct sdio_icr_bits bits;
    uint32 all;
};

typedef struct sdio_regs {
    volatile union sdio_power POWER;        /* SDIO�������ƼĴ���, offset: 0x00 */
    volatile union sdio_clkcr CLKCR;        /* SDIOʱ�ӿ��ƼĴ���, offset: 0x04 */
    volatile uint32 ARG;                    /* SDIOָ������Ĵ���, offset: 0x08 */
    volatile union sdio_cmd CMD;            /* SDIOָ��Ĵ���, offset: 0x0C */
    volatile union sdio_respcmd RESPCMD;    /* SDIOָ����Ӧ�Ĵ���, offset: 0x10 */
    volatile uint32  RESP1;                 /* SDIO��Ӧ�Ĵ���1, offset: 0x14 */
    volatile uint32  RESP2;                 /* SDIO��Ӧ�Ĵ���2, offset: 0x18 */
    volatile uint32  RESP3;                 /* SDIO��Ӧ�Ĵ���3, offset: 0x1C */
    volatile uint32  RESP4;                 /* SDIO��Ӧ�Ĵ���4, offset: 0x20 */
    volatile uint32 DTIMER;                 /* SDIO���ݳ�ʱ�Ĵ���, offset: 0x24 */
    volatile union sdio_dlen DLEN;          /* SDIO���ݳ��ȼĴ���, offset: 0x28 */
    volatile union sdio_dctrl DCTRL;        /* SDIO���ݿ��ƼĴ���, offset: 0x2C */
    volatile union sdio_dcount DCOUNT;      /* SDIO���ݼ����Ĵ���, offset: 0x30 */
    volatile union sdio_sta STA;            /* SDIO״̬�Ĵ���, offset: 0x34 */
    volatile union sdio_icr ICR;            /* SDIO����жϼĴ���, offset: 0x38 */
    volatile uint32 MASK;           /* SDIO mask register,             Address offset: 0x3C */
    uint32 RESERVED0[2];            /* Reserved, 0x40-0x44                                  */
    volatile uint32  FIFOCNT;       /* SDIO FIFO counter register,     Address offset: 0x48 */
    uint32 RESERVED1[13];           /* Reserved, 0x4C-0x7C                                  */
    volatile uint32 FIFO;           /* SDIO data FIFO register,        Address offset: 0x80 */
} sdio_regs_t;

/* SDIO�Ĵ�����ַӳ�� */
#define SDIO_BASE 0x40012C00
/* SDIO�Ĵ���ָ����� */
#define SDIO ((sdio_regs_t *) SDIO_BASE)

#endif // !STM32F407_SDIO_H
