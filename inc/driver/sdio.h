#ifndef SDIO_H
#define SDIO_H

#include <stm32f407_sdio.h>

enum SD_Error {
    SDE_OK = 0,

    SDE_CMD_RSP_TIMEOUT = 3 /* ÷∏¡ÓœÏ”¶≥¨ ± */

};

enum SD_Cmd {
    SDC_GO_IDLE_STATE = 0
};

enum SD_Error sdio_init(void);

#endif

