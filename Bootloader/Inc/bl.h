/*
 * sd_app.h
 *
 *  Created on: Dec 2, 2020
 *      Author: serbay
 */

#ifndef INC_BL_H_
#define INC_BL_H_

#include "sdcard.h"

#define BL_DEBUG     1

#if BL_DEBUG
#define BL_PRINT(ch, ...) printf("BL_DEBUG: "), printf((ch), ##__VA_ARGS__)
#else
#define BL_PRINT(ch, ...)
#endif

#define BL_VERSION             {1, 0, 0}
#define BL_VERSION_MEM_SPACE   ( 12 )

#define FW_FILE_NAME           		  "firmware.bin"
#define FW_FILE_RENAME_AFTER_UPDATE   "firmware_updated.bin"

#define FW_MEM_BLOCK            ( 512 )

typedef enum{
	BL_WELCOME,
	BL_CHECK_SD_CARD,
	BL_INIT,
	BL_APP_AREA_EXIST,
	BL_SD_EXIST,
	BL_SD_INIT_SUCCESS,
	BL_FW_FILE_EXIST,
	BL_FW_CHECK_CRC,
	BL_FLASH_UNLOCK,
	BL_FLASH_IS_UNLOCKED,
	BL_FW_FILE_READ_FINISHED,
	BL_FW_FLASH_WRITE_FINISHED,
	BL_FW_FILE_RENAME,
	BL_OP_COMPLETED,
	BL_ERR
}bl_state;

typedef enum{
	BL_NO_ERR,
	BL_SD_NOT_EXIST,
	BL_SD_INIT_ERR,
	BL_CRC_ERR,
	BL_FW_FILE_NOT_EXIST,
	BL_FW_FILE_NOT_RENAMED,
	BL_FW_FILE_READ_ERR,
	BL_FW_SIZE_ERR,
	BL_FW_SIZE_DOES_NOT_MATCH_READ_SIZE,
	BL_FLASH_UNLOCK_ERR,
	BL_FW_FLASH_WRITE_ERR,
	BL_FLASH_LOCK_ERR,
	BL_APP_AREA_NOT_EXIST
}bl_err;

typedef struct{
	sd_t sd;
	bl_state state;
	bl_err err;
	char data[FW_MEM_BLOCK];
	uint32_t fw_size;
	uint32_t total_read_size;
	char crc[4];
}bl_t;

extern bl_t bl;

void bl_machine(bl_t *bl);

#endif /* INC_BL_H_ */
