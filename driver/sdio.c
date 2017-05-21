#include <sdio.h>
#include <stm32f407_rcc.h>
#include <stm32f407_gpio.h>

#include <utils.h>
#include <uart4.h>

static void sdio_init_hw(void) {
    struct gpio_pin_conf pincof;
    // 时钟
    RCC->AHB1ENR.bits.gpioc = 1;
    RCC->AHB1ENR.bits.gpiod = 1;
    RCC->APB2ENR.bits.sdio = 1;
    RCC->AHB1ENR.bits.dma2 = 1;
    // 功能选择
    GPIOC->AFR.bits.pin8 = GPIO_AF_SDIO;
    GPIOC->AFR.bits.pin9 = GPIO_AF_SDIO;
    GPIOC->AFR.bits.pin10 = GPIO_AF_SDIO;
    GPIOC->AFR.bits.pin11 = GPIO_AF_SDIO;
    GPIOC->AFR.bits.pin12 = GPIO_AF_SDIO;
    GPIOD->AFR.bits.pin2 = GPIO_AF_SDIO;
    // PC8, PC9, PC10, PC11 ==> D0, D1, D2, D3
    pincof.mode = GPIO_Mode_Af;
    pincof.otype = GPIO_OType_PP;
    pincof.speed = GPIO_OSpeed_Medium;
    pincof.pull = GPIO_Pull_Up;
    gpio_init(GPIOC, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11, &pincof);
    // PD2 ==> CMD
    gpio_init(GPIOD, GPIO_Pin_2, &pincof);
    // PC12 ==> CLK
    pincof.pull = GPIO_Pull_No;
    gpio_init(GPIOC, GPIO_Pin_12, &pincof);
}

static enum SD_Error sdio_check_cmd0(void) {
    enum SD_Error e = SDE_ERROR;
    uint32 timeout = SDIO_TIMEOUT;
    
    while ((timeout > 0) && (0 == SDIO->STA.bits.cmdsent))
        timeout--;

    e = (0 == timeout) ? SDE_CMD_RSP_TIMEOUT : SDE_OK;

    SDIO->ICR.bits.cmdsentc = 1;
    return e;
}

static enum SD_Error sdio_check_resp1(uint8 cmd) {
    uint32 status = SDIO->STA.all;

    while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
        status = SDIO->STA.all;

    if (status & SDIO_FLAG_CTIMEOUT) {
        SDIO->ICR.bits.ctimeoutc = 1;
        return SDE_CMD_RSP_TIMEOUT;
    } else if (status & SDIO_FLAG_CCRCFAIL) {
        SDIO->ICR.bits.ccrcfailc = 1;
        return SDE_CMD_CRC_FAIL;
    }

    if (SDIO->RESPCMD.bits.RESPCMD != cmd) {
        return SDE_ILLEGAL_CMD;
    }

    // 清除所有状态
    SDIO->ICR.all = 0x05FF;
    uint32 r1 = SDIO->RESP1;

    if (0 == (r1 & SD_OCR_ERRORBITS))
        return SDE_OK;

    if (r1 & SD_OCR_ADDR_OUT_OF_RANGE)
        return SDE_ADDR_OUT_OF_RANGE;

    if (r1 & SD_OCR_ADDR_MISALIGNED)
        return(SDE_ADDR_MISALIGNED);

    if (r1 & SD_OCR_BLOCK_LEN_ERR)
        return(SDE_BLOCK_LEN_ERR);

    if (r1 & SD_OCR_ERASE_SEQ_ERR)
        return(SDE_ERASE_SEQ_ERR);

    if (r1 & SD_OCR_BAD_ERASE_PARAM)
        return(SDE_BAD_ERASE_PARAM);

    if (r1 & SD_OCR_WRITE_PROT_VIOLATION)
        return(SDE_WRITE_PROT_VIOLATION);

    if (r1 & SD_OCR_LOCK_UNLOCK_FAILED)
        return(SDE_LOCK_UNLOCK_FAILED);

    if (r1 & SD_OCR_COM_CRC_FAILED)
        return(SDE_COM_CRC_FAILED);

    if (r1 & SD_OCR_ILLEGAL_CMD)
        return(SDE_ILLEGAL_CMD);

    if (r1 & SD_OCR_CARD_ECC_FAILED)
        return(SDE_CARD_ECC_FAILED);

    if (r1 & SD_OCR_CC_ERROR)
        return(SDE_CC_ERROR);

    if (r1 & SD_OCR_GENERAL_UNKNOWN_ERROR)
        return(SDE_GENERAL_UNKNOWN_ERROR);

    if (r1 & SD_OCR_STREAM_READ_UNDERRUN)
        return(SDE_STREAM_READ_UNDERRUN);

    if (r1 & SD_OCR_STREAM_WRITE_OVERRUN)
        return(SDE_STREAM_WRITE_OVERRUN);

    if (r1 & SD_OCR_CID_CSD_OVERWRIETE)
        return(SDE_CID_CSD_OVERWRITE);

    if (r1 & SD_OCR_WP_ERASE_SKIP)
        return(SDE_WP_ERASE_SKIP);

    if (r1 & SD_OCR_CARD_ECC_DISABLED)
        return(SDE_CARD_ECC_DISABLED);

    if (r1 & SD_OCR_ERASE_RESET)
        return(SDE_ERASE_RESET);

    if (r1 & SD_OCR_AKE_SEQ_ERROR)
        return(SDE_AKE_SEQ_ERROR);

    return SDE_ERROR;
}

static enum SD_Error sdio_check_resp2(void) {
    uint32 status = SDIO->STA.all;

    while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
        status = SDIO->STA.all;

    if (status & SDIO_FLAG_CTIMEOUT) {
        SDIO->ICR.bits.ctimeoutc = 1;
        return SDE_CMD_RSP_TIMEOUT;
    } else if (status & SDIO_FLAG_CCRCFAIL) {
        SDIO->ICR.bits.ccrcfailc = 1;
        return SDE_CMD_CRC_FAIL;
    }

    SDIO->ICR.all = 0x05FF;
    return SDE_OK;
}

static enum SD_Error sdio_check_resp3(void) {
    uint32 status = SDIO->STA.all;

    while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
        status = SDIO->STA.all;

    if (status & SDIO_FLAG_CTIMEOUT) {
        SDIO->ICR.bits.ctimeoutc = 1;
        return SDE_CMD_RSP_TIMEOUT;
    }
    // 为什么不检测CRC校验
    SDIO->ICR.all = 0x05FF;
    return SDE_OK;
}

static enum SD_Error sdio_check_resp6(uint8 cmd, uint16 *rca) {
    uint32 status = SDIO->STA.all;

    while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
        status = SDIO->STA.all;

    if (status & SDIO_FLAG_CTIMEOUT) {
        SDIO->ICR.bits.ctimeoutc = 1;
        return SDE_CMD_RSP_TIMEOUT;
    } else if (status & SDIO_FLAG_CCRCFAIL) {
        SDIO->ICR.bits.ccrcfailc = 1;
        return SDE_CMD_CRC_FAIL;
    }

    if (SDIO->RESPCMD.bits.RESPCMD != cmd) {
        return SDE_ILLEGAL_CMD;
    }

    // 清除所有状态
    SDIO->ICR.all = 0x05FF;
    uint32 r1 = SDIO->RESP1;

    if (r1 & SD_R6_GENERAL_UNKNOWN_ERROR)
        return SDE_GENERAL_UNKNOWN_ERROR;
    if (r1 & SD_R6_ILLEGAL_CMD)
        return SDE_ILLEGAL_CMD;
    if (r1 & SD_R6_COM_CRC_FAILED)
        return SDE_COM_CRC_FAILED;

    *rca = (uint16)(r1 >> 16);
    return SDE_OK;
}

static enum SD_Error sdio_check_resp7(void) {
    enum SD_Error e = SDE_ERROR;
    uint32 status = SDIO->STA.all;

    while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT))) {
        status = SDIO->STA.all;
    }

    if (status & SDIO_FLAG_CTIMEOUT) {
        e = SDE_CMD_RSP_TIMEOUT;
        SDIO->ICR.bits.ctimeoutc = 1;
    } else if (status & SDIO_FLAG_CCRCFAIL) {
        e = SDE_CMD_CRC_FAIL;
        SDIO->ICR.bits.ccrcfailc = 1;
    } else if (SDIO_FLAG_CMDREND & status) {
        e = SDE_OK;
        SDIO->ICR.bits.cmdrendc = 1;
    }

    return e;
}

static enum SD_Error sdio_cmd0(void) {
    union sdio_cmd cmd;
    // CMD0: SD_CMD_GO_IDLE_STATE
    cmd.all = 0;
    cmd.bits.CMDINDEX = SD_CMD_GO_IDLE_STATE;
    cmd.bits.WAITRESP = SDIO_Response_No;
    cmd.bits.CPSMEN = 1;
    sdio_send_cmd(cmd, 0);
    return sdio_check_cmd0();
}

static enum SD_Error sdio_cmd8(void) {
    union sdio_cmd cmd;
    // CMD8: 验证SD卡接口，不知道为啥是SDIO_SEND_IF_COND，
    // 看样子和SD_CMD_HS_SEND_EXT_CSD是冲突的
    cmd.all = 0;
    cmd.bits.CMDINDEX = SDIO_CMD_SEND_IF_COND;
    cmd.bits.WAITRESP = SDIO_Response_Short;
    cmd.bits.CPSMEN = 1;
    sdio_send_cmd(cmd, SD_CHECK_PATTERN);
    return sdio_check_resp7();
}

static enum SD_Error sdio_cmd55(uint32 arg) {
    union sdio_cmd cmd;
    // CMD55: 不知道干嘛的
    cmd.all = 0;
    cmd.bits.CMDINDEX = SD_CMD_APP_CMD;
    cmd.bits.WAITRESP = SDIO_Response_Short;
    cmd.bits.CPSMEN = 1;
    sdio_send_cmd(cmd, arg);
    return sdio_check_resp1(SD_CMD_APP_CMD);
}

static enum SD_Error sdio_cmd41(void) {
    union sdio_cmd cmd;
    // CMD41: 不知道干嘛的
    cmd.all = 0;
    cmd.bits.CMDINDEX = SD_CMD_SD_APP_OP_COND;
    cmd.bits.WAITRESP = SDIO_Response_Short;
    cmd.bits.CPSMEN = 1;
    sdio_send_cmd(cmd, SD_VOLTAGE_WINDOW_SD | SD_HIGH_CAPACITY);
    return sdio_check_resp3();
}

/*
 * sdio_find_scr - 获取SD卡的 SCR寄存器的值
 *
 * @rca: 选择SD卡地址
 * @pscr: 输出SCR寄存器的缓存地址
 */
static enum SD_Error sdio_find_scr(uint16 rca, uint32 *pscr) {
    enum SD_Error e = SDE_OK;
    union sdio_cmd cmd;
    union sdio_dctrl dctrl;

    // 设置Block大小为8字节
    cmd.all = 0;
    cmd.bits.CMDINDEX = SD_CMD_SET_BLOCKLEN;
    cmd.bits.WAITRESP = SDIO_Response_Short;
    cmd.bits.CPSMEN = 1;
    sdio_send_cmd(cmd, 8);
    e = sdio_check_resp1(SD_CMD_SET_BLOCKLEN);
    if (SDE_OK != e)
        return e;

    e = sdio_cmd55((uint32)rca << 16);
    if (SDE_OK != e)
        return e;

    dctrl.all = SDIO->DCTRL.all;
    dctrl.bits.DBLOCKSIZE = SDIO_DataBlockSize_8b;
    dctrl.bits.DTDIR = SDIO_TransDir_ToSdio;
    dctrl.bits.DTMODE = SDIO_TransMode_Block;
    dctrl.bits.DTEN = 1;
    sdio_config_data(dctrl, SD_DATATIMEOUT, 8);

    cmd.all = 0;
    cmd.bits.CMDINDEX = SD_CMD_SD_APP_SEND_SCR;
    cmd.bits.WAITRESP = SDIO_Response_Short;
    cmd.bits.CPSMEN = 1;
    sdio_send_cmd(cmd, 0);
    e = sdio_check_resp1(SD_CMD_SD_APP_SEND_SCR);
    if (SDE_OK != e)
        return e;

    uint32 i = 1;
    union sdio_sta status = SDIO->STA;
    while (!(status.all & (SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR))) {
        if (status.bits.rxdavl == 1) {
            pscr[i] = change_ending_32b(SDIO->FIFO);
            i--;
        }
        status = SDIO->STA;
    }

    if (status.bits.dtimeout) {
        SDIO->ICR.bits.dtimeoutc = 1;
        return SDE_DATA_TIMEOUT;
    } else if (status.bits.dcrcfail) {
        SDIO->ICR.bits.dcrcfailc = 1;
        return SDE_DATA_CRC_FAIL;
    } else if (status.bits.rxoverr) {
        SDIO->ICR.bits.rxoverrc = 1;
        return SDE_RX_OVERRUN;
    } else if (status.bits.stbiterr) {
        SDIO->ICR.bits.stbiterrc = 1;
        return SDE_START_BIT_ERR;
    }

    SDIO->ICR.all = 0x05FF;

    return e;
}

enum SD_Error sdio_poweron(struct sd_card *card) {
    enum SD_Error e = SDE_OK;

    // CMD0: SD_CMD_GO_IDLE_STATE
    e = sdio_cmd0();
    if (SDE_OK != e)
        return e;
    // CMD8: 验证SD卡接口，不知道为啥是SDIO_SEND_IF_COND，
    // 看样子和SD_CMD_HS_SEND_EXT_CSD是冲突的
    e = sdio_cmd8();
    if (SDE_OK != e)
        return e;

    card->cardtype = SDIO_STD_CAPACITY_SD_CARD_V2_0;

    e = sdio_cmd55(0);
    if (SDE_OK != e)
        return e;

    bool validvoltage = FALSE;
    uint32 count = 0;
    uint32 res = 0;
    while (!validvoltage && (count < SD_MAX_VOLT_TRIAL)) {
        e = sdio_cmd55(0);
        if (SDE_OK != e)
            return e;
        e = sdio_cmd41();
        if (SDE_OK != e)
            return e;
        res = SDIO->RESP1;
        validvoltage = (((res >> 31) == 1) ? TRUE : FALSE);
        count++;
    }

    if (count >= SD_MAX_VOLT_TRIAL)
        return SDE_INVALID_VOLTRANGE;

    return e;
}

enum SD_Error sdio_init_card(struct sd_card *card) {
    enum SD_Error e = SDE_OK;

    if (SDIO_Power_Off == SDIO->POWER.bits.PWRCTRL) {
        e = SDE_ERROR;
        return e;
    }
    union sdio_cmd cmd;
    // CMD41: 不知道干嘛的
    cmd.all = 0;
    cmd.bits.CMDINDEX = SD_CMD_ALL_SEND_CID;
    cmd.bits.WAITRESP = SDIO_Response_Long;
    cmd.bits.CPSMEN = 1;
    sdio_send_cmd(cmd, 0);
    e = sdio_check_resp2();
    if (SDE_OK != e)
        return e;
    card->cid.words[0] = SDIO->RESP1;
    card->cid.words[1] = SDIO->RESP2;
    card->cid.words[2] = SDIO->RESP3;
    card->cid.words[3] = SDIO->RESP4;

    // CMD3
    cmd.all = 0;
    cmd.bits.CMDINDEX = SD_CMD_SET_REL_ADDR;
    cmd.bits.WAITRESP = SDIO_Response_Short;
    cmd.bits.CPSMEN = 1;
    sdio_send_cmd(cmd, 0);
    uint16 rca = 0x01;
    e = sdio_check_resp6(SD_CMD_SET_REL_ADDR, &rca);
    if (SDE_OK != e)
        return e;
    card->rca = rca;
    // CMD9
    cmd.all = 0;
    cmd.bits.CMDINDEX = SD_CMD_SEND_CSD;
    cmd.bits.WAITRESP = SDIO_Response_Long;
    cmd.bits.CPSMEN = 1;
    sdio_send_cmd(cmd, (uint32)(rca << 16));
    e = sdio_check_resp2();
    if (SDE_OK != e)
        return e;
    card->csd.words[0] = SDIO->RESP1;
    card->csd.words[1] = SDIO->RESP2;
    card->csd.words[2] = SDIO->RESP3;
    card->csd.words[3] = SDIO->RESP4;

    return SDE_OK;
}

/*
 * sdio_sel_desel - 选择设备
 */
enum SD_Error sdio_sel(uint16 rca) {
    union sdio_cmd cmd;
    cmd.all = 0;
    cmd.bits.CMDINDEX = SD_CMD_SEL_DESEL_CARD;
    cmd.bits.WAITRESP = SDIO_Response_Short;
    cmd.bits.CPSMEN = 1;
    sdio_send_cmd(cmd, (uint32)rca << 16);
    return sdio_check_resp1(SD_CMD_SEL_DESEL_CARD);
}
/*
 * sdio_en_widemode - 开启多线传输模式
 *
 * 只支持4线
 *
 * @rca: 设备地址
 */
enum SD_Error sdio_en_widemode(uint16 rca) {
    enum SD_Error e = SDE_OK;
    union sdio_cmd cmd;
    uint32 scr[2] = { 0, 0 };

    if (SDIO->RESP1 & SD_CARD_LOCKED) {
        e = SDE_LOCK_UNLOCK_FAILED;
        return e;
    }

    e = sdio_find_scr(rca, scr);
    if (SDE_OK != e)
        return e;

    if (SD_WIDE_BUS_SUPPORT & scr[1]) {
        e = sdio_cmd55((uint32)rca << 16);
        if (SDE_OK != e)
            return e;

        cmd.all = 0;
        cmd.bits.CMDINDEX = SD_CMD_APP_SD_SET_BUSWIDTH;
        cmd.bits.WAITRESP = SDIO_Response_Short;
        cmd.bits.CPSMEN = 1;
        sdio_send_cmd(cmd, 2);
        e = sdio_check_resp1(SD_CMD_APP_SD_SET_BUSWIDTH);
        if (SDE_OK != e)
            return e;

        sdio_init_clkcr(0, SDIO_BusWid_4);
        return e;
    } else {
        return SDE_ERROR;
    }
}
/*
 * sdio_en_widemode - 关闭多线传输模式
 *
 * @rca: 设备地址
 */
enum SD_Error sdio_dis_widemode(uint16 rca) {
    enum SD_Error e = SDE_OK;
    union sdio_cmd cmd;
    uint32 scr[2] = { 0, 0 };

    if (SDIO->RESP1 & SD_CARD_LOCKED) {
        e = SDE_LOCK_UNLOCK_FAILED;
        return e;
    }

    e = sdio_find_scr(rca, scr);
    if (SDE_OK != e)
        return e;

    if (SD_SINGLE_BUS_SUPPORT & scr[1]) {
        e = sdio_cmd55((uint32)rca << 16);
        if (SDE_OK != e)
            return e;

        cmd.all = 0;
        cmd.bits.CMDINDEX = SD_CMD_APP_SD_SET_BUSWIDTH;
        cmd.bits.WAITRESP = SDIO_Response_Short;
        cmd.bits.CPSMEN = 1;
        sdio_send_cmd(cmd, 0);
        e = sdio_check_resp1(SD_CMD_APP_SD_SET_BUSWIDTH);
        if (SDE_OK != e)
            return e;

        sdio_init_clkcr(0, SDIO_BusWid_1);
        return e;
    } else {
        return SDE_ERROR;
    }
}

/*
 * sdio_init - 初始化SDIO设备
 */
enum SD_Error sdio_init(struct sd_card *card) {
    enum SD_Error e = SDE_OK;
    
    sdio_init_hw();

    sdio_init_clkcr(118, SDIO_BusWid_1);

    SDIO->POWER.bits.PWRCTRL = SDIO_Power_On;
    SDIO->CLKCR.bits.CLKEN = 1;

    e = sdio_poweron(card);
    if (SDE_OK != e)
        return e;
    e = sdio_init_card(card);
    if (SDE_OK != e)
        return e;

    card->blocknum = ((card->csd.bits.DeviceSizeH << 2) + card->csd.bits.DeviceSizeL);
    card->blocknum += 1;
    card->blocknum *= (1 << card->csd.bits.DeviceSizeMul + 2);
    card->blocksize = 1 << card->csd.bits.RdBlockLen;
    card->capacity = card->blocknum * card->blocksize;
    
    // 不明白为什么要再来一遍
    // 大概是数据通信可以到25MHz，指令通信只能在400KHz下
    sdio_init_clkcr(0, SDIO_BusWid_1);

    e = sdio_sel(card->rca);
    if (SDE_OK != e)
        return e;

    e = sdio_en_widemode(card->rca);

    SDIO->MASK.bits.dcrcfaile = 1;
    SDIO->MASK.bits.dtimeoute = 1;
    SDIO->MASK.bits.dataende = 1;
    SDIO->MASK.bits.rxoverre = 1;
    SDIO->MASK.bits.stbiterre = 1;
    return e;
}
/*
 * sdio_read_block - 读一个block的数据,通过DMA实现
 *
 * @card: 目标SD卡
 * @addr: 需要读取block地址
 * @buf: 缓存地址
 */
enum SD_Error sdio_read_block(struct sd_card *card, uint32 addr, uint8 *buf) {
    SDIO->DCTRL.all = 0;

    uint16 blocksize = card->blocksize;
    uint32 blocknum = addr / blocksize;

    enum SD_Error e = SDE_OK;
    union sdio_cmd cmd;
    union sdio_dctrl dctrl;

    // 设置Block大小
    cmd.all = 0;
    cmd.bits.CMDINDEX = SD_CMD_SET_BLOCKLEN;
    cmd.bits.WAITRESP = SDIO_Response_Short;
    cmd.bits.CPSMEN = 1;
    sdio_send_cmd(cmd, blocksize);
    e = sdio_check_resp1(SD_CMD_SET_BLOCKLEN);
    if (SDE_OK != e)
        return e;
    // 设置数据
    dctrl.all = SDIO->DCTRL.all;
    dctrl.bits.DBLOCKSIZE = SDIO_DataBlockSize_512b;
    dctrl.bits.DTDIR = SDIO_TransDir_ToSdio;
    dctrl.bits.DTMODE = SDIO_TransMode_Block;
    dctrl.bits.DTEN = 1;
    sdio_config_data(dctrl, SD_DATATIMEOUT, blocksize);
    // CMD17
    cmd.all = 0;
    cmd.bits.CMDINDEX = SD_CMD_READ_SINGLE_BLOCK;
    cmd.bits.WAITRESP = SDIO_Response_Short;
    cmd.bits.CPSMEN = 1;
    sdio_send_cmd(cmd, blocknum);
    e = sdio_check_resp1(SD_CMD_READ_SINGLE_BLOCK);
    if (SDE_OK != e)
        return e;

    SDIO->DCTRL.bits.DMAEN = 1;
    sdio_config_dma((uint32 *)buf, blocksize);
    return e;
}


