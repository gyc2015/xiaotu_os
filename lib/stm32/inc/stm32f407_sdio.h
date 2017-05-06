#ifndef STM32F407_SDIO_H
#define STM32F407_SDIO_H

#include <types.h>

#define SDIO_Power_Off 0x00
#define SDIO_Power_Up 0x02
#define SDIO_Power_On 0x03

/*
 * 电力控制寄存器 SDIO_POWER
 * 偏移地址: 0x00
 * 复位值: 0x0000 0000
 * 访问: word访问
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
 * 时钟控制寄存器 SDIO_CLKCR
 * 偏移地址: 0x04
 * 复位值: 0x0000 0000
 * 访问: word访问
 */
struct sdio_clkcr_bits {
    uint32 CLKDIV : 8;          /* 时钟分频系数 SDIO_CK = SDIOCLK / [CLKDIV + 2] */
    uint32 CLKEN : 1;           /* 使能时钟SDIO_CK */
    uint32 PWRSAV : 1;          /* 省电模式，在该模式下只有当总线在Actived状态下才使能sdio_ck */
    uint32 BYPASS : 1;          /* 旁路时钟分频器，直接由SDIOCLK驱动SDIO_CK */
    uint32 WIDBUS : 2;          /* 总线位宽，00: SDIO_D0, 01:SDIO_D[3:0], 10: SDIO_D[7:0] */
    uint32 NEGEDGE : 1;         /* 在SDIOCLK的下降沿产生SDIO_CK */
    uint32 HWFC_EN : 1;         /* 使能硬件流控制 */
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
 * 指令寄存器 SDIO_CMD
 * 偏移地址: 0x0C
 * 复位值: 0x0000 0000
 * 访问: word访问
 */
struct sdio_cmd_bits {
    uint32 CMDINDEX : 6;            /* 指令索引 */
    uint32 WAITRESP : 2;            /* 等待响应类型 */
    uint32 WAITINT : 1;             /* 若该位为1，CPSM将关闭指令超时检测，等待一个中断请求 */
    uint32 WAITPEND : 1;            /* 若为1，CPSM在发送下一条指令之前，会等待一个数据结束信号 */
    uint32 CPSMEN : 1;              /* 使能Command Path State Machine */
    uint32 SDIOSuspend : 1;         /* 挂起指令 */
    uint32 ENCMDcompl : 1;          /* 不知所云 */
    uint32 nIEN : 1;                /* 关闭CE-ATA设备的中断 */
    uint32 ATACMD : 1;              /* 若为1，CPSM发送CMD61，还是不知所云 */
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
    volatile union sdio_power POWER;        /* SDIO电力控制寄存器, offset: 0x00 */
    volatile union sdio_clkcr CLKCR;        /* SDIO时钟控制寄存器, offset: 0x04 */
    volatile uint32 ARG;                    /* SDIO指令参数寄存器, offset: 0x08 */
    volatile union sdio_cmd CMD;            /* SDIO指令寄存器, offset: 0x0C */
    volatile union sdio_respcmd RESPCMD;    /* SDIO指令响应寄存器, offset: 0x10 */
    volatile uint32  RESP1;                 /* SDIO响应寄存器1, offset: 0x14 */
    volatile uint32  RESP2;                 /* SDIO响应寄存器2, offset: 0x18 */
    volatile uint32  RESP3;                 /* SDIO响应寄存器3, offset: 0x1C */
    volatile uint32  RESP4;                 /* SDIO响应寄存器4, offset: 0x20 */
    volatile uint32 DTIMER;                 /* SDIO数据超时寄存器, offset: 0x24 */
    volatile union sdio_dlen DLEN;          /* SDIO数据长度寄存器, offset: 0x28 */
    volatile union sdio_dctrl DCTRL;        /* SDIO数据控制寄存器, offset: 0x2C */
    volatile union sdio_dcount DCOUNT;      /* SDIO数据计数寄存器, offset: 0x30 */
    volatile union sdio_sta STA;            /* SDIO状态寄存器, offset: 0x34 */
    volatile union sdio_icr ICR;            /* SDIO清除中断寄存器, offset: 0x38 */
    volatile uint32 MASK;           /* SDIO mask register,             Address offset: 0x3C */
    uint32 RESERVED0[2];            /* Reserved, 0x40-0x44                                  */
    volatile uint32  FIFOCNT;       /* SDIO FIFO counter register,     Address offset: 0x48 */
    uint32 RESERVED1[13];           /* Reserved, 0x4C-0x7C                                  */
    volatile uint32 FIFO;           /* SDIO data FIFO register,        Address offset: 0x80 */
} sdio_regs_t;

/* SDIO寄存器地址映射 */
#define SDIO_BASE 0x40012C00
/* SDIO寄存器指针访问 */
#define SDIO ((sdio_regs_t *) SDIO_BASE)

#endif // !STM32F407_SDIO_H
