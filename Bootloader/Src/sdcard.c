/*
 * sdcard.c
 *
 *  Created on: Nov 29, 2020
 *      Author: serbay
 */

#include "sdcard.h"

bool sd_link_driver(sd_t *sd)
{
	bool error = false;

	if(FATFS_LinkDriver(&SD_Driver, sd->path) == 0)
	{
		SD_PRINT("Link Success\n");
	}

	else
	{
		SD_PRINT("Link Error!\n");
		error = true;
	}

	return error;
}

bool sd_mount(sd_t *sd)
{
	bool error = false;

	if(f_mount(&sd->fs, (TCHAR const*)sd->path, 0) != FR_OK)
	{
		SD_PRINT("Mount Error!\n");
		error = true;
	}

	else
	{
		SD_PRINT("Mount Success\n");
	}

	return error;
}

void sd_file_create(sd_t *sd, const char *file_name)
{
	if(f_open(&sd->file, file_name, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK){
		SD_PRINT("file_create() Error!\n");
	}

	else{
		SD_PRINT("%s file is created...\n", file_name);
	}
}

bool sd_file_open_for_read(sd_t *sd, const char *file_name)
{
	bool error = false;

	if(f_open(&sd->file, file_name, FA_READ) != FR_OK){
		error = true;
		SD_PRINT("file_open_for_read() error!\n");
	}

	else{
		SD_PRINT("%s file is successfully opened\n", file_name);
	}

	return error;
}

void sd_file_close(sd_t *sd)
{
	f_close(&sd->file);
}

bool sd_file_rename(const char *old_name, const char *new_name)
{
	bool error = false;

	if (f_rename(old_name, new_name) != FR_OK){
		error = true;
		SD_PRINT("sd_file_rename() error!\n");
	}

	else{
		SD_PRINT("fw file is successfully renamed to %s\n", new_name);
	}

	return error;
}

void sd_file_write(sd_t *sd, const void *buf, uint32_t size)
{
	if (f_write(&sd->file, buf, (UINT)size, (UINT *)&sd->written_bytes) != FR_OK){
		SD_PRINT("file_write() error!\n");
	}

	else{
		SD_PRINT("file_write() success!\n");
		SD_PRINT("file_write() buf: %s\n", (char*)buf);
	}
}

bool sd_file_read(sd_t *sd, void *buf, uint32_t size)
{
	bool error = false;

	if (f_read(&sd->file, buf, size, (UINT*)&sd->read_bytes) != FR_OK){
		SD_PRINT("file_read() error!\n");
		error = true;
	}

	else{
		SD_PRINT("%ld bytes successfully read\n", sd->read_bytes);
	}

	return error;
}

bool sd_set_file_pointer(sd_t *sd, uint32_t offset)
{
	bool error = false;

	if (f_lseek(&sd->file, offset) != FR_OK){
		error = true;
	}

	return error;
}

uint32_t sd_get_file_pointer(sd_t *sd)
{
	return f_tell(&sd->file);
}

void sd_file_rewind(sd_t *sd)
{
	f_rewind(&sd->file);
}

uint32_t sd_get_file_size(sd_t *sd)
{
	return (uint32_t)f_size(&sd->file);
}

bool sd_init(sd_t *sd)
{
	SD_PRINT("FS is initiliazing...\n");

	if (sd_link_driver(sd) == SD_SUCCESS){
		if (sd_mount(sd) == SD_SUCCESS){
			SD_PRINT("FS init is successfully completed...\n");
			return false;
		}

		else
			return true;
	}

	return true;
}
