/*
 * sdcard.h
 *
 *  Created on: Nov 30, 2020
 *      Author: serbay
 */

#ifndef INC_SDCARD_H_
#define INC_SDCARD_H_

#include <stdint.h>
#include <stdbool.h>

#include "fatfs.h"
#include "sdio.h"

enum {
	SD_SUCCESS,
	SD_ERR
};

#define SD_DEBUG              0

int printf(const char *p, ...);

#if SD_DEBUG
#define SD_PRINT(ch, ...) printf("SD_DEBUG: "), printf((ch), ##__VA_ARGS__)
#else
#define SD_PRINT(ch, ...)
#endif

typedef struct _sd{
	FATFS fs;
	FIL file;
	char path[4];

	uint32_t written_bytes;
	uint32_t read_bytes;
}sd_t;

bool sd_link_driver(sd_t *sd);
bool sd_mount(sd_t *sd);
void sd_file_create(sd_t *sd, const char *file_name);
bool sd_file_open_for_read(sd_t *sd, const char *file_name);
void sd_file_close(sd_t *sd);
void sd_file_write(sd_t *sd, const void *buf, uint32_t size);
bool sd_file_read(sd_t *sd, void *buf, uint32_t size);
bool sd_file_rename(const char *old_name, const char *new_name);
bool sd_set_file_pointer(sd_t *sd, uint32_t offset);
uint32_t sd_get_file_pointer(sd_t *sd);
void sd_file_rewind(sd_t *sd);
uint32_t sd_get_file_size(sd_t *sd);
bool sd_init(sd_t *sd);

#endif /* INC_SDCARD_H_ */
