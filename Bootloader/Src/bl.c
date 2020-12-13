/*
 * sd_app.c
 *
 *  Created on: Dec 2, 2020
 *      Author: serbay
 */

#include "bl.h"
#include "flash.h"
#include "crc.h"
#include "sd_bsp.h"
#include "led_bsp.h"

bl_t bl;

static uint32_t write_address = FLASH_USER_START_ADDR;

static void bl_jump_to_user_app(void)
{
	typedef void (*pFunction)(void);
	uint32_t jump_address = *(__IO uint32_t*) (FLASH_USER_START_ADDR + 4);
	pFunction jump_to_app = (pFunction) jump_address;
	__set_MSP(*(__IO uint32_t*) FLASH_USER_START_ADDR);
	jump_to_app();
}

static void bl_welcome_message(bl_t *bl)
{
	printf("*******************************************\n"
		   "*********** STM32 SD Bootloader ***********\n"
		   "*******************************************\n");

	BL_PRINT("Bootloader is started\n");

	uint8_t version[3] = BL_VERSION;
	BL_PRINT("Version: %d.%d.%d\n", version[0], version[1], version[2]);

	bl->state = BL_CHECK_SD_CARD;
}

static void bl_check_sd_card(bl_t *bl)
{
	if (SD_CARD_IS_INSERTED()){
		bl->state = BL_INIT;
	}

	else{
		bl->err = BL_SD_NOT_EXIST;
		bl->state = BL_ERR;
	}
}

static void bl_open_firmware_file(bl_t *bl)
{
	if (sd_file_open_for_read(&bl->sd, FW_FILE_NAME) == SD_SUCCESS){
		BL_PRINT("Firmware file is successully opened\n");
		bl->fw_size = sd_get_file_size(&bl->sd);
		BL_PRINT("Firmware file size: %ld\n", bl->fw_size);
		bl->state = BL_FW_FILE_EXIST;
	}

	else{
		bl->err = BL_FW_FILE_NOT_EXIST;
		bl->state = BL_ERR;
	}
}

static void bl_read_firmware(bl_t *bl)
{
	static uint32_t read_cycle = 0;

	bool read_error = false;

	read_error = sd_file_read(&bl->sd, bl->data, FW_MEM_BLOCK);

	if (read_error || bl->sd.read_bytes == 0){
		bl->err = BL_FW_FILE_READ_ERR;
		bl->state = BL_ERR;
	}

	else if (!read_error && bl->sd.read_bytes < FW_MEM_BLOCK){
			bl->total_read_size += bl->sd.read_bytes;

			BL_PRINT("Read is completed. read_cycle: %ld\n", read_cycle);
			BL_PRINT("Last read bytes: %ld\n", bl->sd.read_bytes);

			for (int i = 0; i < 4; i++)
				bl->crc[i] = bl->data[bl->sd.read_bytes - i - 1];

			read_cycle = 0;
			sd_file_rewind(&bl->sd);

			if (bl->total_read_size == bl->fw_size){
				bl->state = BL_FW_FILE_READ_FINISHED;
				bl->total_read_size = 0;
			}

			else{
				bl->err = BL_FW_SIZE_DOES_NOT_MATCH_READ_SIZE;
				bl->state = BL_ERR;
			}

	}

	else{
		// BL_PRINT("[%ld] read is done...\n", read_cycle);
		bl->total_read_size += bl->sd.read_bytes;
		read_cycle++;
	}
}

static void bl_flash_unlock(bl_t *bl)
{
	if (!flash_unlock_and_erase()){
		bl->state = BL_FLASH_IS_UNLOCKED;
	}

	else{
		bl->err = BL_FLASH_UNLOCK_ERR;
		bl->state = BL_ERR;
	}
}

static void bl_flash_lock(bl_t *bl)
{
	if (!flash_lock()){
		bl->state = BL_FW_FILE_RENAME;
	}

	else{
		bl->err = BL_FLASH_LOCK_ERR;
		bl->state = BL_ERR;
	}
}

static inline uint32_t bl_merge_dword(char *data)
{
	uint32_t merged_data = data[0];
	merged_data |= data[1] << 8;
	merged_data |= data[2] << 16;
	merged_data |= data[3] << 24;

	return merged_data;
}

static bool bl_write_version_to_flash(void)
{
	uint32_t version[3] = BL_VERSION;

	uint32_t address = FLASH_USER_END_ADDR - BL_VERSION_MEM_SPACE  + 1;

	for (int i = 0; i < 3; i++)
	{
		if (flash_write(&address, version[i])){
			return true;
		}
	}

	return false;
}

static void bl_write_to_flash(bl_t *bl)
{
	for (uint32_t i = 0; i < bl->sd.read_bytes; i += 4){
		uint32_t merged_data = bl_merge_dword(bl->data + i);

		if (!flash_write(&write_address, merged_data)){
			if (bl->state == BL_FW_FILE_READ_FINISHED){
				if (!bl_write_version_to_flash())
					bl->state = BL_FW_FLASH_WRITE_FINISHED;
				else
					goto EXIT;
			}
		}

		else{
			EXIT:
				bl->err = BL_FW_FLASH_WRITE_ERR;
				bl->state = BL_ERR;
		}
	}
}

static void bl_check_app_sector(void)
{
	uint32_t version[3] =  {0};

	uint32_t address = FLASH_USER_END_ADDR - BL_VERSION_MEM_SPACE + 1;

	for (int i = 0; i < 3; i++){
		version[i] = flash_read(&address);
	}

	if (version[0] != 0xFFFFFFFF && version[1] != 0xFFFFFFFF && version[2] != 0xFFFFFFFF){
		BL_PRINT("User App is found and verified\n");
		BL_PRINT("User App was updated with Version: %ld.%ld.%ld before\n", version[0], version[1], version[2]);
		BL_PRINT("Jumping to User App...\n");
		bl_jump_to_user_app();
	}
}

static void bl_check_fw_size(bl_t *bl)
{
	if (bl->fw_size >= (FLASH_APP_SECTOR_SIZE - BL_VERSION_MEM_SPACE)){
		bl->err = BL_FW_SIZE_ERR;
		bl->state = BL_ERR;
	}

	else{
		bl->state = BL_FW_CHECK_CRC;
	}
}

static void bl_check_fw_crc(bl_t *bl)
{
	static uint32_t crc = 0;

	if (bl->state == BL_FW_FILE_READ_FINISHED){
		crc = rc_crc32(crc, bl->data, bl->sd.read_bytes - 4);

		uint32_t crc_merged = bl_merge_dword(bl->crc);

		if (crc_merged == crc){
			BL_PRINT("FW CRC is verified!\n");
			bl->state = BL_FLASH_UNLOCK;
		}

		else{
			bl->err = BL_CRC_ERR;
			bl->state = BL_ERR;
		}

		crc = 0;
	}

	else{
		crc = rc_crc32(crc, bl->data, bl->sd.read_bytes);
	}
}

static void bl_rename_fw_file(bl_t *bl)
{
	sd_file_close(&bl->sd);

	if (!sd_file_rename(FW_FILE_NAME, FW_FILE_RENAME_AFTER_UPDATE)){
		bl->state = BL_OP_COMPLETED;
	}

	else{
		bl->err = BL_FW_FILE_NOT_RENAMED;
		bl->state = BL_ERR;
	}
}

static void bl_operation_completed(void)
{
	BL_PRINT("Firmware Update is successfully completed\n");
	uint8_t version[3] = BL_VERSION;
	BL_PRINT("User App is updated with Version: %d.%d.%d\n", version[0], version[1], version[2]);
	BL_PRINT("Jumping to Reset Handler...\n");
	HAL_Delay(1); // Wait for last console output
	NVIC_SystemReset();
}

static void bl_init(bl_t *bl)
{
	if (!sd_init(&bl->sd)){
		bl->state = BL_SD_INIT_SUCCESS;
	}

	else{
		bl->err = BL_SD_INIT_ERR;
		bl->state = BL_ERR;
	}
}

static void bl_error_handler(bl_t *bl)
{
	switch (bl->err){
			case BL_SD_INIT_ERR:
				bl_check_app_sector();
				BL_PRINT("SD Card Initilization Error!\n");
				break;

			case BL_SD_NOT_EXIST:
				bl_check_app_sector();
				BL_PRINT("SD Card is not inserted!\n");
				break;

			case BL_FW_FILE_NOT_EXIST:
				bl_check_app_sector();
				BL_PRINT("Firmware file does not exist or cannot open!\n");
				break;

			case BL_FW_SIZE_DOES_NOT_MATCH_READ_SIZE:
				bl_check_app_sector();
				BL_PRINT("Firmware file size does not match total read size!\n");
				break;

			case BL_CRC_ERR:
				BL_PRINT("Firmware file has wrong CRC data!\n");
				break;

			case BL_FW_FILE_READ_ERR:
				BL_PRINT("Firmware file read error!\n");
				break;

			case BL_FW_SIZE_ERR:
				BL_PRINT("Firmware size error!\n");
				break;

			case BL_FW_FILE_NOT_RENAMED:
				BL_PRINT("Firmware file is not renamed!\n");
				BL_PRINT("There could be a file with same name of renamed fw file!\n");
				break;

			case BL_FLASH_UNLOCK_ERR:
				BL_PRINT("Flash Unlock Error!\n");
				break;

			case BL_FW_FLASH_WRITE_ERR:
				BL_PRINT("Flash Write Error!\n");
				break;

			case BL_FLASH_LOCK_ERR:
				BL_PRINT("Flash Lock Error!\n");
				break;

			default:
				break;
	}

	for (;;)
	{
		// User Defined Code for warning user
		LED_ERROR_TOGGLE();
		HAL_Delay(100);
	}
}

void bl_machine(bl_t *bl)
{
	switch (bl->state) {
		case BL_WELCOME:
			bl_welcome_message(bl);
			break;

		case BL_CHECK_SD_CARD:
			bl_check_sd_card(bl);
			break;

	    case BL_INIT:
	    	bl_init(bl);
	    	break;

		case BL_SD_INIT_SUCCESS:
			bl_open_firmware_file(bl);
			break;

		case BL_FW_FILE_EXIST:
			bl_check_fw_size(bl);
			break;

		case BL_FW_CHECK_CRC:
			bl_read_firmware(bl);
			bl_check_fw_crc(bl);
			break;

		case BL_FLASH_UNLOCK:
			bl_flash_unlock(bl);
			break;

		case BL_FLASH_IS_UNLOCKED:
			bl_read_firmware(bl);
			bl_write_to_flash(bl);
			break;

		case BL_FW_FLASH_WRITE_FINISHED:
			bl_flash_lock(bl);
			break;

		case BL_FW_FILE_RENAME:
			bl_rename_fw_file(bl);
			break;

		case BL_OP_COMPLETED:
			bl_operation_completed();
			break;

		case BL_ERR:
			bl_error_handler(bl);
			break;

		default:
			break;
	}
}
