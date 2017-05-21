#ifndef SDIO_H
#define SDIO_H

#include <stm32f407_sdio.h>

enum SD_Error {
    SDE_OK = 0x00,
    SDE_ERROR = 0xFF,

    SDE_CMD_CRC_FAIL            = 1,    /* 接收到响应(但是CRC校验错误) */
    SDE_DATA_CRC_FAIL           = 2, /*!< Data bock sent/received (CRC check Failed) */
    SDE_CMD_RSP_TIMEOUT         = 3,    /* 指令响应超时 */
    SDE_DATA_TIMEOUT            = 4, /*!< Data time out */
    SDE_TX_UNDERRUN             = 5, /*!< Transmit FIFO under-run */
    SDE_RX_OVERRUN              = 6, /*!< Receive FIFO over-run */
    SDE_START_BIT_ERR           = 7, /*!< Start bit not detected on all data signals in widE bus mode */
    SDE_CMD_OUT_OF_RANGE        = 8, /*!< CMD's argument was out of range.*/
    SDE_ADDR_MISALIGNED         = 9, /*!< Misaligned address */
    SDE_BLOCK_LEN_ERR           = 10, /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
    SDE_ERASE_SEQ_ERR           = 11, /*!< An error in the sequence of erase command occurs.*/
    SDE_BAD_ERASE_PARAM         = 12, /*!< An Invalid selection for erase groups */
    SDE_WRITE_PROT_VIOLATION    = 13, /*!< Attempt to program a write protect block */
    SDE_LOCK_UNLOCK_FAILED      = 14, /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
    SDE_COM_CRC_FAILED          = 15, /*!< CRC check of the previous command failed */
    SDE_ILLEGAL_CMD             = 16,   /* 非法指令 */
    SDE_CARD_ECC_FAILED         = 17, /*!< Card internal ECC was applied but failed to correct the data */
    SDE_CC_ERROR                = 18, /*!< Internal card controller error */
    SDE_GENERAL_UNKNOWN_ERROR   = 19, /*!< General or Unknown error */
    SDE_STREAM_READ_UNDERRUN    = 20, /*!< The card could not sustain data transfer in stream read operation. */
    SDE_STREAM_WRITE_OVERRUN    = 21, /*!< The card could not sustain data programming in stream mode */
    SDE_CID_CSD_OVERWRITE       = 22, /*!< CID/CSD overwrite error */
    SDE_WP_ERASE_SKIP           = 23, /*!< only partial address space was erased */
    SDE_CARD_ECC_DISABLED       = 24, /*!< Command has been executed without using internal ECC */
    SDE_ERASE_RESET             = 25, /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
    SDE_AKE_SEQ_ERROR           = 26, /*!< Error in sequence of authentication. */
    SDE_INVALID_VOLTRANGE       = 27,
    SDE_ADDR_OUT_OF_RANGE       = 28,
    SDE_SWITCH_ERROR            = 29,
    SDE_SDIO_DISABLED           = 30,
    SDE_SDIO_FUNCTION_BUSY      = 31,
    SDE_SDIO_FUNCTION_FAILED    = 32,
    SDE_SDIO_UNKNOWN_FUNCTION   = 33

};

#define SD_CMD_GO_IDLE_STATE                       ((uint8)0)
#define SD_CMD_SEND_OP_COND                        ((uint8)1)
#define SD_CMD_ALL_SEND_CID                        ((uint8)2)
#define SD_CMD_SET_REL_ADDR                        ((uint8)3) /*!< SDIO_SEND_REL_ADDR for SD Card */
#define SD_CMD_SET_DSR                             ((uint8)4)
#define SD_CMD_SDIO_SEN_OP_COND                    ((uint8)5)
#define SD_CMD_HS_SWITCH                           ((uint8)6)
#define SD_CMD_SEL_DESEL_CARD                      ((uint8)7)
#define SD_CMD_HS_SEND_EXT_CSD                     ((uint8)8)
#define SD_CMD_SEND_CSD                            ((uint8)9)
#define SD_CMD_SEND_CID                            ((uint8)10)
#define SD_CMD_READ_DAT_UNTIL_STOP                 ((uint8)11) /*!< SD Card doesn't support it */
#define SD_CMD_STOP_TRANSMISSION                   ((uint8)12)
#define SD_CMD_SEND_STATUS                         ((uint8)13)
#define SD_CMD_HS_BUSTEST_READ                     ((uint8)14)
#define SD_CMD_GO_INACTIVE_STATE                   ((uint8)15)
#define SD_CMD_SET_BLOCKLEN                        ((uint8)16)
#define SD_CMD_READ_SINGLE_BLOCK                   ((uint8)17)
#define SD_CMD_READ_MULT_BLOCK                     ((uint8)18)
#define SD_CMD_HS_BUSTEST_WRITE                    ((uint8)19)
#define SD_CMD_WRITE_DAT_UNTIL_STOP                ((uint8)20) /*!< SD Card doesn't support it */
#define SD_CMD_SET_BLOCK_COUNT                     ((uint8)23) /*!< SD Card doesn't support it */
#define SD_CMD_WRITE_SINGLE_BLOCK                  ((uint8)24)
#define SD_CMD_WRITE_MULT_BLOCK                    ((uint8)25)
#define SD_CMD_PROG_CID                            ((uint8)26) /*!< reserved for manufacturers */
#define SD_CMD_PROG_CSD                            ((uint8)27)
#define SD_CMD_SET_WRITE_PROT                      ((uint8)28)
#define SD_CMD_CLR_WRITE_PROT                      ((uint8)29)
#define SD_CMD_SEND_WRITE_PROT                     ((uint8)30)
#define SD_CMD_SD_ERASE_GRP_START                  ((uint8)32) /*!< To set the address of the first write block to be erased. (For SD card only) */
#define SD_CMD_SD_ERASE_GRP_END                    ((uint8)33) /*!< To set the address of the last write block of the continuous range to be erased. (For SD card only) */
#define SD_CMD_ERASE_GRP_START                     ((uint8)35) /*!< To set the address of the first write block to be erased. (For MMC card only spec 3.31) */
#define SD_CMD_ERASE_GRP_END                       ((uint8)36) /*!< To set the address of the last write block of the continuous range to be erased. (For MMC card only spec 3.31) */
#define SD_CMD_ERASE                               ((uint8)38)
#define SD_CMD_FAST_IO                             ((uint8)39) /*!< SD Card doesn't support it */
#define SD_CMD_GO_IRQ_STATE                        ((uint8)40) /*!< SD Card doesn't support it */
#define SD_CMD_LOCK_UNLOCK                         ((uint8)42)
#define SD_CMD_APP_CMD                             ((uint8)55)
#define SD_CMD_GEN_CMD                             ((uint8)56)
#define SD_CMD_NO_CMD                              ((uint8)64)
/*
 * Following commands are SD Card Specific commands.
 * SDIO_APP_CMD should be sent before sending these commands.
 */
#define SD_CMD_APP_SD_SET_BUSWIDTH                 ((uint8)6)  /*!< For SD Card only */
#define SD_CMD_SD_APP_STAUS                        ((uint8)13) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        ((uint8)22) /*!< For SD Card only */
#define SD_CMD_SD_APP_OP_COND                      ((uint8)41) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT          ((uint8)42) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_SCR                     ((uint8)51) /*!< For SD Card only */
#define SD_CMD_SDIO_RW_DIRECT                      ((uint8)52) /*!< For SD I/O Card only */
#define SD_CMD_SDIO_RW_EXTENDED                    ((uint8)53) /*!< For SD I/O Card only */


#define SDIO_CMD_SEND_IF_COND       (8)

#define SDIO_TIMEOUT            (10000)

#define SD_CHECK_PATTERN        ((uint32)0x000001AA)

/*
 * Supported SD Memory Cards
 */
#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((uint32)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((uint32)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((uint32)0x00000002)
#define SDIO_MULTIMEDIA_CARD                       ((uint32)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD                ((uint32)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((uint32)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((uint32)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD                ((uint32)0x00000007)

/*
 * Mask for errors Card Status R1 (OCR Register)
 */
#define SD_OCR_ADDR_OUT_OF_RANGE        ((uint32)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((uint32)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((uint32)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((uint32)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((uint32)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((uint32)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((uint32)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((uint32)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((uint32)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((uint32)0x00200000)
#define SD_OCR_CC_ERROR                 ((uint32)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((uint32)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN     ((uint32)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((uint32)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE       ((uint32)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((uint32)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((uint32)0x00004000)
#define SD_OCR_ERASE_RESET              ((uint32)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR            ((uint32)0x00000008)
#define SD_OCR_ERRORBITS                ((uint32)0xFDFFE008)

/*
 * Masks for R6 Response
 */
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((uint32)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((uint32)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((uint32)0x00008000)

#define SD_VOLTAGE_WINDOW_SD            ((uint32)0x80100000)
#define SD_HIGH_CAPACITY                ((uint32)0x40000000)
#define SD_STD_CAPACITY                 ((uint32)0x00000000)
#define SD_CHECK_PATTERN                ((uint32)0x000001AA)

#define SD_MAX_VOLT_TRIAL               ((uint32)0x0000FFFF)
#define SD_ALLZERO                      ((uint32)0x00000000)

#define SD_WIDE_BUS_SUPPORT             ((uint32)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((uint32)0x00010000)
#define SD_CARD_LOCKED                  ((uint32)0x02000000)

#define SD_DATATIMEOUT                  ((uint32)0xFFFFFFFF)
#define SD_0TO7BITS                     ((uint32)0x000000FF)
#define SD_8TO15BITS                    ((uint32)0x0000FF00)
#define SD_16TO23BITS                   ((uint32)0x00FF0000)
#define SD_24TO31BITS                   ((uint32)0xFF000000)
#define SD_MAX_DATA_LENGTH              ((uint32)0x01FFFFFF)

#define SD_HALFFIFO                     ((uint32)0x00000008)
#define SD_HALFFIFOBYTES                ((uint32)0x00000020)

/*
 * card specification data
 */
struct sd_csd_bits {
    // csd_tab[0]
    uint32 MaxBusClkFrec : 8;
    uint32 NSAC : 8;
    uint32 TAAC : 8;
    uint32 rsv1 : 2;
    uint32 SysSpecVersion : 4;
    uint32 CSDStruct : 2;
    // csd_tab[1]
    uint32 DeviceSizeH : 10;
    uint32 rsv2 : 2;
    uint32 DSRImpl : 1;
    uint32 RdBlockMisalign : 1;
    uint32 WrBlockMisalign : 1;
    uint32 PartBlockRead : 1;
    uint32 RdBlockLen : 4;
    uint32 CardComdClasses : 12;
    // csd_tab[2]
    uint32 WrProtectGrSize : 7;
    uint32 EraseGrMul : 7;
    uint32 EraseGrSize : 1;
    uint32 DeviceSizeMul : 3;
    uint32 MaxWrCurrentVDDMax : 3;
    uint32 MaxWrCurrentVDDMin : 3;
    uint32 MaxRdCurrentVDDMax : 3;
    uint32 MaxRdCurrentVDDMin : 3;
    uint32 DeviceSizeL : 2;
    // csd_tab[3]
    uint32 rsv4 : 1;
    uint32 CSD_CRC : 7;
    uint32 ECC : 2;
    uint32 FileFormat : 2;
    uint32 TempWrProtect : 1;
    uint32 PermWrProtect : 1;
    uint32 CopyFlag : 1;
    uint32 FileFormatGrouop : 1;
    uint32 ContentProtectAppli : 1;
    uint32 rsv3 : 4;
    uint32 WriteBlockPaPartial : 1;
    uint32 MaxWrBlockLen : 4;
    uint32 WrSpeedFact : 3;
    uint32 ManDeflECC : 2;
    uint32 WrProtectGrEnable : 1;
};
union sd_csd {
    struct sd_csd_bits bits;
    uint32 words[4];
    uint8 bytes[16];
};

/*
 * card indentification data
 */
struct sd_cid_bits {
    uint32 ProdName1_H : 8;
    uint32 OEM_AppliID : 16;
    uint32 ManufacturerID : 8;
    uint32 ProdName2 : 8;
    uint32 ProdName1_L : 24;
    uint32 ProdSN_H : 24;
    uint32 ProdRev : 8;
    uint32 rsv2 : 1;
    uint32 CID_CRC : 7;
    uint32 ManufactDate : 12;
    uint32 rsv1 : 4;
    uint32 ProdSN_L : 8;
};
union sd_cid {
    struct sd_cid_bits bits;
    uint32 words[4];
    uint8 bytes[16];
};

struct sd_card {
    uint32 cardtype;
    uint32 rca;

    union sd_cid cid;
    union sd_csd csd;

    uint32 blocknum;
    uint32 blocksize;
    uint64 capacity;
};

enum SD_Error sdio_init(struct sd_card *card);
enum SD_Error sdio_read_block(const struct sd_card *card, uint32 bnum, uint8 *buf);
enum SD_Error sdio_write_block(const struct sd_card *card, uint32 bnum, const uint8 *buf);

#endif

