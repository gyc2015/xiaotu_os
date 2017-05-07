#include <stm32f407_sdio.h>

void sdio_send_cmd(union sdio_cmd cmd, uint32 arg) {
    // 写入指令参数
    SDIO->ARG = arg;
    // 写入指令
    SDIO->CMD = cmd;
}

void sdio_config_data(union sdio_dctrl dctrl, uint32 timeout, uint32 dlen) {
    SDIO->DTIMER = timeout;
    SDIO->DLEN.bits.datalen = dlen;
    SDIO->DCTRL = dctrl;
}

