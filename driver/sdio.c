#include <sdio.h>
#include <stm32f407_rcc.h>
#include <stm32f407_gpio.h>

void sdio_init_hw(void) {
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

void sdio_reset(void) {
    RCC->APB2RSTR.bits.sdio = 1;
    RCC->APB2RSTR.bits.sdio = 0;
}

void sdio_send_cmd(union sdio_cmd cmd, uint32 arg) {
    // 写入指令参数
    SDIO->ARG = arg;
    // 写入指令
    SDIO->CMD = cmd;
}

static enum SD_Error sdio_check_cmd0(void) {
    enum SD_Error e = SDE_OK;
    uint32 timeout = 10000;
    
    while ((timeout > 0) && (0 == SDIO->STA.bits.cmdsent)) {
        timeout--;
    }

    if (0 == timeout) {
        e = SDE_CMD_RSP_TIMEOUT;
        return e;
    }

    SDIO->ICR.bits.cmdsentc = 1;
    return e;
}

/*
 * sdio_init - 初始化SDIO设备
 *
 * 在STM32F407中，SDIO的驱动时钟SDIOCLK频率为48MHz，由PLL直接驱动。
 * 正常情况下，输出的SDIO_CK频率不能超过400KHz
 * SDIO_CK = SDIOCLK / (CLKDIV + 2)
 */
enum SD_Error sdio_init(void) {
    enum SD_Error e = SDE_OK;
    union sdio_cmd cmd;

    sdio_init_hw();
    sdio_reset();

    SDIO->CLKCR.bits.CLKDIV = 118;
    SDIO->CLKCR.bits.NEGEDGE = FALSE;
    SDIO->CLKCR.bits.BYPASS = FALSE;
    SDIO->CLKCR.bits.PWRSAV = FALSE;
    SDIO->CLKCR.bits.WIDBUS = SDIO_BusWid_1;
    SDIO->CLKCR.bits.HWFC_EN = FALSE;

    SDIO->POWER.bits.PWRCTRL = SDIO_Power_On;

    SDIO->CLKCR.bits.CLKEN = 1;

    // CMD0: SD_CMD_GO_IDLE_STATE
    cmd.all = 0;
    cmd.bits.CMDINDEX = SDC_GO_IDLE_STATE;
    cmd.bits.WAITRESP = SDIO_Response_No;
    cmd.bits.CPSMEN = 1;
    sdio_send_cmd(cmd, 0);
    e = sdio_check_cmd0();
    if (SDE_OK != e)
        return e;

    return e;
}

